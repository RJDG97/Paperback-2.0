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
        using ComponentInfos = std::array< const component::info*, paperback::settings::max_components_per_entity_v >;


        //-----------------------------------
        //            Default
        //-----------------------------------

        PPB_INLINE
        instance( coordinator::instance& Coordinator, const tools::bits& ComponentBits ) noexcept;

        PPB_INLINE
        void Init( std::span<const component::info* const> Types, const u32 NumComponents, const std::string Name = "unnamed archetype") noexcept;


        //-----------------------------------
        //        Create / Delete
        //-----------------------------------

        template< typename T_CALLBACK = paperback::empty_lambda >
        void CreateEntity( T_CALLBACK&& Function = paperback::empty_lambda{} ) noexcept;

        PPB_INLINE
        void CloneEntity( component::entity& Entity ) noexcept;
        
        PPB_INLINE
        u32 DeleteEntity( const PoolDetails Details ) noexcept;

        PPB_INLINE
        void DestroyEntity( component::entity& Entity ) noexcept;

        PPB_INLINE
        void Clear( void ) noexcept;


        //-----------------------------------
        //             Getters
        //-----------------------------------

        template < typename T_COMPONENT >
        T_COMPONENT& GetComponent( const PoolDetails Details ) noexcept;

        template < typename... T_COMPONENTS >
        requires( (( std::is_reference_v<T_COMPONENTS> ) && ...) )
        constexpr auto GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept;

        template < typename... T_COMPONENTS >
        requires( !(( std::is_reference_v<T_COMPONENTS> ) && ...) )
        constexpr auto GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept;

        template < typename T_FUNCTION = paperback::empty_lambda >
        component::entity TransferExistingEntity( const component::entity Entity, T_FUNCTION&& Function = paperback::empty_lambda{} ) noexcept;


        //-----------------------------------
        //              Guard
        //-----------------------------------

        template < typename T_FUNCTION >
        requires( xcore::function::is_callable_v<T_FUNCTION>&&
                  std::is_same_v<typename xcore::function::traits<T_FUNCTION>::return_type, void>&&
                  xcore::function::traits<T_FUNCTION>::arg_count_v == 0 )
        void AccessGuard( T_FUNCTION&& Function ) noexcept;

        PPB_INLINE
        void UpdateStructuralChanges() noexcept;


        //-----------------------------------
        //              Save
        //-----------------------------------

        PPB_INLINE
        void SerializeAllEntities( paperback::JsonFile& Jfile ) noexcept;

        PPB_INLINE
        std::vector <rttr::instance> GetEntityComponents( const u32 Index ) noexcept;

        PPB_INLINE 
        archetype::instance* GetArchetypePointer( const u32 Index ) noexcept;

        PPB_INLINE
        u32 GetEntityCount( void ) const noexcept;

        PPB_INLINE
        ComponentPool& GetComponentPools( void ) noexcept;

        PPB_INLINE
        std::string GetName( void ) noexcept;

        PPB_INLINE
        void SetName( const std::string Name ) noexcept;

        PPB_INLINE 
        ComponentInfos& GetComponentInfos( void ) noexcept;

        PPB_INLINE
        u32 GetComponentNumber(void) noexcept;




    private:

        coordinator::instance&        m_Coordinator;
        ComponentPool                 m_ComponentPool            {   };                           // Component Pool
        ComponentInfos                m_ComponentInfos           {   };                           // Component Infos
        MoveList                      m_MoveList                 {   };                           // List of entities to be moved
        DeleteList                    m_DeleteList               {   };                           // List of entities to be deleted
        tools::bits                   m_ComponentBits            {   };                           // Component Signature
        std::string                   m_pName                    { "Unnamed Archetype" };         // Archetype name for reflecting in Editor
        u32                           m_EntityCount              { 0 };                           // Number of Entities within Archetype
        u32                           m_PoolAllocationIndex      { 0 };                           // Determines which pool to allocate to - 0 Default      ( TO FIX )
        u32                           m_ProcessReference         { 0 };                           // Set to 1 when Archetype is updating
        u32                           m_NumberOfComponents       { 0 };                           // Number of components
    };
}