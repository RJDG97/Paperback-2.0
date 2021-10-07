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
		JsonFile Jfile;

		Jfile.StartWriter(FilePath);
		Jfile.StartObject().WriteKey("Entities");
		Jfile.StartArray();

		for (auto& Archetype : PPB.GetArchetypeList())
		{
			Jfile.StartObject().WriteKey(Archetype->GetName());
			Jfile.StartArray();

			component::temp_guid Temp = {};

			Jfile.StartObject().WriteKey("Guid").StartArray();
			auto& ComponentInfoArray = Archetype->GetComponentInfos();

			for (u32 i = 0; i < Archetype->GetComponentNumber(); ++i)
			{
				Temp.m_Value = ComponentInfoArray[i]->m_Guid.m_Value;
				Jfile.WriteGuid(Temp);
			}
			Jfile.EndArray();
			Jfile.EndObject();

			Archetype->SerializeAllEntities(Jfile);



			Jfile.EndArray();
			Jfile.EndObject();
		}

		Jfile.EndArray();
		Jfile.EndObject();
		Jfile.EndWriter();
	}

	PPB_INLINE
	void instance::OpenScene( const std::string& FilePath ) noexcept
	{
		JsonFile Jfile;
		Jfile.StartReader(FilePath);
		Jfile.LoadEntities("Entities");
		Jfile.EndReader();
	}

	//-----------------------------------
	//       Entity / Archetype
	//-----------------------------------
	template < typename... T_COMPONENTS >
	archetype::instance& instance::GetOrCreateArchetype( void ) noexcept
	{
		return m_ArchetypeMgr.GetOrCreateArchetype<T_COMPONENTS...>( *this );
	}

	archetype::instance& instance::GetOrCreateArchetype( const tools::bits ArchetypeSignature ) noexcept
	{
		return m_ArchetypeMgr.GetOrCreateArchetype( ArchetypeSignature );
	}

	archetype::instance& instance::CreateArchetype( const tools::bits& Signature ) noexcept // Maybe remove
	{
		return m_ArchetypeMgr.CreateArchetype( Signature );
	}

	template< typename T_FUNCTION >
	void instance::CreateEntity( T_FUNCTION&& Function ) noexcept
	{
		m_ArchetypeMgr.CreateEntity( Function, *this );
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

	void instance::RemoveEntity( const uint32_t SwappedGlobalIndex, const component::entity Entity ) noexcept
	{
		m_ArchetypeMgr.RemoveEntity( SwappedGlobalIndex, Entity );
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
	void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept
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
										  pComponent += sizeof(std::decay_t<T_C>);
				
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
	void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept
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
								pComponent += sizeof(std::decay_t<T_C>);

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

	void instance::RegisterEntity( const paperback::vm::PoolDetails Details, paperback::archetype::instance& Archetype ) noexcept
	{
		m_ArchetypeMgr.RegisterEntity( Details, Archetype );
	}
}