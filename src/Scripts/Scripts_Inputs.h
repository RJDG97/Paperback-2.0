#pragma once

#include "Mono.h"
#include "paperback_input.h"
#include "paperback_coordinator.h"

namespace MONO_INPUTS
{
	MONO_EXPORT bool IsKeyPress(int Key)
	{
		return PPB.m_Input.IsKeyPress(Key);
	}

	MONO_EXPORT bool IsKeyPressDown(int Key)
	{
		return PPB.m_Input.IsKeyPressDown(Key);
	}

	MONO_EXPORT bool IsKeyPressUp(int Key)
	{
		return PPB.m_Input.IsKeyPressUp(Key);
	}

	MONO_EXPORT bool IsMousePress(int Key)
	{
		return PPB.m_Input.IsMousePress(Key);
	}

	MONO_EXPORT bool IsMouseDown(int Key)
	{
		return PPB.m_Input.IsMouseDown(Key);
	}

	MONO_EXPORT bool IsMouseUp(int Key)
	{
		return PPB.m_Input.IsMouseUp(Key);
	}

	void Add_Internals()
	{
		mono_add_internal_call("CSScript.Inputs::IsKeyPress(int)", &MONO_INPUTS::IsKeyPress);
		mono_add_internal_call("CSScript.Inputs::IsKeyPressDown(int)", &MONO_INPUTS::IsKeyPressDown);
		mono_add_internal_call("CSScript.Inputs::IsKeyPressUp(int)", &MONO_INPUTS::IsKeyPressUp);
		mono_add_internal_call("CSScript.Inputs::IsMousePress(int)", &MONO_INPUTS::IsMousePress);
		mono_add_internal_call("CSScript.Inputs::IsMouseDown(int)", &MONO_INPUTS::IsMouseDown);
		mono_add_internal_call("CSScript.Inputs::IsMouseUp(int)", &MONO_INPUTS::IsMouseUp);
	}
}