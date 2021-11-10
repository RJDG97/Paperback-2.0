#pragma once

#include "Mono.h"
#include "paperback_camera.h"

namespace MONO_CAMERA
{

	MONO_EXPORT void MoveForward()
	{
		Camera3D::GetInstanced().MoveForward();
	}

	MONO_EXPORT void MoveBackward()
	{
		Camera3D::GetInstanced().MoveBackward();
	}

	MONO_EXPORT void MoveLeft()
	{
		Camera3D::GetInstanced().MoveLeft();
	}

	MONO_EXPORT void MoveRight()
	{
		Camera3D::GetInstanced().MoveRight();
	}

	MONO_EXPORT void MoveUp()
	{
		Camera3D::GetInstanced().MoveUp();
	}

	MONO_EXPORT void MoveDown()
	{
		Camera3D::GetInstanced().MoveDown();
	}

	MONO_EXPORT void RotateUp()
	{
		Camera3D::GetInstanced().RotateUp();
	}

	MONO_EXPORT void RotateDown()
	{
		Camera3D::GetInstanced().RotateDown();
	}

	MONO_EXPORT void RotateLeft()
	{
		Camera3D::GetInstanced().RotateLeft();
	}

	MONO_EXPORT void RotateRight()
	{
		Camera3D::GetInstanced().RotateRight();
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