#pragma once
#include "../Functionality/RenderResource/RenderResourceLoader.h"
	
namespace paperback::coordinator
{
	//-----------------------------------
	//            Default
	//-----------------------------------

	instance::instance( void ) noexcept
	{
		paperback::logger::Init();

		m_CompMgr.RegisterComponent<paperback::component::entity>();
	}

	instance::~instance( void ) noexcept
	{
		
	}

	instance& instance::GetInstance( void ) noexcept
	{
		static instance Instance;
		return Instance;
	}


	void instance::Initialize( void ) noexcept
	{
		m_ArchetypeMgr.Initialize();

		INFO_PRINT( "Initialized Engine" );
	}

	void instance::Update( void ) noexcept
	{
		while ( m_GameActive )
		{
			XCORE_PERF_FRAME_MARK()
			XCORE_PERF_FRAME_MARK_START( "paperback::frame" )

			m_SystemMgr.Run();

			/*if (IsKeyPressDown(GLFW_KEY_ESCAPE))
				m_GameActive = false;*/

			XCORE_PERF_FRAME_MARK_END( "paperback::frame" )
		}
	}

	void instance::Terminate( void ) noexcept
	{
		m_SystemMgr.Terminate();
		m_ArchetypeMgr.Terminate();
		m_CompMgr.Terminate();
		m_Input.Terminate();
	}

	void instance::QuitApplication(void) noexcept
	{
		m_GameActive = false;
	}

	//-----------------------------------
	//          Registration
	//-----------------------------------

	template < concepts::System... T_SYSTEMS >
	constexpr void instance::RegisterSystems( void ) noexcept
	{
		m_SystemMgr.RegisterSystems<T_SYSTEMS...>( *this );
	}

	template< typename... T_COMPONENTS >
	constexpr void instance::RegisterComponents( void ) noexcept
	{
		m_CompMgr.RegisterComponents<T_COMPONENTS...>();
	}

	template < typename T_EVENT >
    void instance::RegisterEvent( void ) noexcept
	{
		m_GlobalEventMgr.RegisterEvent<T_EVENT>( );
	}

    template < paperback::concepts::Event T_EVENT
             , typename                   T_CLASS >
    void instance::RegisterEventClass( T_CLASS* Class ) noexcept
	{
		m_GlobalEventMgr.RegisterEventClass<T_EVENT>( static_cast<T_CLASS*>( Class ) );
	}


    //-----------------------------------
    //             Removal
    //-----------------------------------

    template < paperback::concepts::Event T_EVENT >
    void instance::RemoveEvent( void ) noexcept
	{
		m_GlobalEventMgr.RemoveEvent<T_EVENT>( );
	}

    template < paperback::concepts::Event T_EVENT
             , typename                   T_CLASS >
    void instance::RemoveEventClass( T_CLASS* Class ) noexcept
	{
		m_GlobalEventMgr.RemoveEventClass<T_EVENT>( static_cast<T_CLASS*>( Class ) );
	}


	//-----------------------------------
	//           Save Scene
	//-----------------------------------

	PPB_INLINE
	void instance::SaveScene( const std::string& FilePath, const std::string& EntityInfoPath ) noexcept
	{
		paperback::component::temp_guid Temp = {};
		paperback::archetype::TempMgr TempMgr;
		
		paperback::JsonFile JFile;

		JFile.StartWriter(FilePath);
		JFile.StartObject().WriteKey("Entities");
		JFile.StartArray();

		//Save Archetype Manager
		TempMgr.EntityHead = m_ArchetypeMgr.GetEntityHead();

		JFile.StartObject().WriteKey("Archetype Manager");

		JFile.StartObject().Write(TempMgr).EndObject();

		JFile.EndObject();

		// Serialize Prefabs Instances + Normal Entities
		for (auto& Archetype : PPB.GetArchetypeList())
		{
			if (Archetype->GetCurrentEntityCount() == 0)
				continue;
			else
			{
				JFile.StartObject().WriteKey(Archetype->GetName()).StartArray();

				//Serialize GUIDs

				JFile.StartObject().WriteKey("Guid").StartArray();

				auto& ComponentInfoArray = Archetype->GetComponentInfos();

				for (paperback::u32 i = 0; i < Archetype->GetComponentCount(); ++i)
				{
					Temp.m_Value = ComponentInfoArray[i]->m_Guid.m_Value;
					JFile.WriteGuid(Temp);
				}

				JFile.EndArray().EndObject();

				//Serialize Components

				Archetype->SerializeAllEntities(JFile);
				JFile.EndArray().EndObject();
			}
		}

		JFile.EndArray().EndObject().EndWriter();

		// Save the entity info after everything else
		SaveEntityInfo(EntityInfoPath);
	}

