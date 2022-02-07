#pragma once

#include "Mono.h"

namespace MONO_APPLICATION
{
	MONO_EXPORT void Quit()
	{
		return PPB.QuitApplication();
	}

	MONO_EXPORT void ChangeScene(MonoString* scene_name)
	{

		PPB.QueueScene(mono_string_to_utf8(scene_name));
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Application::Quit()", &MONO_APPLICATION::Quit);
		mono_add_internal_call("CSScript.Application::ChangeScene(string)", &MONO_APPLICATION::ChangeScene);
	}

}
