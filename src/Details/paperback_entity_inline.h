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

        void instance::Init( std::span<const component::info* const> Types ) noexcept
        {
            m_ComponentInfos = Types;
            for ( size_t i = 0, max = m_ComponentPool.size(); i < max; ++i )
                m_ComponentPool[ i ].Init( Types );
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

            }( reinterpret_cast<func_traits::args_tuple*>( nullptr ) );
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

            m_DeleteList.clear();
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

        //template < concepts::ReferenceArgs... T_COMPONENTS >
        //constexpr auto instance::GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept
        //{
        //    using TupleComponents = std::tuple< T_COMPONENTS... >;
        //    using SortedTuple = component::details::sort_tuple_t< TupleComponents >;
        //
        //    std::array<std::byte*, sizeof...(T_COMPONENTS)> ComponentArray;
        //
        //    [&]<typename... SORTED_COMPONENTS>( std::tuple<SORTED_COMPONENTS...>* ) constexpr noexcept
        //    {
        //        (( ComponentArray[ xcore::types::tuple_t2i_v< SORTED_COMPONENTS, TupleComponents > ]
        //             = [&]<typename T_COMPONENT>( std::tuple<T_COMPONENT>* ) constexpr noexcept
        //               {
        //                   auto ComponentIndex = Pool.GetComponentIndexFromGUIDInSequence( component::info_v< SORTED_COMPONENTS >.m_Guid, 0 );
        //                   return Pool.m_ComponentPool[ ComponentIndex ][ component::info_v< T_COMPONENT >.m_Size * PoolIndex ];
        //             
        //               }( xcore::types::make_null_tuple_v< SORTED_COMPONENTS > )),
        //         ... );
        //
        //    }( xcore::types::null_tuple_v< SortedTuple > );
        //
        //    return ComponentArray;
        //}
        //
        //template < concepts::MixedArgs... T_COMPONENTS >
        //constexpr auto instance::GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept
        //{
        //    using TupleComponents = std::tuple< T_COMPONENTS... >;
        //    using SortedTuple = component::details::sort_tuple_t< TupleComponents >;
        //
        //    std::array<std::byte*, sizeof...(T_COMPONENTS)> ComponentArray;
        //
        //    [&]<typename... SORTED_COMPONENTS>( std::tuple<SORTED_COMPONENTS...>* ) constexpr noexcept
		//	{
		//		(( ComponentArray[ xcore::types::tuple_t2i_v< SORTED_COMPONENTS, TupleComponents > ]
        //            = [&]<typename T_COMPONENT>( std::tuple<T_COMPONENT>* ) constexpr noexcept
		//		      {
		//		          int ComponentIndex = Pool.GetComponentIndexFromGUIDInSequence( component::info_v< SORTED_COMPONENTS >.m_Guid, 0 );
        //
		//		          if constexpr ( std::is_pointer_v<T_COMPONENT> ) return ( ComponentIndex < 0 )
        //                                                                         ? nullptr 
        //                                                                         : Pool.m_ComponentPool[ ComponentIndex ][ sizeof(std::decay<T_COMPONENT>) * PoolIndex ];
		//		          else									          return   Pool.m_ComponentPool[ ComponentIndex ][ sizeof(std::decay<T_COMPONENT>) * PoolIndex ];
        //
		//		      }( xcore::types::make_null_tuple_v< SORTED_COMPONENTS > )),
        //        ... );
        //
		//	}( xcore::types::null_tuple_v< SortedTuple > );
        //}

        //template < typename T_FUNCTION >
        //component::entity& instance::TransferExistingEntity( component::entity& Entity, T_FUNCTION&& Function ) noexcept
        //{
        //    using func_traits = xcore::function::traits<T_FUNCTION>;

        //    auto& EntityInfo = m_Coordinator.GetEntityInfo( Entity.m_GlobalIndex );
        //    const auto NewPoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].TransferExistingComponents
        //    (
        //        EntityInfo.m_PoolDetails
        //    ,   EntityInfo.m_pArchetype.m_ComponentPool[ EntityInfo.m_PoolDetails.m_Key ]
        //    );

        //    EntityInfo.m_pArchetype = this;
        //    EntityInfo.m_PoolDetails.m_Key = m_PoolAllocationIndex;
        //    EntityInfo.m_PoolDetails.m_PoolIndex = NewPoolIndex;

        //    // TEMP
        //    [&]<typename... T_COMPONENTS>( std::tuple<T_COMPONENTS...>* )
        //    {
        //        assert( m_ComponentBits.Has( component::info_v<T_COMPONENTS>.m_UID ) && ... );
        //
        //        // const auto PoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].Append();

        //        if ( !std::is_same_v<empty_lambda, T_FUNCTION> )
        //        {
        //            Function( m_ComponentPool[m_PoolAllocationIndex].GetComponent<T_COMPONENTS>(NewPoolIndex) ... );
        //        }

        //        // GOTTA RETURN ENTITY COMPONENT WITH UPDATED INDEX

        //    }( reinterpret_cast<func_traits::args_tuple*>( nullptr ) );
        //}


        vm::instance& instance::GetPoolWithIndex( const uint32_t EntityPoolIndex ) noexcept
        {
            auto Pool = m_ComponentPool.begin();
            std::advance( Pool, EntityPoolIndex );
            return *Pool;
        }
    }
}