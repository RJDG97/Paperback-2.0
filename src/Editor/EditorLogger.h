/**********************************************************************************
*\file         EditorLogger.h
*\brief        EditorLogger.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "../../build/Paperback_V2/paperback_pch.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <dearImGui/imterm/terminal.hpp>
#include <dearImGui/imterm/terminal_commands.hpp>


namespace paperback
{
	class EditorLogger
	{
	public:
		using ELogger = std::shared_ptr<spdlog::logger>;
		using EWriter = std::shared_ptr<spdlog::logger>;
		using ETerminal = std::shared_ptr<ImTerm::terminal<terminal_commands>>;

		inline static void Init(void);

		inline static ELogger& GetLogger(void) { return m_CoreLogger; }

		inline static EWriter& GetWriter(void) { return m_ClientLogger; }

		inline static ETerminal& GetTerminal(void) { return m_Terminal; };

	private:

		static ELogger m_CoreLogger;
		static EWriter m_ClientLogger;
		static ETerminal m_Terminal;
	};
}

// Writer Methods
#define EDITOR_TRACE_PRINT( ... ) ::paperback::EditorLogger::GetLogger()->trace( __VA_ARGS__ )
#define EDITOR_DEBUG_PRINT( ... ) ::paperback::EditorLogger::GetLogger()->debug( __VA_ARGS__ )
#define EDITOR_INFO_PRINT( ... ) ::paperback::EditorLogger::GetLogger()->info( __VA_ARGS__ )
#define EDITOR_WARN_PRINT( ... ) ::paperback::EditorLogger::GetLogger()->warn( __VA_ARGS__ )
#define EDITOR_ERROR_PRINT( ... ) ::paperback::EditorLogger::GetLogger()->error( __VA_ARGS__ )
#define EDITOR_CRITICAL_PRINT( ... ) ::paperback::EditorLogger::GetLogger()->critical( __VA_ARGS__ )

// Logger Methods
#define EDITOR_TRACE( ... ) ::paperback::EditorLogger::GetWriter()->trace( __VA_ARGS__ )
#define EDITOR_DEBUG( ... ) ::paperback::EditorLogger::GetWriter()->debug( __VA_ARGS__ )
#define EDITOR_INFO( ... ) ::paperback::EditorLogger::GetWriter()->info( __VA_ARGS__ )
#define EDITOR_WARN( ... ) ::paperback::EditorLogger::GetWriter()->warn( __VA_ARGS__ )
#define EDITOR_ERROR( ... ) ::paperback::EditorLogger::GetWriter()->error( __VA_ARGS__ )
#define EDITOR_CRITICAL( ... ) ::paperback::EditorLogger::GetWriter()->critical( __VA_ARGS__ )
