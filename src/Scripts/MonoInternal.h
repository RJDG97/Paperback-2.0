#pragma once

#include "Mono.h"
#include "Scripts/Scripts_Camera.h"
#include "Scripts/Scripts_Inputs.h"

#include "Mono.h"

namespace MONO_INTERNALS
{
	MONO_EXPORT void print(MonoString* string)
	{
		std::cout << mono_string_to_utf8(string) << std::endl;
	}

	void MonoAddInternalCall()
	{
		MONO_CAMERA::AddInternals();
		MONO_INPUTS::AddInternals();
		mono_add_internal_call("CSScript.Main::print(string)", &print);
	}
}