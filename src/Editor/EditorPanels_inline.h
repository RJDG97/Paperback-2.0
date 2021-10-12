#pragma once
#include "Editor/EditorPanels.h"

struct imgui_system;

namespace paperback::editor
{
	//-----------------------------------
	//    Panel Info Initialization
	//-----------------------------------

	namespace details
	{
		template< typename T_PANEL >
		editor::type::info CreateInfo( void )
		{
			return editor::type::info
			{
				.m_Guid = T_PANEL::typedef_v.m_Guid.isValid()
								   ? T_PANEL::typedef_v.m_Guid
								   : type::guid{ __FUNCSIG__ }

			,   .m_TypeID = T_PANEL::typedef_v.id_v
			,	.m_RunSystem = []( editor::instance& pPanel, editor::type::call UpdateType )
								   {
									   static_cast< editor::details::completed<T_PANEL>& >( pPanel ).Run( UpdateType );
								   }
			,	.m_Destructor = []( editor::instance& pPanel ) noexcept
								   {
									   std::destroy_at(&static_cast<editor::details::completed<T_PANEL>&>( pPanel ));
								   }
			,	.m_pName = T_PANEL::typedef_v.m_pName
			};
		}
	}

	//-----------------------------------
	//         Panel Details
	//-----------------------------------

	template<typename USER_PANEL>
	details::completed<USER_PANEL>::completed(imgui_system& Editor) : USER_PANEL{ Editor }
	{}

	template < typename USER_PANEL >
	void details::completed< USER_PANEL >::Run( const editor::type::call Type ) 
	{
		switch ( Type )
		{
			case editor::type::call::CREATED:
			{
				if constexpr ( &USER_PANEL::OnSystemCreated != &panel_interface::OnSystemCreated )
				{
					USER_PANEL::OnSystemCreated();
				}
				break;
			}
			case editor::type::call::UPDATE:
			{
				if constexpr ( &USER_PANEL::Update != &panel_interface::Update )
				{
					USER_PANEL::Update();
				}
				break;
			}
		}
	}
}