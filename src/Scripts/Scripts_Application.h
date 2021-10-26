#pragma once

#include "Mono.h"

namespace MONO_APPLICATION
{
	MONO_EXPORT void Quit()
	{
		return PPB.QuitApplication();
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Application::Quit()", &MONO_APPLICATION::Quit);
	}

}
