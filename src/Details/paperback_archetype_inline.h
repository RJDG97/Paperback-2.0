//#include "..\paperback_archetype.h"
#include "Components/Parent.h"
#include "Components/Offset.h"
#include "Components/Child.h"
#pragma once

namespace paperback::archetype
{
    //-----------------------------------
    //            Default
    //-----------------------------------
    instance::instance( coordinator::instance& Coordinator, const tools::bits& ComponentBits ) noexcept :
        m_Coordinator{ Coordinator },
        m_ComponentBits{ ComponentBits }
    { }

    void instance::Init( std::span<const component::info* const> Types, const u32 NumComponents, const std::string Name ) noexcept
    {
        // Deep copy infos
        for ( u32 i = 0; i < NumComponents; ++i )
            m_ComponentInfos[i] = Types[i];

        m_pName              = Name;
        m_ArchetypeGuid      = archetype::instance::guid{ m_ComponentBits.GenerateGUID() };

        for ( size_t i = 0, max = m_ComponentPool.size(); i < max; ++i )
            m_ComponentPool[ i ].Init( std::span{ m_ComponentInfos.data(), NumComponents }, NumComponents, &m_Coordinator );
    }


    //-----------------------------------
    //        Create / Delete
    //-----------------------------------

    template< typename T_CALLBACK >
    paperback::component::entity instance::CreateEntity( T_CALLBACK&& Function ) noexcept
    {
        using func_traits = xcore::function::traits<T_CALLBACK>;
        
        return [&]<typename... T_COMPONENTS>( std::tuple<T_COMPONENTS...>* ) -> paperback::component::entity
        {
            PPB_ASSERT_MSG( !( m_ComponentBits.Has( component::info_v<T_COMPONENTS>.m_UID ) && ... ), "Archetype CreateEntity: Creating entity with invalid components in function" );
        
            // Generate the next valid ID within m_ComponentPool
            const auto PoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].Append();
            // Do not register entity if it's invalid
            if ( PoolIndex == settings::invalid_index_v ) return paperback::component::entity
                                                                 {
                                                                     .m_GlobalIndex = settings::invalid_index_v
                                                                 };
            
            if ( !std::is_same_v<empty_lambda, T_CALLBACK> )
            {
                Function( m_ComponentPool[ m_PoolAllocationIndex ].GetComponent<T_COMPONENTS>( PoolIndex ) ... );
            }
        
            // Generates Global Index
            return m_Coordinator.RegisterEntity( paperback::vm::PoolDetails
                                                      {
                                                          .m_Key       = m_PoolAllocationIndex
                                                      ,   .m_PoolIndex = PoolIndex
                                                      }
                                                    , *this );
        }( reinterpret_cast<typename func_traits::args_tuple*>(nullptr) );
    }
    
    void instance::CloneEntity( component::entity& Entity ) noexcept
    {
        // Entity Info of Entity to clone
        auto& EntityInfo = m_Coordinator.GetEntityInfo( Entity );

        // Generate the next valid ID within m_ComponentPool
        const auto PoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].Append();
        auto UpdatedPoolDetails = paperback::vm::PoolDetails
                                  {
                                      .m_Key       = m_PoolAllocationIndex
                                  ,   .m_PoolIndex = PoolIndex
                                  };
        m_Coordinator.RegisterEntity( UpdatedPoolDetails
                                    , *this );

        if ( UpdatedPoolDetails.m_Key == EntityInfo.m_PoolDetails.m_Key )
            m_ComponentPool[ UpdatedPoolDetails.m_Key ].CloneComponents( PoolIndex, EntityInfo.m_PoolDetails.m_PoolIndex );

        // For when separate component pools are implemented in the future
        /* 
        else
            m_ComponentPool[ UpdatedPoolDetails.m_Key ].CloneComponents( PoolIndex, EntityInfo.m_PoolDetails.m_PoolIndex, &m_ComponentPool[ EntityInfo.m_PoolDetails.m_Key ] );
        */
    }
    
    void instance::DestroyEntity( component::entity& Entity ) noexcept
    {
        auto& EntityInfo = m_Coordinator.GetEntityInfo( Entity );
        m_ComponentPool[ EntityInfo.m_PoolDetails.m_Key ].DestroyEntity( Entity );

        UpdateStructuralChanges();
    }

    void instance::Clear(void) noexcept
    {
        for ( u32 i = 0, max = static_cast<u32>( m_ComponentPool.size() ); i < max; ++i )
        {
            u32 Count = m_ComponentPool[i].GetCurrentEntityCount();
            while ( Count-- )
            {
                DestroyEntity( GetComponent<paperback::component::entity>( vm::PoolDetails{ i, Count } ));
            }
        }
    }


    //-----------------------------------
    //              Guard
    //-----------------------------------
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

    void instance::UpdateStructuralChanges( void ) noexcept
    {
        if ( m_ProcessReference > 0 ) return;

        for ( auto& Pool : m_ComponentPool )
        {
            Pool.UpdateStructuralChanges();
        }
    }


    //-----------------------------------
    //              Save
    //-----------------------------------
    void instance::SerializeAllEntities( paperback::JsonFile& Jfile ) noexcept
    {
        for ( auto& Pool : m_ComponentPool )
        for ( u32 i = 0, max = Pool.GetCurrentEntityCount(); i < max; ++i )
        {
            Jfile.StartObject();

            Pool.SerializePoolComponentsAtEntityIndex( i, Jfile );

            Jfile.EndObject();
        }
    }

    //-----------------------------------
    //             Getters
    //-----------------------------------

    template < typename T_COMPONENT > 
    T_COMPONENT& instance::GetComponent( const PoolDetails Details ) noexcept
    {
        return m_ComponentPool[ Details.m_Key ].GetComponent<T_COMPONENT>( Details.m_PoolIndex );
    }

    template < typename... T_COMPONENTS >
    std::tuple<T_COMPONENTS&...> instance::GetComponents( const PoolDetails Details ) noexcept
    {
        return {
            GetComponent<T_COMPONENTS>( Details )...
        };
    }

    template < typename T_COMPONENT >
    T_COMPONENT* instance::FindComponent( const PoolDetails Details ) noexcept
    {
        return m_ComponentPool[ Details.m_Key ].FindComponent<T_COMPONENT>( Details.m_PoolIndex );
    }

    template < typename... T_COMPONENTS >
    std::tuple<T_COMPONENTS*...> instance::FindComponents( const PoolDetails Details ) noexcept
    {
        return {
            FindComponent<T_COMPONENTS>( Details )...
        };
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

            E.g. using query = std::tuple< paperback::query::must< my_tag_component > >;
        */
        
        if constexpr ( !(( paperback::BaseType<T_COMPONENTS>::typedef_v.id_v == paperback::component::type::id::DATA ) && ... ) )
        {
            WARN_PRINT( "Using component::type::id::TAG component in operator() parameter list - Consider adding tag components in the query instead" );
            ERROR_LOG( "Using component::type::id::TAG component in operator() parameter list - Consider adding tag components in the query instead" );
        }
    
        auto& MemoryPool = Pool.GetMemoryPool();
        std::array<std::byte*, sizeof...(T_COMPONENTS)> ComponentArray;
    
        [&]<typename... SORTED_COMPONENTS>( std::tuple<SORTED_COMPONENTS...>* ) constexpr noexcept
        {
            int Sequence = 0;

            (( ComponentArray[ xcore::types::tuple_t2i_v< SORTED_COMPONENTS, TupleComponents > ]
                 = &MemoryPool[ Pool.GetComponentIndexFromGUIDInSequence( component::info_v< SORTED_COMPONENTS >.m_Guid, Sequence ) ]
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

            E.g. using query = std::tuple< paperback::query::must< my_tag_component > >;
        */

        if constexpr ( !(( paperback::BaseType<T_COMPONENTS>::typedef_v.id_v == paperback::component::type::id::DATA ) && ... ) )
        {
            WARN_PRINT( "Using component::type::id::TAG component in operator() parameter list - Consider adding tag components in the query instead" );
            ERROR_LOG( "Using component::type::id::TAG component in operator() parameter list - Consider adding tag components in the query instead" );
        }
    
        auto& MemoryPool = Pool.GetMemoryPool();
        std::array<std::byte*, sizeof...(T_COMPONENTS)> ComponentArray;
    
        [&]<typename... SORTED_COMPONENTS>( std::tuple<SORTED_COMPONENTS...>* ) constexpr noexcept
		{
			(( ComponentArray[ xcore::types::tuple_t2i_v< SORTED_COMPONENTS, TupleComponents > ]
                = [&]<typename T_COMPONENT>( std::tuple<T_COMPONENT>* ) constexpr noexcept
			      {
			          int ComponentIndex = Pool.GetComponentIndexFromGUIDInSequence( component::info_v< SORTED_COMPONENTS >.m_Guid, 0 );
    
			          if constexpr ( std::is_pointer_v<T_COMPONENT> ) return ( ComponentIndex < 0 )
                                                                             ? nullptr 
                                                                             : &MemoryPool[ ComponentIndex ][ sizeof(std::decay<T_COMPONENT>) * PoolIndex ];
			          else									          return   &MemoryPool[ ComponentIndex ][ sizeof(std::decay<T_COMPONENT>) * PoolIndex ];
    
			      }( xcore::types::make_null_tuple_v< SORTED_COMPONENTS > ))
            , ... );
    
		}( xcore::types::null_tuple_v< SortedTuple > );

        return ComponentArray;
    }


    //-----------------------------------
    //         Transfer Entity
    //-----------------------------------

    template < typename T_FUNCTION >
    component::entity instance::TransferExistingEntity( const component::entity Entity, T_FUNCTION&& Function ) noexcept
    {
        using func_traits = xcore::function::traits<T_FUNCTION>;
        
        // Get info of Entity that is to be transferred to "new archetype" aka this one
        auto& TransferredEntityInfo = m_Coordinator.GetEntityInfo( Entity.m_GlobalIndex );

        // Transfer matching components over - m_ComponentPool[ 0 ].TransferExistingComponents
        const auto NewPoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].TransferExistingComponents( TransferredEntityInfo.m_PoolDetails                                                                 // Entity's Pool Key & Index
                                                                                                     , TransferredEntityInfo.m_pArchetype->m_ComponentPool[ TransferredEntityInfo.m_PoolDetails.m_Key ] ); // Entity's Pool

        TransferredEntityInfo.m_pArchetype->UpdateStructuralChanges();

        // Update Entity Info
        TransferredEntityInfo.m_pArchetype = this;
        TransferredEntityInfo.m_PoolDetails.m_Key = m_PoolAllocationIndex;
        TransferredEntityInfo.m_PoolDetails.m_PoolIndex = NewPoolIndex;

        //// Initialize stuff with the lambda function
        // Probably not needed but we shall see c:

        // Temporary Return
        return m_ComponentPool[ m_PoolAllocationIndex ].GetComponent<component::entity>( NewPoolIndex );
    }


    //-----------------------------------
    //       Data Member Getters
    //-----------------------------------

    std::vector <rttr::instance> instance::GetEntityComponents( const u32 Index ) noexcept
    {
        return m_ComponentPool[0].GetComponents(Index);
    }

    archetype::instance* instance::GetArchetypePointer( const u32 Index ) noexcept
    {
        auto& c_Entity = GetComponent<component::entity>(vm::PoolDetails{ 0, Index });
        auto& EntityInfo = m_Coordinator.GetEntityInfo(c_Entity);

        return EntityInfo.m_pArchetype;

    }

    std::string instance::GetName( void ) noexcept 
    {
        return m_pName;
    }

    void instance::SetName(const std::string Name) noexcept
    {
        m_pName = Name;
    }

    u32 instance::GetCurrentEntityCount( void ) const noexcept
    {
        return m_ComponentPool[0].GetCurrentEntityCount();
    }

    instance::ComponentPool& instance::GetComponentPools( void ) noexcept
    {
        return m_ComponentPool;
    }

    instance::ComponentInfos& instance::GetComponentInfos(void) noexcept
    {
        return m_ComponentInfos;
    }

    bool instance::CheckComponentExistence(const component::info* Info) noexcept
    {
        for ( u32 i = 0, max = m_ComponentPool[0].GetComponentCount(); i < max; ++i )
        {
            if (m_ComponentInfos[i] == Info)
                return true;
        }
        return false;
    }

    const u32 instance::GetComponentCount(void) const noexcept
    {
        return m_ComponentPool[0].GetComponentCount();
    }

    tools::bits& instance::GetComponentBits( void ) noexcept
    {
        return m_ComponentBits;
    }

    const instance::guid& instance::GetArchetypeGuid( void ) const noexcept
    {
        return m_ArchetypeGuid;
    }


    //-----------------------------------
    //             Private
    //-----------------------------------

    void instance::UnlinkChildrenAndParents( const PoolDetails Details ) noexcept
    {
        if ( m_ComponentBits.Has<parent>() )
        {
            auto& Parent = GetComponent<parent>( Details );

            while ( Parent.m_Infos.size() )
            {
                auto& ChildInfo = m_Coordinator.GetEntityInfo( Parent.m_Infos[ Parent.m_Infos.size() - 1 ].m_ChildGID );
                auto& Child     = ChildInfo.m_pArchetype->GetComponent< paperback::component::entity >( ChildInfo.m_PoolDetails );

                m_Coordinator.AddOrRemoveComponents< std::tuple<>
                                                 , std::tuple<child, offset>
                                                 >( Child );

                Parent.m_Infos.pop_back();
            }
        }
        else if ( m_ComponentBits.Has<child>() )
        {
            auto& Child         = GetComponent<child>( Details );
            auto& ChildEntity   = GetComponent<paperback::component::entity>( Details );
            auto& ParentInfo    = m_Coordinator.GetEntityInfo( Child.m_Info.m_ParentGID );
            
            auto& Parent        = ParentInfo.m_pArchetype->GetComponent< parent >( ParentInfo.m_PoolDetails );

            auto begin = Parent.m_Infos.begin(), end = Parent.m_Infos.end();
            for ( ; begin != end; ++begin )
            {
                if ( begin->m_ChildGID == ChildEntity.m_GlobalIndex ) break;
            }
            if ( begin != end )
                Parent.m_Infos.erase( begin );
        }
    }
}