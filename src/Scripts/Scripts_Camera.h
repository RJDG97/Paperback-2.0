#pragma once

#include "Mono.h"
#include "paperback_camera.h"

namespace MONO_CAMERA
{

	MONO_EXPORT void MoveForward()
	{
		Camera::GetInstanced().MoveForward();
	}

	MONO_EXPORT void MoveBackward()
	{
		Camera::GetInstanced().MoveBackward();
	}

	MONO_EXPORT void MoveLeft()
	{
		Camera::GetInstanced().MoveLeft();
	}

	MONO_EXPORT void MoveRight()
	{
		Camera::GetInstanced().MoveRight();
	}

	MONO_EXPORT void MoveUp()
	{
		Camera::GetInstanced().MoveUp();
	}

	MONO_EXPORT void MoveDown()
	{
		Camera::GetInstanced().MoveDown();
	}

	MONO_EXPORT void RotateUp()
	{
		Camera::GetInstanced().RotateUp();
	}

	MONO_EXPORT void RotateDown()
	{
		Camera::GetInstanced().RotateDown();
	}

	MONO_EXPORT void RotateLeft()
	{
		Camera::GetInstanced().RotateLeft();
	}

	MONO_EXPORT void RotateRight()
	{
		Camera::GetInstanced().RotateRight();
	}

	void Add_Internals()
	{
		mono_add_internal_call("CSScript.Camera::MoveForward()", &MONO_CAMERA::MoveForward);
		mono_add_internal_call("CSScript.Camera::MoveBackward()", &MONO_CAMERA::MoveBackward);
		mono_add_internal_call("CSScript.Camera::MoveLeft()", &MONO_CAMERA::MoveLeft);
		mono_add_internal_call("CSScript.Camera::MoveRight()", &MONO_CAMERA::MoveRight);
		mono_add_internal_call("CSScript.Camera::MoveUp()", &MONO_CAMERA::MoveUp);
		mono_add_internal_call("CSScript.Camera::MoveDown()", &MONO_CAMERA::MoveDown);
		mono_add_internal_call("CSScript.Camera::RotateUp()", &MONO_CAMERA::RotateUp);
		mono_add_internal_call("CSScript.Camera::RotateDown()", &MONO_CAMERA::RotateDown);
		mono_add_internal_call("CSScript.Camera::RotateLeft()", &MONO_CAMERA::RotateLeft);
		mono_add_internal_call("CSScript.Camera::RotateRight()", &MONO_CAMERA::RotateRight);
	}
}