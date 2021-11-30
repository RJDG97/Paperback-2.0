#pragma once
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;
namespace paperback::coordinator
{
	class instance final
	{
	public:

		//-----------------------------------
		//             Default
		//-----------------------------------
		PPB_INLINE
		instance( void ) noexcept;

		PPB_INLINE
		instance( const instance& ) = delete;

		PPB_INLINE
		~instance( void ) noexcept;

		PPB_INLINE
		static instance& GetInstance( void ) noexcept;

		PPB_INLINE
		void Initialize( void ) noexcept;

		PPB_INLINE
		void Update( void ) noexcept;

		PPB_INLINE
		void Terminate( void ) noexcept;

		PPB_INLINE
		void QuitApplication(void) noexcept;

		//-----------------------------------
		//           Registration
		//-----------------------------------

		template < concepts::System... T_SYSTEMS >
		constexpr void RegisterSystems() noexcept;
		
		template< typename... T_COMPONENTS >
		constexpr void RegisterComponents( void ) noexcept;

		template < typename T_EVENT >
        void RegisterEvent( void ) noexcept;

        template < paperback::concepts::Event T_EVENT
                 , typename                   T_CLASS >
        void RegisterEventClass( T_CLASS* Class ) noexcept;


        //-----------------------------------
        //             Removal
        //-----------------------------------

        template < paperback::concepts::Event T_EVENT >
        void RemoveEvent( void ) noexcept;

        template < paperback::concepts::Event T_EVENT
                 , typename                   T_CLASS >
        void RemoveEventClass( T_CLASS* Class ) noexcept;


		//-----------------------------------
		//          Serialization
		//-----------------------------------

		PPB_INLINE
		void SaveScene( const std::string& FilePath, const std::string& EntityInfoPath ) noexcept;

		PPB_INLINE
		void OpenScene( const std::string& SceneName) noexcept;

		PPB_INLINE
		void QueueScene( const std::string& SceneName ) noexcept;

		PPB_INLINE
		void OpenQueuedScene() noexcept;

		PPB_INLINE
		void LoadEntityInfo( const std::string& FilePath ) noexcept;

		PPB_INLINE
		void SaveEntityInfo( const std::string& FilePath ) noexcept;
		
		PPB_INLINE
		void OpenEditScene( const std::string& FilePath, const std::string& EntityInfoPath ) noexcept;

		PPB_INLINE
		void SavePrefabs( const std::string& FilePath ) noexcept;

		PPB_INLINE
		void SaveIndividualPrefab( const std::string& FilePath, paperback::archetype::instance* PrefabArchetype, const paperback::u32 Index ) noexcept;

		PPB_INLINE
		void LoadPrefabs( const std::string& FilePath ) noexcept;

		PPB_INLINE
		bool VerifyState( const std::string& StateName ) noexcept;

		PPB_INLINE
		void ResetSystems() noexcept;

		//-----------------------------------
		//    Archetype / Entity Methods
		//-----------------------------------

		template < typename... T_COMPONENTS >
		archetype::instance& GetOrCreateArchetype( void ) noexcept;

		PPB_INLINE
		archetype::instance& GetOrCreateArchetype( const tools::bits ArchetypeSignature
												 , std::string ArchetypeName = "Unnamed Archetype" ) noexcept;

		PPB_INLINE
		void CreatePrefab( void ) noexcept;

		template< typename T_FUNCTION = paperback::empty_lambda >
		void CreateEntity( T_FUNCTION&& Function = paperback::empty_lambda{} ) noexcept;

		template< typename T_FUNCTION = paperback::empty_lambda >
		void CreateEntities( T_FUNCTION&& Function = paperback::empty_lambda{}
						   , const u32 Count = 1 ) noexcept;

		PPB_INLINE
		void DeleteEntity( component::entity& Entity ) noexcept;

		PPB_INLINE
		void ResetAllArchetypes( void ) noexcept;

		
		template < concepts::TupleSpecialization T_TUPLE_ADD
				 , concepts::TupleSpecialization T_TUPLE_REMOVE = std::tuple<>
				 , concepts::Callable			 T_FUNCTION     = paperback::empty_lambda >
		component::entity AddOrRemoveComponents( const component::entity Entity
											   , T_FUNCTION&& Function = paperback::empty_lambda{} ) noexcept;

		template < concepts::Callable T_FUNCTION = paperback::empty_lambda >
	    component::entity AddOrRemoveComponents( const component::entity Entity
								               , std::span<const component::info*> Add 
								               , std::span<const component::info*> Remove
								               , T_FUNCTION&& Function = paperback::empty_lambda{} ) noexcept;

		template < typename T_COMPONENT >
		void UpdatePrefabInstancesOnPrefabComponentUpdate( const entity::info& PrefabInfo
														 , const T_COMPONENT&  UpdatedComponent ) noexcept;

		
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

		PPB_INLINE
		void TogglePause( const bool& Status ) noexcept;


		//-----------------------------------
		//        Getters / Setters
		//-----------------------------------

		PPB_INLINE
		paperback::entity::info& GetEntityInfo( component::entity& Entity ) const noexcept;

		PPB_INLINE
        paperback::entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

		template< typename T_SYSTEM >
		T_SYSTEM* FindSystem( void ) noexcept;

