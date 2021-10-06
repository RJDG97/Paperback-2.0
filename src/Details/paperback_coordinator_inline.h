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
			ERROR_LOG( "FPS: " + std::to_string( m_Clock.FPS() ) );

			XCORE_PERF_FRAME_MARK_END( "paperback::frame" )
		}
	}

	void instance::Terminate( void ) noexcept
	{
		m_SystemMgr.Terminate();
		m_EntityMgr.Terminate();
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
	void instance::SaveScene( const std::string& FilePath ) noexcept
	{
		JsonFile jfile;

		jfile.StartWriter(FilePath);
		jfile.StartObject().WriteKey("Entities");
		jfile.StartArray();

		for (auto& Archetype : m_EntityMgr.m_pArchetypeList)
		{
			//jfile.WriteKey(Archetype); // probably write like entity count and other relevant data for tracking?
			Archetype->SerializeAllEntities(jfile);
		}

		jfile.EndArray();
		jfile.EndObject();
		jfile.EndWriter();
	}


	//-----------------------------------
	//       Entity / Archetype
	//-----------------------------------
	template < typename... T_COMPONENTS >
	archetype::instance& instance::GetOrCreateArchetype( void ) noexcept
	{
		return m_EntityMgr.GetOrCreateArchetype<T_COMPONENTS...>( *this );
	}

	template< typename T_FUNCTION >
	void instance::CreateEntity( T_FUNCTION&& Function ) noexcept
	{
		m_EntityMgr.CreateEntity( Function, *this );
	}

	template< typename T_FUNCTION >
	void instance::CreateEntities( T_FUNCTION&& Function, const u32 Count ) noexcept
	{
		// To define
	}

	void instance::DeleteEntity( component::entity& Entity ) noexcept // Change it to call "archetype manager" -> delete entity ( Handle all checks there instead )
	{
		assert( Entity.IsZombie() == false );
		auto& Info = GetEntityInfo( Entity );
		assert( Info.m_Validation == Entity.m_Validation );
		Info.m_pArchetype->DestroyEntity( Entity );
	}

	void instance::RemoveEntity( const uint32_t SwappedGlobalIndex, const component::entity Entity ) noexcept
	{
		m_EntityMgr.RemoveEntity( SwappedGlobalIndex, Entity );
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
        return m_EntityMgr.AddOrRemoveComponents( Entity
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
		return m_EntityMgr.Search<T_COMPONENTS...>();
	}

	archetype::instance* instance::Search( const tools::bits& Bits ) const noexcept
	{
		return m_EntityMgr.Search( Bits );
	}
	
	std::vector<archetype::instance*> instance::Search( const tools::query& Query ) const noexcept
	{
		return m_EntityMgr.Search( Query );
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
			for ( auto& Pool : Archetype->m_ComponentPool )
			{
				auto ComponentArray = Archetype->GetComponentArray( Pool, 0, xcore::types::null_tuple_v< func_traits::args_tuple > );

				Archetype->AccessGuard( [&]
				{
					for (int i = Pool.m_CurrentEntityCount; i; --i)
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
			//if ( Archetype->m_EntityCount == 0 ) continue;
			bool bBreak = false;

			for ( auto& Pool : Archetype->m_ComponentPool )
			{
				auto ComponentArray = Archetype->GetComponentArray( Pool, 0, xcore::types::null_tuple_v< func_traits::args_tuple > );

				Archetype->AccessGuard( [&]
				{
					for (int i = Pool.m_CurrentEntityCount; i; --i)
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
		return m_EntityMgr.GetEntityInfo( Entity );
	}

	entity::info& instance::GetEntityInfo( const u32 GlobalIndex ) const noexcept
	{
		return m_EntityMgr.GetEntityInfo( GlobalIndex );
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
	
	//-----------------------------------
	//         Helper Function
	//-----------------------------------
	void instance::FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept
	{
		m_EntityMgr.FreeEntitiesInArchetype( Archetype );
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
}