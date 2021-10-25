#pragma once
#include "spdlog/include/spdlog/sinks/basic_file_sink.h"
#include "spdlog/include/spdlog/sinks/stdout_color_sinks.h"

namespace paperback
{
	//logger::MTWriter logger::mt_writer;
	//logger::MTLogger logger::mt_logger;

	void logger::Init( void )
	{
        spdlog::set_pattern("%^[%T] %n: %v%$");

		auto GetCurrent = []() -> std::string
								  {
								  	  time_t now = time( 0 );
								  	  char Str[ 26 ];
								  	  ctime_s( Str, sizeof Str, &now );
								  	  
								  	  std::string Time = Str;
								  	  Time.erase( Time.find('\n') );
								  	  std::replace( Time.begin(), Time.end(), ' ', '_' );
								  	  std::replace( Time.begin(), Time.end(), ':', '-' );
								  	  
								  	  return Time;
								  };

		mt_writer = spdlog::stdout_color_mt( "User" );


	#ifdef PAPERBACK_DEBUG
		spdlog::set_level( spdlog::level::trace );
		mt_logger = spdlog::basic_logger_mt( "System", "../../logs/DebugLog__" + GetCurrent() + ".txt" );
		spdlog::flush_every( std::chrono::seconds(5) );
		//mt_logger->flush_on( spdlog::level::trace );
	#else
		spdlog::set_level( spdlog::level::err );
		mt_logger = spdlog::basic_logger_mt( "System", "../../logs/ReleaseLog__" + GetCurrent() + ".txt" );
		spdlog::flush_every( std::chrono::seconds(5) );
		//mt_logger->flush_on( spdlog::level::err );
	#endif
	}
}