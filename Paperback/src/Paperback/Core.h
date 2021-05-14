#pragma once

/*
	https://jeffpar.github.io/kbarchive/kb/168/Q168958/
*/

#ifdef PAPERBACK_PLATFORM_WINDOWS
	#ifdef PAPERBACK_BUILD_DLL
		#define PAPERBACK_API __declspec(dllexport)
	#else
		#define PAPERBACK_API __declspec(dllimport)
	#endif
#else
	#error Paperback only supports x64
#endif