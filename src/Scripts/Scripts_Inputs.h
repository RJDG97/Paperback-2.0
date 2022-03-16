#pragma once

#include "Mono.h"
#include "paperback_input.h"
#include "paperback_coordinator.h"

namespace MONO_INPUTS
{
	MONO_EXPORT bool IsKeyPress(int Key)
	{
		return PPB.IsKeyPress(Key);
	}

	MONO_EXPORT bool IsKeyPressDown(int Key)
	{
		return PPB.IsKeyPressDown(Key);
	}

	MONO_EXPORT bool IsKeyPressUp(int Key)
	{
		return PPB.IsKeyPressUp(Key);
	}

	MONO_EXPORT bool IsGamepadButtonPress(int Key)
	{
		return PPB.IsGamepadButtonPress(Key);
	}

	MONO_EXPORT bool IsGamepadButtonPressDown(int Key)
	{
		return PPB.IsGamepadButtonPressDown(Key);
	}

	MONO_EXPORT bool IsGamepadButtonPressUp(int Key)
	{
		return PPB.IsGamepadButtonPressUp(Key);
	}

	MONO_EXPORT bool IsMousePress(int Key)
	{
		return PPB.IsMousePress(Key);
	}

	MONO_EXPORT bool IsMouseDown(int Key)
	{
		return PPB.IsMouseDown(Key);
	}

	MONO_EXPORT bool IsMouseUp(int Key)
	{
		return PPB.IsMouseUp(Key);
	}

	void AddInternals()
	{
		mono_add_internal_call("CSScript.Input::IsKeyPress(int)", &MONO_INPUTS::IsKeyPress);
		mono_add_internal_call("CSScript.Input::IsKeyPressDown(int)", &MONO_INPUTS::IsKeyPressDown);
		mono_add_internal_call("CSScript.Input::IsKeyPressUp(int)", &MONO_INPUTS::IsKeyPressUp);
		mono_add_internal_call("CSScript.Input::IsGamepadButtonPress(int)", &MONO_INPUTS::IsGamepadButtonPress);
		mono_add_internal_call("CSScript.Input::IsGamepadButtonPressDown(int)", &MONO_INPUTS::IsGamepadButtonPressDown);
		mono_add_internal_call("CSScript.Input::IsGamepadButtonPressUp(int)", &MONO_INPUTS::IsGamepadButtonPressUp);
		mono_add_internal_call("CSScript.Input::IsMousePress(int)", &MONO_INPUTS::IsMousePress);
		mono_add_internal_call("CSScript.Input::IsMouseDown(int)", &MONO_INPUTS::IsMouseDown);
		mono_add_internal_call("CSScript.Input::IsMouseUp(int)", &MONO_INPUTS::IsMouseUp);
	}
}