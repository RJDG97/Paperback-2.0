#pragma once
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

			if (IsKeyPressDown(GLFW_KEY_ESCAPE))
				m_GameActive = false;

			XCORE_PERF_FRAME_MARK_END( "paperback::frame" )
		}
	}

	void instance::Terminate( void ) noexcept
	{
		m_SystemMgr.Terminate();
		m_ArchetypeMgr.Terminate();
		m_CompMgr.Terminate();
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


	//-----------------------------------
	//           Save Scene
	//-----------------------------------
	PPB_INLINE
	void instance::SaveScene(const std::string& FilePath) noexcept
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

		// Serialize Prefabs 
		for (auto& Archetype : PPB.GetArchetypeList())
		{
			if ( Archetype->GetComponentBits().Has(paperback::component::info_v<prefab>.m_UID) ) //Have prefab component
			{
				JFile.StartObject().WriteKey(("Prefab " + Archetype->GetName()).c_str()).StartArray();

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
			else
			{
				// Serialize Prefabs Instances + Normal Entities
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
		SaveEntityInfo("../../resources/assetloading/EntityInfoTest.json");
	}

	PPB_INLINE
	void instance::SaveEntityInfo( const std::string& FilePath) noexcept
	{
		paperback::entity::TempInfo Tempinfo{};
		// Probably just save into the same file everytime
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
	void instance::OpenScene( const std::string& FilePath ) noexcept
	{
		JsonFile Jfile;

		Jfile.StartReader(FilePath);
		Jfile.LoadEntities("Entities");
		Jfile.EndReader();

		// After loading in all Entities, load in the Entity Infos
		//LoadEntityInfo( "../../resources/assetloading/EntityInfoTest.json" );
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
	void instance::SaveSingleEntity( const std::string& FilePath, const paperback::entity::info& EntityInfo ) noexcept
	{
		paperback::entity::TempInfo Tempinfo = {};
		paperback::component::temp_guid Temp = {};
		
		paperback::JsonFile JFile;

		JFile.StartWriter(FilePath);

		/*&*/ JFile.StartObject().WriteKey(EntityInfo.m_pArchetype->GetName().c_str()).StartArray();

		/*%*/ JFile.StartObject().WriteKey("Guid").StartArray();

		auto& EntityComponentInfo = EntityInfo.m_pArchetype->GetComponentInfos();

		for (paperback::u32 i = 0; i < EntityInfo.m_pArchetype->GetComponentCount(); ++i)
		{
			Temp.m_Value = EntityComponentInfo[i]->m_Guid.m_Value;
			JFile.WriteGuid(Temp);
		}

		//auto& Components = EntityInfo.m_pArchetype->GetEntityComponents();

		//if ()

		/*%*/ JFile.EndArray().EndObject();

		/*&*/ JFile.EndArray().EndObject().EndWriter();

	}

	//-----------------------------------
	//       Entity / Archetype
	//-----------------------------------
	template < typename... T_COMPONENTS >
	archetype::instance& instance::GetOrCreateArchetype( void ) noexcept
	{
		return m_ArchetypeMgr.GetOrCreateArchetype<T_COMPONENTS...>( );
	}

	archetype::instance& instance::GetOrCreateArchetype( const tools::bits ArchetypeSignature ) noexcept
	{
		return m_ArchetypeMgr.GetOrCreateArchetype( ArchetypeSignature );
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


	//-----------------------------------
	//             Getters
	//-----------------------------------
	entity::info& instance::GetEntityInfo( component::entity& Entity ) const noexcept
	{
		return m_ArchetypeMgr.GetEntityInfo( Entity );
	}

	entity::info& instance::GetEntityInfo( const u32 GlobalIndex ) const noexcept
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
		m_Input.UpateInputs();
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