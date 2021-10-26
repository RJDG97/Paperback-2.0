#pragma once

#include "Mono.h"

namespace MONO_DEBUG
{
	MONO_EXPORT void Log(MonoString* string)
	{
		DEBUG_PRINT(mono_string_to_utf8(string));
	}

	//MONO_EXPORT void DrawLine()
	//{
	//}

	void AddInternalCall()
	{
		mono_add_internal_call("CSScript.Debug::Log(string)", &MONO_DEBUG::Log);
	}
}