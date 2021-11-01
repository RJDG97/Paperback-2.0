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
        using EntityListHead    = std::priority_queue< u32, std::vector<u32>, std::greater<u32> >;
        using EntityInfoList    = std::unique_ptr< entity::info[] >;
        using ArchetypeBitsList = std::vector< tools::bits >;
        using ArchetypeList     = std::vector< std::unique_ptr<archetype::instance> >;
        using ArchetypeMap      = std::unordered_map< u64, archetype::instance* >;


        //-----------------------------------
		//             Default
		//-----------------------------------

        PPB_INLINE
        manager( paperback::coordinator::instance& Coordinator );

        PPB_INLINE
        ~manager() 
        {
            ResetAllArchetypes();
        }

        PPB_INLINE
        void Initialize( void ) noexcept;


        //-----------------------------------
		//             Create
		//-----------------------------------

        template < typename T_FUNCTION = paperback::empty_lambda >
        void CreateEntity( T_FUNCTION&& Function = paperback::empty_lambda{} ) noexcept;

        template < typename... T_COMPONENTS >
        archetype::instance& GetOrCreateArchetype( void ) noexcept;

        PPB_INLINE
		archetype::instance& GetOrCreateArchetype( const tools::bits ArchetypeSignature ) noexcept;

        
        template < concepts::Callable T_FUNCTION = paperback::empty_lambda >
	    component::entity AddOrRemoveComponents( const component::entity Entity
								               , std::span<const component::info* const> Add 
								               , std::span<const component::info* const> Remove 
								               , T_FUNCTION&& Function = paperback::empty_lambda{} ) noexcept;


        //-----------------------------------
        //             Query
        //-----------------------------------

        template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search() const noexcept;

        PPB_INLINE
        archetype::instance* Search( const tools::bits& Bits ) const noexcept;

        PPB_INLINE
        std::vector<archetype::instance*> Search( const tools::query& Query ) const noexcept;


        //-----------------------------------
		//              Clear
		//-----------------------------------

        PPB_INLINE
		void ResetAllArchetypes( void ) noexcept;

        PPB_INLINE
        void Terminate( void ) noexcept;


        //-----------------------------------
        //             Getters
        //-----------------------------------

        PPB_INLINE
        entity::info& GetEntityInfo( const component::entity Entity ) const noexcept;

        PPB_INLINE
        entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

        PPB_INLINE
        std::vector<paperback::archetype::instance*> GetArchetypeList( void ) noexcept;

        PPB_INLINE
        paperback::component::entity& RegisterEntity( const PoolDetails Details
                                                    , archetype::instance& Archetype ) noexcept;

        PPB_INLINE
        void RemoveEntity( const u32 SwappedGlobalIndex
                         , const u32 DeletedEntityIndex ) noexcept;

    private:

        //-----------------------------------
        //         Helper Functions
        //-----------------------------------

        PPB_INLINE
        u32 AppendEntity() noexcept;

        PPB_INLINE
        archetype::instance& GetOrCreateArchetype( std::span<const component::info* const> Types
                                                 , const u32 Count) noexcept;

        PPB_INLINE
        archetype::instance& CreateAndInitializeArchetype( std::span<const component::info* const> Types
                                                         , const u32 Count
                                                         , const tools::bits& Signature ) noexcept;

        template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search( std::span<const component::info* const> Types ) const noexcept;


        paperback::coordinator::instance&   m_Coordinator;
        EntityInfoList                      m_EntityInfos       = std::make_unique<entity::info[]>( settings::max_entities_v );
        ArchetypeList                       m_pArchetypeList    {   };
        ArchetypeMap                        m_pArchetypeMap     {   };
        ArchetypeBitsList                   m_ArchetypeBits     {   };
        /*
        For the "Linked-List":
        - m_EntityHead stores -> m_PoolDetails.m_PoolIndex (Available Indexes)
        - m_Validation.m_Next -> Move / Delete Indexes
        */
        u32                                 m_EntityHead        { 0 };

    };
}