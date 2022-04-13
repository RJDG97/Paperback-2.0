/**********************************************************************************
*\file         EditorLogger_Inline.h
*\brief        EditorLogger_Inline.h
*
*\author	   Adele Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
					 or disclosure of this file or its contents without the prior
					 written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "EditorLogger.h"

namespace paperback
{
	EditorLogger::EWriter EditorLogger::m_CoreLogger;
	EditorLogger::ELogger EditorLogger::m_ClientLogger;
	EditorLogger::ETerminal EditorLogger::m_Terminal;

	void EditorLogger::Init(void)
	{
		std::vector<spdlog::sink_ptr> Sinks;

		custom_command_struct Cmd;
		m_Terminal = std::make_shared<ImTerm::terminal<terminal_commands>>(Cmd);
		m_Terminal->set_min_log_level(ImTerm::message::severity::info);
		spdlog::set_level(spdlog::level::trace);

		//Setting Color
		m_Terminal->theme().log_level_colors[ImTerm::message::severity::trace] = ImTerm::theme::constexpr_color{ 1.0f, 1.0f, 1.0f, 1.0f };
		m_Terminal->theme().log_level_colors[ImTerm::message::severity::info] = ImTerm::theme::constexpr_color{ 0.0f, 1.0f, 0.0f, 1.0f };
		m_Terminal->theme().log_level_colors[ImTerm::message::severity::warn] = ImTerm::theme::constexpr_color{ 1.0f, 1.0f, 0.0f, 1.0f };
		m_Terminal->theme().log_level_colors[ImTerm::message::severity::err] = ImTerm::theme::constexpr_color{ 1.0f, 0.0f, 0.0f, 1.0f };
		m_Terminal->theme().log_level_colors[ImTerm::message::severity::critical] = ImTerm::theme::constexpr_color{ 1.0f, 0.0f, 0.0f, 1.0f };

		m_Terminal->set_autocomplete_pos(ImTerm::position::nowhere);

		Sinks.emplace_back(m_Terminal->get_terminal_helper());
		Sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

		Sinks[0]->set_pattern("%^[%T] %n (%l) - %v%$");
		Sinks[1]->set_pattern("%^[%T] %n (%l) - %v%$");

		//Sinks[0]->set_pattern("%^[%T] %n - %v%$");
		//Sinks[1]->set_pattern("%^[%T] %n - %v%$");


		m_CoreLogger = std::make_shared<spdlog::logger>("Editor", Sinks.begin(), Sinks.end());
		m_CoreLogger->set_level(spdlog::level::trace);
		m_CoreLogger->flush_on(spdlog::level::trace);
		spdlog::register_logger(m_CoreLogger);

		m_ClientLogger = std::make_shared<spdlog::logger>("EditorLog", Sinks.begin(), Sinks.end());
		m_ClientLogger->set_level(spdlog::level::trace);
		m_ClientLogger->flush_on(spdlog::level::trace);
		spdlog::register_logger(m_ClientLogger);
	}
}
