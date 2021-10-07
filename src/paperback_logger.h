#pragma once

#include "spdlog/include/spdlog/spdlog.h"
#include "spdlog/include/spdlog/fmt/ostr.h"

namespace paperback
{
	class logger
	{
	public:

		using MTLogger = std::shared_ptr< spdlog::logger >;
		using MTWriter = std::shared_ptr< spdlog::logger >;

		PPB_INLINE
		static void Init( void );

		PPB_INLINE
		static MTLogger& GetLogger( void ) { return mt_logger; }

		PPB_INLINE
		static MTWriter& GetWriter( void ) { return mt_writer; }

	private:

		// Loggers & filesink
		static MTWriter mt_writer;
		static MTLogger mt_logger;
	};
}


#ifdef PAPERBACK_DEBUG


	// Writer Methods
	#define TRACE_PRINT( ... ) ::paperback::logger::GetWriter()->trace( __VA_ARGS__ )
	#define DEBUG_PRINT( ... ) ::paperback::logger::GetWriter()->debug( __VA_ARGS__ )
	#define INFO_PRINT( ... ) ::paperback::logger::GetWriter()->info( __VA_ARGS__ )
	#define WARN_PRINT( ... ) ::paperback::logger::GetWriter()->warn( __VA_ARGS__ )
	#define ERROR_PRINT( ... ) ::paperback::logger::GetWriter()->error( __VA_ARGS__ )
	#define CRITICAL_PRINT( ... ) ::paperback::logger::GetWriter()->critical( __VA_ARGS__ )

	// Logger Methods
	#define TRACE_LOG( ... ) ::paperback::logger::GetLogger()->trace( __VA_ARGS__ )
	#define DEBUG_LOG( ... ) ::paperback::logger::GetLogger()->debug( __VA_ARGS__ )
	#define INFO_LOG( ... ) ::paperback::logger::GetLogger()->info( __VA_ARGS__ )
	#define WARN_LOG( ... ) ::paperback::logger::GetLogger()->warn( __VA_ARGS__ )
	#define ERROR_LOG( ... ) ::paperback::logger::GetLogger()->error( __VA_ARGS__ )
	#define CRITICAL_LOG( ... ) ::paperback::logger::GetLogger()->critical( __VA_ARGS__ )

	// Assertion Methods
	#define PPB_ASSERT( expr )														\
		if (expr) {																	\
			ERROR_PRINT("Assertion occured at {}: Line {}", __FILE__, __LINE__);	\
			ERROR_LOG("Assertion occured at {}: Line {}", __FILE__, __LINE__);		\
			DEBUG_BREAK();															\
		}

	#define PPB_ASSERT_MSG( expr, ... )												\
		if (expr) {																	\
			ERROR_PRINT("Assertion occured at {}: Line {}", __FILE__, __LINE__);	\
			ERROR_PRINT(__VA_ARGS__);												\
			ERROR_LOG("Assertion occured at {}: Line {}", __FILE__, __LINE__);		\
			ERROR_LOG(__VA_ARGS__);													\
			DEBUG_BREAK();															\
		}

#else

	// Writer Methods
	#define TRACE_PRINT( ... ) 
	#define DEBUG_PRINT( ... ) 
	#define INFO_PRINT( ... )
	#define WARN_PRINT( ... )
	#define ERROR_PRINT( ... ) ::paperback::logger::GetWriter()->error( __VA_ARGS__ )
	#define CRITICAL_PRINT( ... ) ::paperback::logger::GetWriter()->critical( __VA_ARGS__ )

	// Logger Methods
	#define TRACE_LOG( ... )
	#define DEBUG_LOG( ... )
	#define INFO_LOG( ... )
	#define WARN_LOG( ... )
	#define ERROR_LOG( ... ) ::paperback::logger::GetLogger()->error( __VA_ARGS__ )
	#define CRITICAL_LOG( ... ) ::paperback::logger::GetLogger()->critical( __VA_ARGS__ )

	// Assertion Methods
	#define PPB_ASSERT( expr )
	#define PPB_ASSERT_MSG( expr, ... )

#endif

// Critical Assertion Methods
#define PPB_CRITICAL_ASSERT( expr )												\
	if (expr) {																	\
		CRITICAL_PRINT("Assertion occured at {}: Line {}", __FILE__, __LINE__);	\
		CRITICAL_LOG("Assertion occured at {}: Line {}", __FILE__, __LINE__);	\
		DEBUG_BREAK();															\
	}

#define PPB_CRITICAL_ASSERT_MSG( expr, ... )									\
	if (expr) {																	\
		CRITICAL_PRINT("Assertion occured at {}: Line {}", __FILE__, __LINE__);	\
		CRITICAL_PRINT(__VA_ARGS__);											\
		CRITICAL_LOG("Assertion occured at {}: Line {}", __FILE__, __LINE__);	\
		CRITICAL_LOG(__VA_ARGS__);												\
		DEBUG_BREAK();															\
	}

#define PPB_ERR_PRINT_N_LOG( ... ) {											\
		ERROR_PRINT("Error occured at {}: Line {}", __FILE__, __LINE__);		\
		ERROR_PRINT(__VA_ARGS__);												\
		ERROR_LOG("Error occured at {}: Line {}", __FILE__, __LINE__);			\
		ERROR_LOG(__VA_ARGS__);													\
	}