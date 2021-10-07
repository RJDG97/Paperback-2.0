#pragma once
// jy
namespace paperback::coordinator
{
	struct instance final
	{
		tools::clock				m_Clock;
		Input						m_Input;
		component::manager			m_CompMgr;
		entity::manager				m_EntityMgr{ *this };
		system::manager				m_SystemMgr{ m_Clock };
		bool						m_GameActive = true;

	//public:

		//-----------------------------------
		//             Default
		//-----------------------------------
		instance( void ) noexcept;
		instance( const instance& ) = delete;
		~instance( void ) noexcept;

		PPB_INLINE
		static instance& GetInstance( void ) noexcept;

		PPB_INLINE
		void Initialize( void ) noexcept;

		PPB_INLINE
		void Update( void ) noexcept;

		PPB_INLINE
		void Terminate( void ) noexcept;


		//-----------------------------------
		//           Registration
		//-----------------------------------

		template < concepts::System... T_SYSTEMS >
		constexpr void RegisterSystems() noexcept;
		
		template< typename... T_COMPONENTS >
		constexpr void RegisterComponents( void ) noexcept;

		PPB_INLINE
		void SaveScene( const std::string& FilePath ) noexcept;

		PPB_INLINE
		void OpenScene( const std::string& FilePath ) noexcept;


		//-----------------------------------
		//    Archetype / Entity Methods
		//-----------------------------------

		template < typename... T_COMPONENTS >
		archetype::instance& GetOrCreateArchetype( void ) noexcept;

		PPB_INLINE
		archetype::instance& CreateArchetype(const tools::bits& Signature) noexcept;

		template< typename T_FUNCTION >
		void CreateEntity( T_FUNCTION&& Function ) noexcept;

		template< typename T_FUNCTION >
		void CreateEntities( T_FUNCTION&& Function
						   , const u32 Count ) noexcept;

		PPB_INLINE
		void DeleteEntity( component::entity& Entity ) noexcept;

		PPB_INLINE
		void RemoveEntity( const uint32_t SwappedGlobalIndex, const component::entity Entity ) noexcept;

		void ResetAllArchetypes(void) noexcept;

		
		template < concepts::TupleSpecialization T_TUPLE_ADD
				 , concepts::TupleSpecialization T_TUPLE_REMOVE = std::tuple<>
				 , concepts::Callable			 T_FUNCTION     = paperback::empty_lambda >
		component::entity AddOrRemoveComponents( const component::entity Entity
											   , T_FUNCTION&& Function = paperback::empty_lambda{} ) noexcept;
		
		
		//-----------------------------------
		//           Query Methods
		//-----------------------------------

		template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search( void ) const noexcept;

		PPB_INLINE
        archetype::instance* Search( const tools::bits& Bits ) const noexcept;

        PPB_INLINE
        std::vector<archetype::instance*> Search( const tools::query& Query ) const noexcept;
        


		//-----------------------------------
		//           Game Loops
		//-----------------------------------

        template < concepts::Callable_Void T_FUNCTION >
        void ForEach( const std::vector<archetype::instance*>& ArchetypeList
					, T_FUNCTION&& Function ) noexcept;

        template < concepts::Callable_Bool T_FUNCTION >
        void ForEach( const std::vector<archetype::instance*>& ArchetypeList
					, T_FUNCTION&& Function ) noexcept;


		//-----------------------------------
		//        Getters / Setters
		//-----------------------------------

		PPB_INLINE
		entity::info& GetEntityInfo( component::entity& Entity ) const noexcept;

		PPB_INLINE
        entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

		PPB_INLINE
		const paperback::component::info* FindComponentInfo(const paperback::component::type::guid ComponentGuid) noexcept;


		PPB_INLINE
		archetype::instance* FindArchetype( const tools::bits& Signature ) noexcept;
		
		template< typename T_SYSTEM >
		T_SYSTEM* FindSystem( void ) noexcept;

		template< typename T_SYSTEM >
		T_SYSTEM& GetSystem( void ) noexcept;

		PPB_INLINE // To Place Inside Archetype Directly
		void FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept;

		PPB_FORCEINLINE
		float DeltaTime() const noexcept;

		PPB_FORCEINLINE
        void SetTimeScale( const float s = 1.0f ) noexcept;

		PPB_FORCEINLINE
        float GetTimeScale() const noexcept;
	};
}

static struct engine
{
	paperback::coordinator::instance& m_Coordinator = paperback::coordinator::instance::GetInstance();
	int m_Width = 1280;
	int m_Height = 720;
} m_Engine;

#define PPB m_Engine.m_Coordinator