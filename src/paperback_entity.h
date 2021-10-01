#pragma once

namespace paperback::archetype
{
    struct instance
    {
        using PoolDetails    = vm::PoolDetails;
        using EntityIDList   = std::vector<uint16_t>;
        using ComponentPool  = std::array<vm::instance, 2>;
        using DeleteList     = std::vector<component::entity>;
        using ComponentInfos = std::span<const component::info* const>;

        coordinator::instance&        m_Coordinator;
        ComponentPool                 m_ComponentPool            {   };                           // Component Pool
        ComponentInfos                m_ComponentInfos           {   };                           // Component Infos
        DeleteList                    m_DeleteList               {   };                           // List of entities to be deleted
        tools::bits                   m_ComponentBits            {   };                           // Component Signature
        uint32_t                      m_EntityCount              { 0 };                           // Number of Entities within Archetype
        uint32_t                      m_PoolAllocationIndex      { 0 };                           // Determines which pool to allocate to - 0 Default      ( TO FIX )
        uint32_t                      m_ProcessReference         { 0 };                           // Set to 1 when Archetype is updating

        PPB_INLINE
        instance( coordinator::instance& Coordinator, const tools::bits& ComponentBits ) noexcept;

        PPB_INLINE
        void Init( std::span<const component::info* const> Types ) noexcept;

        template< typename T_CALLBACK >
        PoolDetails CreateEntity( T_CALLBACK&& Function ) noexcept;
        
        PPB_INLINE
        u32 DeleteEntity( const PoolDetails Details ) noexcept;

        PPB_INLINE
        void DestroyEntity( component::entity& Entity ) noexcept;

        template < typename T_COMPONENT >
        T_COMPONENT& GetComponent( const PoolDetails Details ) noexcept;

        template < typename... T_COMPONENTS >
        requires( (( std::is_reference_v<T_COMPONENTS> ) && ...) )
        constexpr auto GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept;

        template < typename... T_COMPONENTS >
        requires( !(( std::is_reference_v<T_COMPONENTS> ) && ...) )
        constexpr auto GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept;

        //template < typename T_FUNCTION >
        //component::entity& TransferExistingEntity( component::entity& Entity, T_FUNCTION&& Function ) noexcept;

        template < typename T_FUNCTION >
        requires( xcore::function::is_callable_v<T_FUNCTION>&&
                  std::is_same_v<typename xcore::function::traits<T_FUNCTION>::return_type, void>&&
                  xcore::function::traits<T_FUNCTION>::arg_count_v == 0 )
        void AccessGuard( T_FUNCTION&& Function ) noexcept;

        PPB_INLINE
        void UpdateStructuralChanges() noexcept;

        PPB_INLINE
        void SerializeAllEntities( paperback::JsonFile& Jfile ) noexcept;

        PPB_INLINE
        std::vector <rttr::instance> GetEntityComponents( const u32 Index ) noexcept;

        // Private Stuff
        PPB_INLINE
        vm::instance& GetPoolWithIndex( const uint32_t EntityPoolIndex ) noexcept;
    };
}