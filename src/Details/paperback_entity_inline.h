#include "..\paperback_entity.h"
#pragma once

namespace paperback
{
    namespace archetype
    {
        instance::instance( coordinator::instance& Coordinator, const tools::bits& ComponentBits ) noexcept :
            m_Coordinator{ Coordinator },
            m_ComponentBits{ ComponentBits }
        { }

        void instance::Init( std::span<const component::info* const> Types, const u32 NumComponents ) noexcept
        {
            // Deep copy infos
            for ( u32 i = 0; i < NumComponents; ++i )
                m_ComponentInfos[i] = Types[i];
            m_NumberOfComponents = NumComponents;

            for ( size_t i = 0, max = m_ComponentPool.size(); i < max; ++i )
                m_ComponentPool[ i ].Init( std::span{ m_ComponentInfos.data(), m_NumberOfComponents }, m_NumberOfComponents );
        }

        template< typename T_CALLBACK >
        instance::PoolDetails instance::CreateEntity( T_CALLBACK&& Function ) noexcept
        {
            using func_traits = xcore::function::traits<T_CALLBACK>;

            assert( ++m_EntityCount < settings::max_entities_v );

            return [&]<typename... T_COMPONENTS>( std::tuple<T_COMPONENTS...>* )
            {
                assert( m_ComponentBits.Has( component::info_v<T_COMPONENTS>.m_UID ) && ... );

                // Generate the next valid ID within m_ComponentPool
                const auto PoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].Append();

                if ( !std::is_same_v<empty_lambda, T_CALLBACK> )
                {
                    Function( m_ComponentPool[ m_PoolAllocationIndex ].GetComponent<T_COMPONENTS>( PoolIndex ) ... );
                }
        
                return PoolDetails
                {
                    .m_Key = m_PoolAllocationIndex
                ,   .m_PoolIndex = PoolIndex
                };

            }( reinterpret_cast<typename func_traits::args_tuple*>( nullptr ) );
        }

        u32 instance::DeleteEntity( const PoolDetails Details ) noexcept
        {
            return m_ComponentPool[ Details.m_Key ].Delete( Details.m_PoolIndex );
        }

        void instance::DestroyEntity( component::entity& Entity ) noexcept
        {
            assert( Entity.IsZombie() == false );

            auto& EntityInfo = m_Coordinator.GetEntityInfo( Entity );
            auto& PoolEntity = GetComponent<component::entity>( EntityInfo.m_PoolDetails );

            assert( &Entity == &PoolEntity );

            Entity.m_Validation.m_bZombie
                = EntityInfo.m_Validation.m_bZombie
                = true;

            m_DeleteList.push_back( Entity );

            if ( m_ProcessReference == 0 ) UpdateStructuralChanges();
        }

        template < typename T_FUNCTION >
        requires( xcore::function::is_callable_v<T_FUNCTION>&&
                  std::is_same_v<typename xcore::function::traits<T_FUNCTION>::return_type, void>&&
                  xcore::function::traits<T_FUNCTION>::arg_count_v == 0 )
        void instance::AccessGuard( T_FUNCTION&& Function ) noexcept
        {
            ++m_ProcessReference;
            Function();
            if ( --m_ProcessReference == 0 ) UpdateStructuralChanges();
        }

        void instance::UpdateStructuralChanges() noexcept
        {
            if ( m_ProcessReference > 0 ) return;

            for (auto& Entity : m_DeleteList)
            {
                auto& Info = m_Coordinator.GetEntityInfo( Entity );
                m_Coordinator.RemoveEntity( DeleteEntity( Info.m_PoolDetails ), Entity );
            }

            std::sort( m_MoveList.begin(), m_MoveList.end(), []( vm::PoolDetails a, vm::PoolDetails b )
                                                             {
                                                                 return a.m_PoolIndex > b.m_PoolIndex;
                                                             }
            );

            for ( auto& [Key, PoolIndex] : m_MoveList )
            {
                m_ComponentPool[ Key ].RemoveTransferredEntity( PoolIndex );
            }

            m_DeleteList.clear();
            m_MoveList.clear();
        }

        void instance::SerializeAllEntities( paperback::JsonFile& Jfile ) noexcept
        {
            for (u32 j = 0; j < m_EntityCount; ++j)
            {
                auto& c_Entity = GetComponent<component::entity>(vm::PoolDetails{ 0, j });

                auto& EntityInfo = m_Coordinator.GetEntityInfo(c_Entity);
                Jfile.StartObject();
                //jfile.Write(EntityInfo); // im not super sure what we wanna store here,probably using EntityInfo to store m_PoolDetails? for pool indexing stuffs

                m_ComponentPool[0].SerializePoolComponentsAtEntityIndex( j, Jfile ); // Pool Index 0 Only For Now

                Jfile.EndObject();
            }
        }

        template < typename T_COMPONENT > 
        T_COMPONENT& instance::GetComponent( const PoolDetails Details ) noexcept
        {
            return m_ComponentPool[ Details.m_Key ].GetComponent<T_COMPONENT>( Details.m_PoolIndex );
        } 