		template< typename T_SYSTEM >
		T_SYSTEM& GetSystem( void ) noexcept;

		PPB_INLINE
		const paperback::component::info* FindComponentInfoFromUID( const u32 ComponentUID ) noexcept;

		PPB_INLINE
		std::vector<paperback::archetype::instance*> GetArchetypeList( void ) noexcept;

		PPB_INLINE
        const paperback::component::info* FindComponentInfo( const paperback::component::type::guid ComponentGuid ) noexcept;

		PPB_INLINE
		paperback::component::manager::ComponentInfoMap& GetComponentInfoMap() noexcept;

		PPB_INLINE
		std::vector<fs::path>& GetDragDropFiles() noexcept;

		PPB_INLINE
		void SetEntityHead( u32 NewEntityHead ) noexcept;

		PPB_INLINE
		paperback::archetype::manager::EntityInfoList& GetEntityInfoList() noexcept;

		PPB_INLINE
		paperback::archetype::instance& GetArchetype( const u64 ArchetypeGuid ) noexcept;


		//-----------------------------------
		//           CPP Scripts
		//-----------------------------------

		template < paperback::concepts::Script... T_SCRIPT >
        void RegisterScripts( void ) noexcept;

        template < paperback::concepts::Script T_SCRIPT >
        T_SCRIPT* FindScript( const paperback::u64 ScriptGuid ) const noexcept;

        template < paperback::concepts::Script T_SCRIPT >
        T_SCRIPT& GetScript( const paperback::u64 ScriptGuid ) const noexcept;

        PPB_FORCEINLINE
        const script::manager::CPPScriptsList& GetScriptsList( void ) const noexcept;


		//-----------------------------------
		//              Clock
		//-----------------------------------

		PPB_FORCEINLINE
		float DeltaTime() const noexcept;

		PPB_FORCEINLINE
        void SetTimeScale( const float s = 1.0f ) noexcept;

		PPB_FORCEINLINE
        float GetTimeScale() const noexcept;

		PPB_INLINE
        auto Now() noexcept -> decltype( std::chrono::high_resolution_clock::now() );

		PPB_INLINE
		u32 GetFPS( void ) noexcept;


		//-----------------------------------
		//              Input
		//-----------------------------------

		PPB_INLINE
		void UpdateInputs() noexcept;

		PPB_INLINE
		void SetKey( int Key, int Action ) noexcept;

		PPB_INLINE
		void SetMouse( int Key, int Action ) noexcept;

		PPB_INLINE
		bool IsKeyPress( int Key ) noexcept;

		PPB_INLINE
		bool IsKeyPressDown( int Key ) noexcept;

		PPB_INLINE
		bool IsKeyPressUp( int Key ) noexcept;

		PPB_INLINE
		bool IsMousePress( int Key ) noexcept;

		PPB_INLINE
		bool IsMouseDown( int Key ) noexcept;

		PPB_INLINE
		bool IsMouseUp( int Key ) noexcept;

		PPB_INLINE
		glm::vec3 GetMousePosition() noexcept;

		PPB_INLINE
		glm::vec2 GetMouseDirection() noexcept;

		PPB_INLINE
		glm::vec3 GetViewportMousePosition(glm::vec2 viewport_min, glm::vec2 viewport_max) noexcept;

		//-----------------------------------
        //         Event Broadcast
        //-----------------------------------

        template < paperback::concepts::Event T_EVENT
                 , typename...                T_ARGS >
        void BroadcastEvent( T_ARGS&&... Args ) const noexcept;


		/*
        /*! Friend Classes
        */
		friend class paperback::vm::instance;
		friend class paperback::archetype::instance;


	protected:

		PPB_INLINE
		paperback::component::entity& RegisterEntity( const paperback::vm::PoolDetails
						                            , archetype::instance& Archetype ) noexcept;

		PPB_INLINE
		void RemoveEntity( const u32 SwappedGlobalIndex
						 , const u32 DeletedEntityIndex ) noexcept;


	private:

		scene_mgr					m_SceneMgr;						// Scene Manager
		std::vector<fs::path>		m_DragDropFiles;				// External Files 
		tools::clock				m_Clock;						// Timer
		event::manager				m_GlobalEventMgr;				// Global Event Manager
		component::manager			m_CompMgr;						// Component Manager
		archetype::manager			m_ArchetypeMgr{ *this };		// Archetype Manager
		system::manager				m_SystemMgr{ m_Clock };			// System Manager
		script::manager				m_ScriptMgr{ *this };			// CPP Scripts Manager
		Input						m_Input{ *this };				// Input
		bool						m_GameActive = true;			// Game Status
		std::string					m_QueuedSceneName = "";			// Currently Queued Scene to change
	};
}

static struct engine
{
	paperback::coordinator::instance& m_Coordinator = paperback::coordinator::instance::GetInstance();
	int m_Width { 0 };
	int m_Height { 0 };
	std::string m_WinName ;

} m_Engine;

#define PPB m_Engine.m_Coordinator

namespace RR_ENGINE
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<engine>("Engine Config")
			.constructor()( rttr::policy::ctor::as_object )
			.property("Window", &engine::m_WinName)
			.property("Width", &engine::m_Width)
			.property("Height", &engine::m_Height);
	}
}