	PPB_INLINE
	void instance::SavePrefabs( const std::string& FilePath ) noexcept
	{
		paperback::component::temp_guid Temp = {};
		paperback::archetype::TempMgr TempMgr;

		paperback::JsonFile JFile;

		JFile.StartWriter(FilePath);
		JFile.StartObject().WriteKey("Prefabs");
		JFile.StartArray();

		// Serialize Prefabs 
		for (auto& Archetype : PPB.GetArchetypeList())
		{
			if (!Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID))
				continue;

			if (Archetype->GetCurrentEntityCount() == 0)
				continue;

			JFile.StartObject().WriteKey(Archetype->GetName()).StartArray();

			//Serialize GUIDs

			JFile.StartObject().WriteKey("Guid").StartArray();

			auto& ComponentInfoArray = Archetype->GetComponentInfos();

			for (paperback::u32 i = 0; i < Archetype->GetComponentCount(); ++i)
			{
				Temp.m_Value = ComponentInfoArray[i]->m_Guid.m_Value;
				JFile.WriteGuid(Temp);
			}

			JFile.EndArray().EndObject();


			for (paperback::u32 i = 0; i < Archetype->GetCurrentEntityCount(); ++i)
			{
				auto& Prefab = Archetype->GetComponent<prefab>(paperback::vm::PoolDetails{ 0, i });

				if (Prefab.m_ReferencePrefabGIDs.size())
					Prefab.m_ReferencePrefabGIDs.clear();
			}

			//Serialize Components

			Archetype->SerializeAllEntities(JFile);
			JFile.EndArray().EndObject();
		}

