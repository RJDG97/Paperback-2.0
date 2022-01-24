#pragma once

#include "Mono.h"
#include "paperback_camera.h"

namespace MONO_CAMERA
{

	MONO_EXPORT void MoveForward()
	{
		cam::GetInstanced().MoveForward();
	}

	MONO_EXPORT void MoveBackward()
	{
		cam::GetInstanced().MoveBackward();
	}

	MONO_EXPORT void MoveLeft()
	{
		cam::GetInstanced().MoveLeft();
	}

	MONO_EXPORT void MoveRight()
	{
		cam::GetInstanced().MoveRight();
	}

	MONO_EXPORT void MoveUp()
	{
		cam::GetInstanced().MoveUp();
	}

	MONO_EXPORT void MoveDown()
	{
		cam::GetInstanced().MoveDown();
	}

	MONO_EXPORT void RotateUp()
	{
		cam::GetInstanced().RotateUp();
	}

	MONO_EXPORT void RotateDown()
	{
		cam::GetInstanced().RotateDown();
	}

	MONO_EXPORT void RotateLeft()
	{
		cam::GetInstanced().RotateLeft();
	}

	MONO_EXPORT void RotateRight()
	{
		cam::GetInstanced().RotateRight();
	}

	void AddInternals()
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