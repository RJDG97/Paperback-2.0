#include "PaperbackPCH.h"

#include "Log.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Paperback
{
	Log::MTWriter Log::mt_writer;
	Log::MTLogger Log::mt_logger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		mt_writer = spdlog::stdout_color_mt("Console");
		mt_logger = spdlog::basic_logger_mt("Logger", "DebugLogs/log.txt");

	#ifdef _DEBUG
		spdlog::set_level(spdlog::level::trace);
	#else
		spdlog::set_level(spdlog::level::err);
	#endif
	}
}