		JFile.EndArray().EndObject().EndWriter();
	}

	PPB_INLINE
	void instance::SaveIndividualPrefab( const std::string& FilePath, paperback::archetype::instance* PrefabArchetype, const paperback::u32 Index ) noexcept
	{
		paperback::component::temp_guid Temp = {};
		paperback::archetype::TempMgr TempMgr;

		paperback::JsonFile JFile;

		JFile.StartWriter(FilePath);
		JFile.StartObject().WriteKey("Prefabs");
		JFile.StartArray();

		JFile.StartObject().WriteKey(PrefabArchetype->GetName()).StartArray();

		//Serialize GUIDs

		JFile.StartObject().WriteKey("Guid").StartArray();

		auto& ComponentInfoArray = PrefabArchetype->GetComponentInfos();

		for (paperback::u32 i = 0; i < PrefabArchetype->GetComponentCount(); ++i)
		{
			Temp.m_Value = ComponentInfoArray[i]->m_Guid.m_Value;
			JFile.WriteGuid(Temp);
		}

		JFile.EndArray().EndObject();

		auto& Prefab = PrefabArchetype->GetComponent<prefab>(paperback::vm::PoolDetails{ 0, Index });

		if (Prefab.m_ReferencePrefabGIDs.size())
			Prefab.m_ReferencePrefabGIDs.clear();

		//Serialize Components

		PrefabArchetype->SerializePrefabEntity(JFile, Index);
		JFile.EndArray().EndObject();

		JFile.EndArray().EndObject().EndWriter();
	}


	PPB_INLINE
	void instance::SaveEntityInfo( const std::string& FilePath ) noexcept
	{
		paperback::entity::TempInfo Tempinfo{};
		paperback::JsonFile JFile;

		JFile.StartWriter( FilePath ).StartObject().WriteKey( "All Entity Info" ).StartArray();

		const auto& EntityInfoList = m_ArchetypeMgr.GetEntityInfoList();

		JFile.StartObject();

		for (u32 i = 0; i < settings::max_entities_v; ++i)
		{
			Tempinfo.ArchetypeGuid = EntityInfoList[i].m_pArchetype ? EntityInfoList[i].m_pArchetype->GetArchetypeGuid().m_Value : 0;
			Tempinfo.PoolDetails = EntityInfoList[i].m_PoolDetails;
			Tempinfo.Validation = EntityInfoList[i].m_Validation;

			JFile.WriteKey("Entity Info").StartObject().Write(Tempinfo).EndObject();
		}

		JFile.EndObject();

		JFile.EndArray().EndObject().EndWriter();
	}

	PPB_INLINE
	void instance::LoadPrefabs( const std::string& FilePath ) noexcept
	{
		JsonFile Jfile;
		Jfile.StartReader(FilePath);
		Jfile.LoadEntities("Prefabs");
		Jfile.EndReader();
	}

	PPB_INLINE
	void instance::OpenScene( const std::string& SceneName = "" ) noexcept
	{
		// Reset AABB Tree
		m_AABBTree.Reset();

		if (SceneName == "")
		{
			//if no arg given then "launching" so just reload scene manager
			m_SceneMgr.ReloadScene();
		}
		else
		{
			//arg given, changing state
			if (m_SceneMgr.TriggerChangeScene(SceneName))
			{
				//scene exists, load scene
				m_SceneMgr.ChangeScene();
			}
		}

		// Initialize AABB Tree
		m_AABBTree.Initialize();
	}

	PPB_INLINE
	void instance::QueueScene(const std::string& SceneName) noexcept 
	{
		if (m_QueuedSceneName == "")
			m_QueuedSceneName = SceneName;
	}

	PPB_INLINE
	void instance::OpenQueuedScene() noexcept
	{
		if (m_QueuedSceneName != "")
		{

			std::string temp = m_QueuedSceneName;
			m_QueuedSceneName = "";

			OpenScene(temp);
		}
	}

	PPB_INLINE
	void instance::LoadEntityInfo(const std::string& FilePath) noexcept
	{
		JsonFile Jfile;

		Jfile.StartReader(FilePath);
		Jfile.LoadEntities("All Entity Info");
		Jfile.EndReader();
	}

	PPB_INLINE
	void instance::LoadTextures( const std::string& FilePath ) noexcept
	{
		RenderResourceLoader::GetInstanced().ReadTextureJson(FilePath);
	}

	PPB_INLINE
	void instance::OpenEditScene( const std::string& FilePath, const std::string& EntityInfoPath ) noexcept
	{
		m_SceneMgr.UpdateScene(FilePath, EntityInfoPath);
		m_SceneMgr.ReloadScene();
	}

	PPB_INLINE
	bool instance::VerifyState( const std::string& StateName ) noexcept
	{
		return m_SceneMgr.VerifyScene( StateName );
	}

	PPB_INLINE
	void instance::ResetSystems() noexcept
	{
		m_SystemMgr.ResetSystems();
	}

	PPB_INLINE
	void instance::ReloadSystems() noexcept
	{
		m_SystemMgr.ReloadSystems();
	}


	//-----------------------------------
	//       Entity / Archetype
	//-----------------------------------

	template < typename... T_COMPONENTS >
	archetype::instance& instance::GetOrCreateArchetype( void ) noexcept
	{
		return m_ArchetypeMgr.GetOrCreateArchetype<T_COMPONENTS...>( );
	}

	archetype::instance& instance::GetOrCreateArchetype( const tools::bits ArchetypeSignature
													   , std::string ArchetypeName ) noexcept
	{
		return m_ArchetypeMgr.GetOrCreateArchetype( ArchetypeSignature, ArchetypeName );
	}

	void instance::CreatePrefab( void ) noexcept
	{
		m_ArchetypeMgr.CreatePrefab();
	}

	template< typename T_FUNCTION >
	void instance::CreateEntity( T_FUNCTION&& Function ) noexcept
	{
		m_ArchetypeMgr.CreateEntity( Function );
	}

	template< typename T_FUNCTION >
	void instance::CreateEntities( T_FUNCTION&& Function, const u32 Count ) noexcept
	{
		
	}

	void instance::DeleteEntity( component::entity& Entity ) noexcept
	{
		assert( Entity.IsZombie() == false );
		auto& Info = GetEntityInfo( Entity );
		assert( Info.m_Validation == Entity.m_Validation );
		Info.m_pArchetype->DestroyEntity( Entity );
	}

	void instance::ResetAllArchetypes( void ) noexcept
	{
		m_ArchetypeMgr.ResetAllArchetypes();
	}

	template < typename T_COMPONENT >
	void instance::UpdatePrefabInstancesOnPrefabComponentUpdate( const entity::info& PrefabInfo
															   , const T_COMPONENT&  UpdatedComponent ) noexcept
	{
		m_ArchetypeMgr.UpdatePrefabInstancesOnPrefabComponentUpdate(PrefabInfo, UpdatedComponent);
	}




	//-----------------------------------
	//      Add Remove Components
	//-----------------------------------

	template < concepts::TupleSpecialization T_TUPLE_ADD
			 , concepts::TupleSpecialization T_TUPLE_REMOVE
			 , concepts::Callable T_FUNCTION >
	component::entity instance::AddOrRemoveComponents( const component::entity Entity
													 , T_FUNCTION&& Function ) noexcept
	{
        return m_ArchetypeMgr.AddOrRemoveComponents( Entity
												   , component::sorted_info_array_v<T_TUPLE_ADD>
												   , component::sorted_info_array_v<T_TUPLE_REMOVE>
												   , Function );
	}

	template < concepts::Callable T_FUNCTION >
	component::entity instance::AddOrRemoveComponents( const component::entity Entity
								                     , std::span< const component::info* > Add
								                     , std::span< const component::info* > Remove
								                     , T_FUNCTION&& Function ) noexcept
	{
		return m_ArchetypeMgr.AddOrRemoveComponents( Entity
												   , Add
												   , Remove
												   , Function );
	}


	//-----------------------------------
	//             Query
	//-----------------------------------

	template < typename... T_COMPONENTS >
	std::vector<archetype::instance*> instance::Search() const noexcept
	{
		return m_ArchetypeMgr.Search<T_COMPONENTS...>();
	}

	archetype::instance* instance::Search( const tools::bits& Bits ) const noexcept
	{
		return m_ArchetypeMgr.Search( Bits );
	}
	
	std::vector<archetype::instance*> instance::Search( const tools::query& Query ) const noexcept
	{
		return m_ArchetypeMgr.Search( Query );
	}


	//-----------------------------------
	//            Game Loop
	//-----------------------------------

	template < concepts::Callable_Void T_FUNCTION>
	void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList
						  , T_FUNCTION&& Function ) noexcept
	{
		using func_traits = xcore::function::traits<T_FUNCTION>;

		for ( const auto& Archetype : ArchetypeList )
		{
			for ( auto& Pool : Archetype->GetComponentPools() )
			{
				auto ComponentArray = Archetype->GetComponentArray( Pool, 0, xcore::types::null_tuple_v< func_traits::args_tuple > );

				Archetype->AccessGuard( [&]
				{
					for (int i = Pool.GetCurrentEntityCount(); i; --i)
					{
						[&]< typename... T_COMPONENTS >( std::tuple<T_COMPONENTS...>* ) constexpr noexcept
						{
							Function( [&]<typename T_C>( std::tuple<T_C>* ) constexpr noexcept -> T_C
									  {
										  auto& pComponent = ComponentArray[xcore::types::tuple_t2i_v< T_C, typename func_traits::args_tuple >];
				
										  if constexpr (std::is_pointer_v<T_C>) if (pComponent == nullptr) return reinterpret_cast<T_C>(nullptr);
				
										  auto p = pComponent;
										  pComponent += sizeof(paperback::BaseType<T_C>);
				
										  if constexpr (std::is_pointer_v<T_C>)		return reinterpret_cast<T_C>(p);
										  else										return reinterpret_cast<T_C>(*p);
				
									  }( xcore::types::make_null_tuple_v<T_COMPONENTS> )
							... );
						}( xcore::types::null_tuple_v< func_traits::args_tuple > );
					}
				});
			}
		}
	}

	template < concepts::Callable_Void T_FUNCTION>
	void instance::ForEach( const std::vector<paperback::u32>& NeighbourList
						  , T_FUNCTION&& Function ) noexcept
	{
		using func_traits = xcore::function::traits<T_FUNCTION>;

		for ( const auto GID : NeighbourList )
        {
			tools::query FuncQuery;							  // Query For Archetype
            const auto&  Info = GetEntityInfo( GID );		  // Get Entity Info

			FuncQuery.AddQueryFromFunction<T_FUNCTION>();     // Assign Query

            if ( Info.m_pArchetype && Info.m_pArchetype->GetComponentBits().Compare( FuncQuery.m_Must )
				                   && Info.m_pArchetype->GetComponentBits().OneOf( FuncQuery.m_OneOf ) )
            {
				Info.m_pArchetype->AccessGuard( [&]
				{
					[&]< typename... T_COMPONENTS >( std::tuple<T_COMPONENTS...>* ) constexpr noexcept
					{
						Function( [&]<typename T_C>( std::tuple<T_C>* ) constexpr noexcept -> T_C
								  {
									  auto pComponent = Info.m_pArchetype->FindComponent( Info.m_PoolDetails, component::info_v<T_C>.m_Guid );
									  if constexpr (std::is_pointer_v<T_C>)
									  {
										  if ( pComponent ) return reinterpret_cast<T_C>( pComponent );
										  else return reinterpret_cast<T_C>( nullptr );
									  }
									  else return reinterpret_cast<T_C>( *pComponent );
				
								  }( xcore::types::make_null_tuple_v<T_COMPONENTS> )
						... );
					}( xcore::types::null_tuple_v< func_traits::args_tuple > );
					
				});
			}
		}
	}

	template < concepts::Callable_Bool T_FUNCTION >
	void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList
						  , T_FUNCTION&& Function ) noexcept
	{
		using func_traits = xcore::function::traits< T_FUNCTION >;

		for ( const auto& Archetype : ArchetypeList )
		{
			bool bBreak = false;

			for ( auto& Pool : Archetype->GetComponentPools() )
			{
				auto ComponentArray = Archetype->GetComponentArray( Pool, 0, xcore::types::null_tuple_v< func_traits::args_tuple > );

				Archetype->AccessGuard( [&]
				{
					for (int i = Pool.GetCurrentEntityCount(); i; --i)
					{
						if ([&]<typename... T_COMPONENTS>(std::tuple<T_COMPONENTS...>*) constexpr noexcept
						{
							return Function([&]<typename T_C>(std::tuple<T_C>*) constexpr noexcept -> T_C
							{
								auto& pComponent = ComponentArray[ xcore::types::tuple_t2i_v< T_C, typename func_traits::args_tuple > ];

								if constexpr (std::is_pointer_v<T_C>) if (pComponent == nullptr) return reinterpret_cast<T_C>(nullptr);

								auto p = pComponent;
								pComponent += sizeof(paperback::BaseType<T_C>);

								if constexpr (std::is_pointer_v<T_C>)		return reinterpret_cast<T_C>(p);
								else										return reinterpret_cast<T_C>(*p);

							}(xcore::types::make_null_tuple_v<T_COMPONENTS>)
							...);
						}(xcore::types::null_tuple_v< func_traits::args_tuple >))
						{
							bBreak = true;
							break;
						}
					}
				});

				if ( bBreak ) break;
			}
			if (bBreak) break;
		}
	}

	template < concepts::Callable_Bool T_FUNCTION>
	void instance::ForEach( const std::vector<paperback::u32>& NeighbourList
						  , T_FUNCTION&& Function ) noexcept
	{
		using func_traits = xcore::function::traits<T_FUNCTION>;

		bool bBreak = false;

		for ( const auto GID : NeighbourList )
        {
			tools::query FuncQuery;							  // Query For Archetype
            const auto&  Info = GetEntityInfo( GID );		  // Get Entity Info

			FuncQuery.AddQueryFromFunction<T_FUNCTION>();     // Assign Query

            if ( Info.m_pArchetype && Info.m_pArchetype->GetComponentBits().Compare( FuncQuery.m_Must )
				                   && Info.m_pArchetype->GetComponentBits().OneOf( FuncQuery.m_OneOf ) )
            {
				Info.m_pArchetype->AccessGuard( [&]
				{
					if ( [&]< typename... T_COMPONENTS >( std::tuple<T_COMPONENTS...>* ) constexpr noexcept
					     {
					     	  return Function( [&]<typename T_C>( std::tuple<T_C>* ) constexpr noexcept -> T_C
					     	  		    {
					     	  		 	     auto pComponent = Info.m_pArchetype->FindComponent( Info.m_PoolDetails, component::info_v<T_C>.m_Guid );
					     	  		 	     if constexpr (std::is_pointer_v<T_C>)
					     	  		 	     {
					     	  		 	 	    if ( pComponent ) return reinterpret_cast<T_C>( pComponent );
					     	  		 	 	    else return reinterpret_cast<T_C>( nullptr );
					     	  		 	     }
					     	  		 	     else return reinterpret_cast<T_C>( *pComponent );
					     	  		    
					     	  		    }( xcore::types::make_null_tuple_v<T_COMPONENTS> )
					     	  ... );
					     }( xcore::types::null_tuple_v< func_traits::args_tuple > ))
					{
						bBreak = true;
					}
				});
			}
			if ( bBreak ) break;
		}
	}

	void instance::ToggleDebug( const bool& Status ) noexcept
	{
		m_SystemMgr.ToggleDebug( Status );
	}

	void instance::TogglePause(const bool& Status) noexcept
	{

		m_bPaused = Status;

		ToggleCursor(Status);

		m_SystemMgr.TogglePause(Status);
	}

	void instance::ToggleCursor(const bool& Status) noexcept
	{

		if (Status == m_bCursorActive)
			return;

		auto WindowsSystem = FindSystem<window_system>();
		m_bCursorActive = Status;

		if (WindowsSystem)
		{

			if (Status)
			{

				glfwSetInputMode(WindowsSystem->m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else
			{

				glfwSetInputMode(WindowsSystem->m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}


	void instance::TogglePlayers(void) noexcept
	{
		//no longer handled here
		// 
		//tools::query m_QueryPlayer;
		//m_QueryPlayer.m_Must.AddFromComponents<camera, player_controller>();
		//m_QueryPlayer.m_NoneOf.AddFromComponents<prefab>();

		//ForEach(Search(m_QueryPlayer), [&](camera& Camera, player_controller& Player_Controller) noexcept
		//{
			//Camera.m_Active = !Camera.m_Active;
			//Player_Controller.m_PlayerStatus = !Player_Controller.m_PlayerStatus;
		//});
	}

	void instance::QuitGame() noexcept
	{
		m_GameActive = false;
	}


	//-----------------------------------
	//             Getters
	//-----------------------------------

	paperback::entity::info& instance::GetEntityInfo( component::entity& Entity ) const noexcept
	{
		return m_ArchetypeMgr.GetEntityInfo( Entity );
	}

	paperback::entity::info& instance::GetEntityInfo( const u32 GlobalIndex ) const noexcept
	{
		return m_ArchetypeMgr.GetEntityInfo( GlobalIndex );
	}

	template< typename T_SYSTEM >
	T_SYSTEM* instance::FindSystem( void ) noexcept
	{
		return m_SystemMgr.FindSystem<T_SYSTEM>();
	}

	template< typename T_SYSTEM >
	T_SYSTEM& instance::GetSystem( void ) noexcept
	{
		auto p = m_SystemMgr.FindSystem<T_SYSTEM>();
		assert( p );
		return *p;
	}

	const paperback::component::info* instance::FindComponentInfoFromUID( const u32 ComponentUID ) noexcept
	{
		return m_CompMgr.FindComponentInfoFromUID( ComponentUID );
	}

	std::vector<paperback::archetype::instance*> instance::GetArchetypeList( void ) noexcept
	{
		return m_ArchetypeMgr.GetArchetypeList();
	}

    const paperback::component::info* instance::FindComponentInfo( const paperback::component::type::guid ComponentGuid ) noexcept
	{
		return m_CompMgr.FindComponentInfo( ComponentGuid );
	}

	paperback::component::manager::ComponentInfoMap& instance::GetComponentInfoMap() noexcept
	{
		return m_CompMgr.GetComponentInfoMap();
	}

	std::vector<fs::path>& instance::GetDragDropFiles() noexcept
	{
		return m_DragDropFiles;
	}

	void instance::SetEntityHead( u32 NewEntityHead ) noexcept
	{
		m_ArchetypeMgr.SetEntityHead(NewEntityHead);
	}

	paperback::archetype::manager::EntityInfoList& instance::GetEntityInfoList() noexcept
	{
		return m_ArchetypeMgr.GetEntityInfoList();
	}

	paperback::archetype::instance& instance::GetArchetype( const u64 ArchetypeGuid ) noexcept
	{
		return m_ArchetypeMgr.GetArchetype( ArchetypeGuid );
	}

	bool instance::GetPauseBool() noexcept
	{
		return m_bPaused;
	}

	
	GLFWwindow* instance::GetWindowHandle( void ) noexcept
	{
		auto Win = FindSystem<window_system>();

		if ( Win ) return Win->m_pWindow;
		else       return nullptr;
	}

	PPB_INLINE
	input::device::Keyboard_Controls* instance::FindKeyboard( void ) noexcept
	{
		return m_Input.FindKeyboard();
	}

	PPB_INLINE
	input::device::Mouse_Controls* instance::FindMouse( void ) noexcept
	{
		return m_Input.FindMouse();
	}

	PPB_INLINE
	input::device::Gamepad_Controls* instance::FindGamepad( void ) noexcept
	{
		return m_Input.FindGamepad();
	}


	//-----------------------------------
	//           CPP Scripts
	//-----------------------------------

	template < paperback::concepts::Script... T_SCRIPT >
    void instance::RegisterScripts( void ) noexcept
	{
		m_ScriptMgr.RegisterScripts<T_SCRIPT...>();
	}

    template < paperback::concepts::Script T_SCRIPT >
    T_SCRIPT* instance::FindScript( const paperback::u64 ScriptGuid ) const noexcept
	{
		return m_ScriptMgr.FindScript<T_SCRIPT>( ScriptGuid );
	}

    template < paperback::concepts::Script T_SCRIPT >
	T_SCRIPT& instance::GetScript( const paperback::u64 ScriptGuid ) const noexcept
	{
		return m_ScriptMgr.GetScript<T_SCRIPT>( ScriptGuid );
	}

    PPB_FORCEINLINE
    const script::manager::CPPScriptsList& instance::GetScriptsList( void ) const noexcept
	{
		return m_ScriptMgr.GetScriptsList();
	}


	//-----------------------------------
	//         Query AABB Tree
	//-----------------------------------
	
	PPB_INLINE
    physics::AABB_Tree::NeighbourList instance::QueryNeighbours( const boundingbox& AABB
                                                               , const transform&   Transform ) noexcept
	{
		return m_AABBTree.QueryNeighbours( AABB
										 , Transform );
	}

	PPB_INLINE
	physics::AABB_Tree::NeighbourList instance::QueryRaycast( const paperback::Vector3f& StartRay
												            , const paperback::Vector3f& EndRay ) noexcept
	{
		return m_AABBTree.QueryRaycast( StartRay
									  , EndRay );
	}

	PPB_INLINE
	std::tuple<physics::AABB_Tree::EntityGID, float> instance::QueryRaycastClosest( const paperback::Vector3f&               StartRay
									                                              , const paperback::Vector3f&               EndRay
															                      , std::span<physics::AABB_Tree::EntityGID> ExcludeList ) noexcept
	{
		return m_AABBTree.QueryRaycastClosest( StartRay
			                                 , EndRay
			                                 , ExcludeList );
	}

	
	//-----------------------------------
	//         Update AABB Tree
	//-----------------------------------
	
	PPB_INLINE
    bool instance::UpdateNode( const boundingbox& AABB
                             , const transform&   Transform
                             , component::entity  Entity ) noexcept
	{
		return m_AABBTree.UpdateNode( AABB, Transform, Entity );
	}

	//-----------------------------------
	//             Setters
	//-----------------------------------

	void instance::SetPauseBool( bool Paused ) noexcept
	{
		m_bPaused = Paused;
	}






















	//-----------------------------------
	//              Clock
	//-----------------------------------

	float instance::DeltaTime() const noexcept
	{
		return m_Clock.DeltaTime();
	}

    void instance::SetTimeScale( const float s ) noexcept
	{
		m_Clock.TimeScale( s );
	}

	float instance::GetTimeScale() const noexcept
	{
		return m_Clock.TimeScale();
	}

	auto instance::Now() noexcept -> decltype( std::chrono::high_resolution_clock::now() )
	{
		return m_Clock.Now();
	}

	u32 instance::GetFPS(void) noexcept
	{
		return m_Clock.FPS();
	}

	//-----------------------------------
	//              Input
	//-----------------------------------

	void instance::UpdateInputs() noexcept
	{
		m_Input.UpdateInputs();
	}

	template < typename T_BINDING_CONSTRUCT >
    paperback::u64 instance::RegisterBinding( void ) noexcept
	{
		return m_Input.RegisterBinding<T_BINDING_CONSTRUCT>();
	}

    PPB_INLINE
    void instance::AssignBindingToAction( const input::binding::type::guid&         BindingGuid
                                        , const paperback::u32                      Key
                                        , const input::device::type::id             DeviceTypeID
                                        , const input::action::BroadcastStatus      Status
                                        , const input::action::KeyPairing           Pairing ) noexcept
	{
		AssignBindingToAction( BindingGuid.m_Value
							 , Key
							 , DeviceTypeID
							 , Status
		                     , Pairing );
	}

	PPB_INLINE
    void instance::AssignBindingToAction( const paperback::u64&                     BindingGuidValue
                                        , const paperback::u32                      Key
                                        , const input::device::type::id             DeviceTypeID
                                        , const input::action::BroadcastStatus      Status
                                        , const input::action::KeyPairing           Pairing ) noexcept
	{
		m_Input.AssignBindingToAction( BindingGuidValue
									 , Key
									 , DeviceTypeID
									 , Status
		                             , Pairing );
	}

    template < typename... T_ARGS >
    void instance::BroadcastAction( const paperback::u32                  Key
                                  , const input::device::type::id         Type
                                  , const input::action::BroadcastStatus  Status
                                  , T_ARGS&&...                           Args ) noexcept
	{
		m_Input.BroadcastAction( Key
			                   , Type
			                   , std::forward<T_ARGS&&>( Args )... );
	}

	void instance::SetKey( int Key, int Action ) noexcept
	{
		m_Input.SetKey( Key, Action );
	}

	void instance::SetMouse( int Key, int Action ) noexcept
	{
		m_Input.SetMouse( Key, Action );
	}

	bool instance::IsKeyPress( int Key ) noexcept
	{
		return m_Input.IsKeyPress( Key );
	}

	bool instance::IsKeyPressDown( int Key ) noexcept
	{
		return m_Input.IsKeyPressDown( Key );
	}

	bool instance::IsKeyPressUp( int Key ) noexcept
	{
		return m_Input.IsKeyPressUp( Key );
	}

	bool instance::IsMousePress( int Key ) noexcept
	{
		return m_Input.IsMousePress( Key );
	}

	bool instance::IsMouseDown( int Key ) noexcept
	{
		return m_Input.IsMouseDown( Key );
	}

	bool instance::IsMouseUp( int Key ) noexcept
	{
		return m_Input.IsMouseUp( Key );
	}

	glm::vec2 instance::Get2DMousePosition() noexcept
	{
		return m_Input.Get2DMousePosition();
	}

	glm::vec3 instance::GetMousePosition() noexcept
	{
		return m_Input.GetMousePosition();
	}

	glm::vec2 instance::GetMouseDirection() noexcept
	{
		return m_Input.GetMouseDirection();
	}

	glm::vec3 instance::GetViewportMousePosition(glm::vec2 viewport_min, glm::vec2 viewport_max) noexcept
	{
		return m_Input.GetViewportMousePosition(viewport_min, viewport_max);
	}

	glm::vec3 instance::GetViewportMousePosition(glm::mat4 projection, glm::mat4 view) noexcept
	{
		return m_Input.GetViewportMousePosition(projection, view);
	}


	//-----------------------------------
	//              Input
	//-----------------------------------
	
	PPB_INLINE
    particles::manager::ParticleList instance::RequestParticles( const int            Quantity
                                                               , const paperback::u32 EmitterGID ) noexcept
	{
		return m_ParticleMgr.RequestParticles( Quantity
											 , EmitterGID );
	}

    PPB_INLINE
    void instance::ReturnDeadParticle( const paperback::u32 ParticleGID ) noexcept
	{
		m_ParticleMgr.ReturnDeadParticle( ParticleGID );
	}


	//-----------------------------------
    //         Event Broadcast
    //-----------------------------------

    template < paperback::concepts::Event T_EVENT
             , typename...                T_ARGS >
    void instance::BroadcastEvent( T_ARGS&&... Args ) const noexcept
	{
		m_GlobalEventMgr.BroadcastEvent<T_EVENT>( std::forward< T_ARGS&& >( Args )... );
	}


	//-----------------------------------
	// Protected - Register for Archetype
	//-----------------------------------

	paperback::component::entity& instance::RegisterEntity( const paperback::vm::PoolDetails Details
								 , paperback::archetype::instance& Archetype ) noexcept
	{
		return m_ArchetypeMgr.RegisterEntity( Details, Archetype );
	}

	void instance::RemoveEntity( const u32 SwappedGlobalIndex
							        , const u32 DeletedEntityIndex ) noexcept
	{
		m_ArchetypeMgr.RemoveEntity( SwappedGlobalIndex, DeletedEntityIndex );
	}
}