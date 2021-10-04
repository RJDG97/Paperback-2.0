#pragma once

#include "Mono.h"
#include "Scripts/Scripts_Camera.h"
#include "Scripts/Scripts_Inputs.h"

namespace MONO_INTERNALS
{
	void Mono_Add_Internal_Call()
	{
		MONO_CAMERA::Add_Internals();
		MONO_INPUTS::Add_Internals();
	}
}