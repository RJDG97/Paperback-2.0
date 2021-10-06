#pragma once

namespace paperback::system
{
	//-----------------------------------
	//    System Info Initialization
	//-----------------------------------
	namespace details
    {
        template< typename T_SYSTEM >
        consteval system::type::info CreateInfo( void ) noexcept
        {
            return system::type::info
            {
                .m_Guid          = T_SYSTEM::typedef_v.m_Guid.isValid()
                                   ? T_SYSTEM::typedef_v.m_Guid
                                   : type::guid{ __FUNCSIG__ }
            ,   .m_TypeID        = T_SYSTEM::typedef_v.id_v
			,	.m_RunSystem   = []( system::instance& pSystem, system::type::call UpdateType )
							     {
								     static_cast<system::details::completed<T_SYSTEM>&>( pSystem ).Run( UpdateType );
							     }
			,	.m_pName		   = T_SYSTEM::typedef_v.m_pName
            };
        }
	}


	//-----------------------------------
	//        System Instance
	//-----------------------------------
	instance::instance( coordinator::instance& Coordinator ) noexcept : 
		m_Coordinator{ Coordinator }
	{ }
	

	//-----------------------------------
	//         System Details
	//-----------------------------------
	template < typename USER_SYSTEM >
	details::completed<USER_SYSTEM>::completed( coordinator::instance& Coordinator ) noexcept :
		USER_SYSTEM{ Coordinator }
	{ }
	
	template < typename USER_SYSTEM >
	void details::completed<USER_SYSTEM>::Run( const paperback::system::type::call Type ) noexcept
	{
		XCORE_PERF_ZONE_SCOPED_N( USER_SYSTEM::typedef_v.m_pName )

		switch ( Type )
		{
			case system::type::call::CREATED:
			{
				if constexpr ( &USER_SYSTEM::OnSystemCreated != &system_interface::OnSystemCreated )
				{
					XCORE_PERF_ZONE_SCOPED_N( "System Created" )
					USER_SYSTEM::OnSystemCreated();
				}
				break;
			}
			case system::type::call::FRAME_START:
			{
				if constexpr ( &USER_SYSTEM::OnFrameStart != &system_interface::OnFrameStart )
				{
					XCORE_PERF_ZONE_SCOPED_N( "Frame Start" )
					USER_SYSTEM::OnFrameStart();
				}
				break;
			}
			case system::type::call::PRE_UPDATE:
			{
				if constexpr ( &USER_SYSTEM::PreUpdate != &system_interface::PreUpdate )
				{
					XCORE_PERF_ZONE_SCOPED_N( "Pre Update" )
					USER_SYSTEM::PreUpdate();
				}
				break;
			}
			case system::type::call::UPDATE:
			{
				if constexpr ( &USER_SYSTEM::Update != &system_interface::Update )
				{
					XCORE_PERF_ZONE_SCOPED_N( "Update" )
					USER_SYSTEM::Update();
				}
				else
				{
					XCORE_PERF_ZONE_SCOPED_N( "Update" )
					tools::query Query;
					Query.AddQueryFromTuple( reinterpret_cast<typename USER_SYSTEM::query*>(nullptr) );
					Query.AddQueryFromFunction( *this );
					USER_SYSTEM::m_Coordinator.ForEach( USER_SYSTEM::m_Coordinator.Search(Query), *this );
				}
				break;
			}
			case system::type::call::POST_UPDATE:
			{
				if constexpr ( &USER_SYSTEM::PostUpdate != &system_interface::PostUpdate )
				{
					XCORE_PERF_ZONE_SCOPED_N( "Post Update" )
					USER_SYSTEM::PostUpdate();
				}
				break;
			}
			case system::type::call::FRAME_END:
			{
				if constexpr ( &USER_SYSTEM::OnFrameEnd != &system_interface::OnFrameEnd )
				{
					XCORE_PERF_ZONE_SCOPED_N( "Frame End" )
					USER_SYSTEM::OnFrameEnd();
				}
				break;
			}
			case system::type::call::TERMINATED:
			{
				if constexpr ( &USER_SYSTEM::OnSystemTerminated != &system_interface::OnSystemTerminated )
				{
					XCORE_PERF_ZONE_SCOPED_N( "Terminated" )
					USER_SYSTEM::OnSystemTerminated();
				}
				break;
			}
		}
	}


	//-----------------------------------
	//   System Instance Functionality
	//-----------------------------------
	template < typename... T_COMPONENTS >
	archetype::instance& instance::GetOrCreateArchetype( void ) noexcept
	{
		return m_Coordinator.GetOrCreateArchetype<T_COMPONENTS...>();
	}

	template< typename T_FUNCTION >
	void instance::CreateEntity( T_FUNCTION&& Function ) noexcept
	{
		m_Coordinator.CreateEntity( Function );
	}

	template< typename T_FUNCTION >
	void instance::CreateEntities( T_FUNCTION&& Function
					   , const u32 Count ) noexcept
	{
		m_Coordinator.CreateEntities( Function, Count );
	}

	void instance::DeleteEntity( component::entity& Entity ) noexcept
	{
		m_Coordinator.DeleteEntity( Entity );
	}

	template < typename... T_COMPONENTS >
    std::vector<archetype::instance*> instance::Search() const noexcept
	{
		return m_Coordinator.Search<T_COMPONENTS...>();
	}

    std::vector<archetype::instance*> instance::Search( const tools::query& Query ) const noexcept
	{
		return m_Coordinator.Search( Query );
	}

	template < concepts::Callable_Void T_FUNCTION>
    void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList
						  , T_FUNCTION&& Function ) noexcept
	{
		m_Coordinator.ForEach<T_FUNCTION>( ArchetypeList, std::forward<T_FUNCTION&&>( Function ) );
	}

    template < concepts::Callable_Bool T_FUNCTION>
    void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList
						  , T_FUNCTION&& Function ) noexcept
	{
		m_Coordinator.ForEach<T_FUNCTION>( ArchetypeList, std::forward<T_FUNCTION&&>( Function ) );
	}

	template< typename T_SYSTEM >
	T_SYSTEM* instance::FindSystem( void ) noexcept
	{
		return m_Coordinator.FindSystem<T_SYSTEM>();
	}

	template< typename T_SYSTEM >
	T_SYSTEM& instance::GetSystem( void ) noexcept
	{
		return m_Coordinator.GetSystem<T_SYSTEM>();
	}

	PPB_FORCEINLINE
	float instance::DeltaTime() const noexcept
	{
		return m_Coordinator.DeltaTime();
	}
}