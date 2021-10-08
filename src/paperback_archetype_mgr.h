#pragma once

namespace paperback::entity
{
    struct info
    {
        archetype::instance*            m_pArchetype = nullptr;
        vm::PoolDetails                 m_PoolDetails;
        component::entity::Validation   m_Validation;
    };
}

namespace paperback::archetype
{
    class manager final
    {
    public:

        using PoolDetails       = vm::PoolDetails;
        using EntityListHead    = std::priority_queue<u32, std::vector<u32>, std::greater<u32> >;
        using EntityInfoList    = std::unique_ptr<entity::info[]>;
        using ArchetypeBitsList = std::vector<tools::bits>;
        using ArchetypeList     = std::vector<std::unique_ptr<archetype::instance>>;

        PPB_INLINE
        manager( paperback::coordinator::instance& Coordinator );

        PPB_INLINE
        void RegisterEntity( const PoolDetails Details, archetype::instance& Archetype ) noexcept;

        template < typename T_FUNCTION >
        void CreateEntity( T_FUNCTION&& Function, coordinator::instance& Coordinator ) noexcept;

        PPB_INLINE
        void RemoveEntity( const u32 SwappedGlobalIndex, const component::entity DeletedEntity ) noexcept;

        PPB_INLINE
		void ResetAllArchetypes( void ) noexcept;

        template < typename... T_COMPONENTS >
        archetype::instance& GetOrCreateArchetype( coordinator::instance& Coordinator ) noexcept;

        PPB_INLINE
		archetype::instance& GetOrCreateArchetype( const tools::bits ArchetypeSignature ) noexcept;

        PPB_INLINE
        archetype::instance& CreateArchetype( const tools::bits& Signature ) noexcept;

        PPB_INLINE
        entity::info& GetEntityInfo( const component::entity Entity ) const noexcept;

        PPB_INLINE
        entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

        PPB_INLINE
        archetype::instance& GetArchetypeFromEntity( const u32 EntityID ) const noexcept;

        PPB_INLINE
        std::vector<paperback::archetype::instance*> GetArchetypeList( void ) noexcept;

        template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search() const noexcept;

        PPB_INLINE
        archetype::instance* Search( const tools::bits& Bits ) const noexcept;

        PPB_INLINE
        std::vector<archetype::instance*> Search( const tools::query& Query ) const noexcept;

        template < concepts::Callable T_FUNCTION = paperback::empty_lambda >
	    component::entity AddOrRemoveComponents( const component::entity Entity
								               , std::span<const component::info* const> Add
								               , std::span<const component::info* const> Remove
								               , T_FUNCTION&& Function = paperback::empty_lambda{} ) noexcept;

        PPB_INLINE
        void Terminate( void ) noexcept;


    private:

        PPB_INLINE
        u32 AppendEntity() noexcept;

        PPB_INLINE
        archetype::instance& GetOrCreateArchetype( std::span<const component::info* const> Types, coordinator::instance& Coordinator ) noexcept;

        template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search( std::span<const component::info* const> Types ) const noexcept;


        EntityInfoList                      m_EntityInfos       = std::make_unique<entity::info[]>( settings::max_entities_v );
        ArchetypeList                       m_pArchetypeList    {   };
        ArchetypeBitsList                   m_ArchetypeBits     {   };
        uint32_t                            m_EntityIDTracker   { 0 };
        EntityListHead                      m_AvailableIndexes  {   };
        paperback::coordinator::instance&   m_Coordinator;
    };
}