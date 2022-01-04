#pragma once

#include "Mono.h"
#include "Scripts/Scripts_Camera.h"
#include "Scripts/Scripts_Inputs.h"
#include "Scripts/Scripts_Application.h"
#include "Scripts/Scripts_Debug.h"
#include "Scripts/Scripts_Transform.h"

namespace MONO_INTERNALS
{
	void MonoAddInternalCall()
	{
		MONO_CAMERA::AddInternals();
		MONO_INPUTS::AddInternals();
		MONO_APPLICATION::AddInternals();
		MONO_DEBUG::AddInternalCall();
		MONO_TRANSFORM::AddInternalCall();
	}
}