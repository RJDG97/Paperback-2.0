/**********************************************************************************
*\file         EditorPanels.h
*\brief        EditorPanels.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Paperback.h"
#include "ImguiSystem.h"

struct imgui_system;

namespace paperback::editor
{
	//-----------------------------------
	//        Panel Interface
	//-----------------------------------
	struct panel_interface
	{
		bool m_bEnabled { false };

		void OnSystemCreated( void ) noexcept {}
		void Update( void ) noexcept {}

		void Enable() { m_bEnabled = !m_bEnabled; }
		bool IsEnabled() { return m_bEnabled; }

	};

	struct instance : panel_interface
	{

		instance(imgui_system& Editor);

		instance ( const instance& ) = delete;
		instance& operator=( const instance& ) = delete;
		imgui_system& m_Imgui ;
	};


	//-----------------------------------
	//          Panel Types
	//-----------------------------------
	namespace type
	{
		using guid = xcore::guid::unit< 64, struct system_tag >;

		enum class id : u8
		{
			UPDATE = 0
		};

		enum class call
		{
			CREATED = 0
		,	UPDATE
		};

		struct update
		{
			static constexpr auto id_v = id::UPDATE;

			type::guid                m_Guid{ };
			const char*				  m_pName{ };
		};

		struct info
		{
			using RunSystem  = void( editor::instance&, type::call );
			using Destructor = void( editor::instance& ) noexcept;

			const type::guid             m_Guid;
			const type::id               m_TypeID;
										 
			RunSystem*					 m_RunSystem;
			Destructor*					 m_Destructor;
			const char*					 m_pName{ };
		};
	}

	//-----------------------------------
	//           System Info
	//-----------------------------------
	namespace details
	{
		template< typename T_PANEL >
		editor::type::info CreateInfo( void );

		template< typename T >
		static auto info_v = editor::details::CreateInfo<T>();
	}

	template< typename T_PANEL >
	auto& info_v = details::info_v< std::decay_t<T_PANEL> >;

	namespace details
	{
		template < typename USER_PANEL >
		struct completed final : USER_PANEL
		{
			completed(imgui_system& Editor);
			void Run( const editor::type::call Type );
		};
	}
}