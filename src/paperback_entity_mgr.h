#pragma once
// Method 1 (Sorted):
// entity manager -> get list of archetypes -> iterate each archetype -> access entity info list in archetypes
// ( Need to add to Archetype as data ) -> access m_ComponentInfos to access GUID which determines component type

// Method 2 (Unsorted):
// entity manager -> entity info list -> iterate all entity info's pool details -> access component infos same as on top

//component::info_v<T_COMPONENT>::m_UID -> Component ID


namespace paperback::entity
{
    struct info
    {
        archetype::instance*            m_pArchetype = nullptr;
        vm::PoolDetails                 m_PoolDetails;
        component::entity::Validation   m_Validation;
    };

    struct manager
    {
        using PoolDetails       = vm::PoolDetails;
        using EntityListHead    = std::priority_queue<u32>;
        using EntityInfoList    = std::unique_ptr<entity::info[]>;
        using ArchetypeBitsList = std::vector<tools::bits>;
        using ArchetypeList     = std::vector<std::unique_ptr<archetype::instance>>;

        EntityInfoList            m_EntityInfos       = std::make_unique<entity::info[]>( settings::max_entities_v );
        ArchetypeList             m_pArchetypeList    {   };
        ArchetypeBitsList         m_ArchetypeBits     {   };
        uint32_t                  m_EntityIDTracker   { 0 };
        EntityListHead            m_AvailableIndexes  {   };

        PPB_INLINE
        void RegisterEntity( const PoolDetails Details, archetype::instance& Archetype ) noexcept;

        template < typename T_FUNCTION >
        void CreateEntity( T_FUNCTION&& Function, coordinator::instance& Coordinator ) noexcept;

        PPB_INLINE
        void RemoveEntity( const u32 SwappedGlobalIndex, const component::entity DeletedEntity ) noexcept;

        template < typename... T_COMPONENTS >
        archetype::instance& GetOrCreateArchetype( coordinator::instance& Coordinator ) noexcept;

        template < typename... T_COMPONENTS > // PRIVATE FN
        archetype::instance& CreateArchetype( coordinator::instance& Coordinator, const tools::bits& Signature ) noexcept;

        PPB_INLINE
        entity::info& GetEntityInfo( const component::entity Entity ) const noexcept;

        PPB_INLINE
        entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

        PPB_INLINE
        archetype::instance& GetArchetypeFromEntity( const u32 EntityID ) const noexcept;

        PPB_INLINE
        void FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept;

        template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search() const noexcept;

        PPB_INLINE
        archetype::instance* Search( const tools::bits& Bits ) const noexcept;

        PPB_INLINE
        std::vector<archetype::instance*> Search( const tools::query& Query ) const noexcept;

        PPB_INLINE
        void Terminate( void ) noexcept;



        // PRIVATE
        PPB_INLINE
        u32 AppendEntity() noexcept;

        PPB_INLINE
        archetype::instance& GetOrCreateArchetype( std::span<const component::info* const> Types, coordinator::instance& Coordinator ) noexcept;

        template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search( std::span<const component::info* const> Types ) const noexcept;
    };
}