        template < typename... T_COMPONENTS >
        requires( (( std::is_reference_v<T_COMPONENTS> ) && ... ))
        constexpr auto instance::GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept
        {
            using TupleComponents = std::tuple< T_COMPONENTS... >;
            using SortedTuple = component::details::sort_tuple_t< TupleComponents >;

            /*
                Do not include component::type::tag components inside of T_SYSTEM::operator() parameters
                Manually include the tag component inside of the relevant paperback::query category inside of a system / query ( must, one_of, none_of )

                E.g.
                using query = std::tuple<
                    paperback::query::must< my_tag_component >
                >;
            */
            assert( (( paperback::BaseType<T_COMPONENTS>::typedef_v.id_v == paperback::component::type::id::DATA ) && ... ) );
        
            std::array<std::byte*, sizeof...(T_COMPONENTS)> ComponentArray;
        
            [&]<typename... SORTED_COMPONENTS>( std::tuple<SORTED_COMPONENTS...>* ) constexpr noexcept
            {
                int Sequence = 0;

                (( ComponentArray[ xcore::types::tuple_t2i_v< SORTED_COMPONENTS, TupleComponents > ]
                     = &Pool.m_ComponentPool[ Pool.GetComponentIndexFromGUIDInSequence( component::info_v< SORTED_COMPONENTS >.m_Guid, Sequence ) ]
                                            [ component::info_v< SORTED_COMPONENTS >.m_Size * PoolIndex ])
                 , ... );
        
            }( xcore::types::null_tuple_v< SortedTuple > );
        
            return ComponentArray;
        }
        
        template < typename... T_COMPONENTS >
        requires( !(( std::is_reference_v<T_COMPONENTS> ) && ...))
        constexpr auto instance::GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept
        {
            using TupleComponents = std::tuple< T_COMPONENTS... >;
            using SortedTuple = component::details::sort_tuple_t< TupleComponents >;

            /*
                Do not include component::type::tag components inside of T_SYSTEM::operator() parameters
                Manually include the tag component inside of the relevant paperback::query category inside of a system / query ( must, one_of, none_of )

                E.g.
                using query = std::tuple<
                    paperback::query::must< my_tag_component >
                >;
            */
            assert( (( paperback::BaseType<T_COMPONENTS>::typedef_v.id_v == paperback::component::type::id::DATA ) && ... ) );
        
            std::array<std::byte*, sizeof...(T_COMPONENTS)> ComponentArray;
        
            [&]<typename... SORTED_COMPONENTS>( std::tuple<SORTED_COMPONENTS...>* ) constexpr noexcept
			{
				(( ComponentArray[ xcore::types::tuple_t2i_v< SORTED_COMPONENTS, TupleComponents > ]
                    = [&]<typename T_COMPONENT>( std::tuple<T_COMPONENT>* ) constexpr noexcept
				      {
				          int ComponentIndex = Pool.GetComponentIndexFromGUIDInSequence( component::info_v< SORTED_COMPONENTS >.m_Guid, 0 );
        
				          if constexpr ( std::is_pointer_v<T_COMPONENT> ) return ( ComponentIndex < 0 )
                                                                                 ? nullptr 
                                                                                 : &Pool.m_ComponentPool[ ComponentIndex ][ sizeof(std::decay<T_COMPONENT>) * PoolIndex ];
				          else									          return   &Pool.m_ComponentPool[ ComponentIndex ][ sizeof(std::decay<T_COMPONENT>) * PoolIndex ];
        
				      }( xcore::types::make_null_tuple_v< SORTED_COMPONENTS > ))
                , ... );
        
			}( xcore::types::null_tuple_v< SortedTuple > );

            return ComponentArray;
        }

        template < typename T_FUNCTION >
        component::entity instance::TransferExistingEntity( const component::entity Entity, T_FUNCTION&& Function ) noexcept
        {
            using func_traits = xcore::function::traits<T_FUNCTION>;

            // Get info of Entity that is to be transferred to "new archetype" aka this one
            auto& EntityInfo = m_Coordinator.GetEntityInfo( Entity.m_GlobalIndex );

            // Transfer matching components over - m_ComponentPool[ 0 ].TransferExistingComponents
            const auto NewPoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].TransferExistingComponents( EntityInfo.m_PoolDetails                                                      // Entity's Pool Key & Index
                                                                                                         , EntityInfo.m_pArchetype->m_ComponentPool[ EntityInfo.m_PoolDetails.m_Key ] ); // Entity's Pool

            // Testing Replacement - Adding to move list to reoganize afterwards
            EntityInfo.m_pArchetype->m_MoveList.push_back( EntityInfo.m_PoolDetails );

            // Update Entity Info
            EntityInfo.m_pArchetype = this;
            EntityInfo.m_PoolDetails.m_Key = m_PoolAllocationIndex;
            EntityInfo.m_PoolDetails.m_PoolIndex = NewPoolIndex;
            ++m_EntityCount;

            // Initialize stuff with the lambda function

            // Temporary Return
            return m_ComponentPool[ m_PoolAllocationIndex ].GetComponent<component::entity>( NewPoolIndex );
        }

        vm::instance& instance::GetPoolWithIndex( const uint32_t EntityPoolIndex ) noexcept
        {
            auto Pool = m_ComponentPool.begin();
            std::advance( Pool, EntityPoolIndex );
            return *Pool;
        }
    }
}