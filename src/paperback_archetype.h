#pragma once

namespace paperback::archetype
{
    class instance final
    {
    public:
        using PoolDetails    = vm::PoolDetails;
        using EntityIDList   = std::vector<uint16_t>;
        using ComponentPool  = std::array<vm::instance, 2>;
        using MoveList       = std::vector<vm::PoolDetails>;
        using DeleteList     = std::vector<component::entity>;
        using guid           = xcore::guid::unit<64, struct archetype_info>;
        using ComponentInfos = std::array< const component::info*, paperback::settings::max_components_per_entity_v >;


        //-----------------------------------
        //            Default
        //-----------------------------------

        PPB_INLINE
        instance( coordinator::instance& Coordinator, const tools::bits& ComponentBits ) noexcept;

        PPB_INLINE
        void Init( std::span<const component::info* const> Types, const u32 NumComponents, const std::string Name = "Unnamed Archetype") noexcept;


        //-----------------------------------
        //        Create / Delete
        //-----------------------------------

        template< typename T_CALLBACK = paperback::empty_lambda >
        paperback::component::entity CreateEntity( T_CALLBACK&& Function = paperback::empty_lambda{} ) noexcept;

        template< typename T_CALLBACK = paperback::empty_lambda >
        paperback::component::entity CreatePrefab(T_CALLBACK&& Function = paperback::empty_lambda{}) noexcept;
        
        // Called By Prefab Instance's Archetype
        PPB_INLINE
        const u32 CloneEntity( component::entity& Entity ) noexcept;

        // Called By The Prefab Archetype
        PPB_INLINE
        const u32 ClonePrefab( const u32 PrefabPoolIndex ) noexcept;

        PPB_INLINE
        void DestroyEntity( component::entity& Entity ) noexcept;

        PPB_INLINE
        void Clear( void ) noexcept;


        //-----------------------------------
        //            Transfer
        //-----------------------------------

        template < typename T_FUNCTION = paperback::empty_lambda >
        component::entity TransferExistingEntity( const component::entity Entity, T_FUNCTION&& Function = paperback::empty_lambda{} ) noexcept;


        //-----------------------------------
        //             Getters
        //-----------------------------------

        template < typename T_COMPONENT >
        T_COMPONENT& GetComponent( const PoolDetails Details ) noexcept;

        template < typename... T_COMPONENTS >
        std::tuple<T_COMPONENTS&...> GetComponents( const PoolDetails Details ) noexcept;

        template < typename T_COMPONENT >
        T_COMPONENT* FindComponent( const PoolDetails Details ) noexcept;

        template < typename... T_COMPONENTS >
        std::tuple<T_COMPONENTS*...> FindComponents( const PoolDetails Details ) noexcept;

        PPB_INLINE
        u32 FindPrefabEntityGID( std::string_view EntityName ) noexcept;

        template < typename... T_COMPONENTS >
        requires( (( std::is_reference_v<T_COMPONENTS> ) && ...) )
        constexpr auto GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept;

        template < typename... T_COMPONENTS >
        requires( !(( std::is_reference_v<T_COMPONENTS> ) && ...) )
        constexpr auto GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept;


        //-----------------------------------
        //              Guard
        //-----------------------------------

        template < typename T_FUNCTION >
        requires( xcore::function::is_callable_v<T_FUNCTION>&&
                  std::is_same_v<typename xcore::function::traits<T_FUNCTION>::return_type, void>&&
                  xcore::function::traits<T_FUNCTION>::arg_count_v == 0 )
        void AccessGuard( T_FUNCTION&& Function ) noexcept;

        PPB_INLINE
        void UpdateStructuralChanges( void ) noexcept;


        //-----------------------------------
        //              Save
        //-----------------------------------

        PPB_INLINE
        void SerializeAllEntities( paperback::JsonFile& Jfile ) noexcept;

        //// Called by the Prefab Instance Archetype to Allocate Memory
        //PPB_INLINE
        //void InitializePrefabInstances( const u32 InstanceCount
        //                              , const u32 PrefabPoolIndex
        //                              , vm::instance& PrefabPool ) noexcept;


        //-----------------------------------
        //       Data Member Getters
        //-----------------------------------

        PPB_INLINE
        std::vector< std::pair < rttr::instance, component::type::guid > > GetEntityComponents( const u32 Index ) noexcept;

        PPB_INLINE
        u32 GetCurrentEntityCount( void ) const noexcept;

        PPB_INLINE
        ComponentPool& GetComponentPools( void ) noexcept;

        PPB_INLINE
        std::string GetName( void ) noexcept;

        PPB_INLINE
        void SetName( const std::string Name ) noexcept;

        PPB_INLINE 
        ComponentInfos& GetComponentInfos( void ) noexcept;

        PPB_INLINE
        const u32 GetComponentCount( void ) const noexcept;

        PPB_INLINE
        bool CheckComponentExistence( const component::info* Info) noexcept;

        PPB_INLINE
        const tools::bits GetComponentBits( void ) noexcept;

        PPB_INLINE
        const guid& GetArchetypeGuid( void ) const noexcept;


    private:

        PPB_INLINE
        const vm::PoolDetails AppendEntity( void ) noexcept;

        PPB_INLINE
        const vm::PoolDetails ClonePrefabComponents( const vm::PoolDetails PrefabInstanceDetails
                                                   , const u32             PrefabPoolIndex
                                                   , vm::instance&         PrefabPool ) noexcept;

        coordinator::instance&        m_Coordinator;                                              // Coordinator Reference
        ComponentPool                 m_ComponentPool            {   };                           // Component Pool
        ComponentInfos                m_ComponentInfos           {   };                           // Component Infos
        tools::bits                   m_ComponentBits            {   };                           // Component Signature
        std::string                   m_pName                    { "Unnamed Archetype" };         // Archetype name for reflecting in Editor
        guid                          m_ArchetypeGuid            {   };                           // Archetype unique signature
        u32                           m_PoolAllocationIndex      { 0 };                           // Determines which pool to allocate to - 0 Default
        u32                           m_ProcessReference         { 0 };                           // Set to 1 when Archetype is updating
    };
}