#pragma once

#include "Mono.h"
#include "paperback_camera.h"

namespace MONO_CAMERA_TOOLS
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
		mono_add_internal_call("CSScript.Tools.CameraTools::MoveForward()", &MONO_CAMERA_TOOLS::MoveForward);
		mono_add_internal_call("CSScript.Tools.CameraTools::MoveBackward()", &MONO_CAMERA_TOOLS::MoveBackward);
		mono_add_internal_call("CSScript.Tools.CameraTools::MoveLeft()", &MONO_CAMERA_TOOLS::MoveLeft);
		mono_add_internal_call("CSScript.Tools.CameraTools::MoveRight()", &MONO_CAMERA_TOOLS::MoveRight);
		mono_add_internal_call("CSScript.Tools.CameraTools::MoveUp()", &MONO_CAMERA_TOOLS::MoveUp);
		mono_add_internal_call("CSScript.Tools.CameraTools::MoveDown()", &MONO_CAMERA_TOOLS::MoveDown);
		mono_add_internal_call("CSScript.Tools.CameraTools::RotateUp()", &MONO_CAMERA_TOOLS::RotateUp);
		mono_add_internal_call("CSScript.Tools.CameraTools::RotateDown()", &MONO_CAMERA_TOOLS::RotateDown);
		mono_add_internal_call("CSScript.Tools.CameraTools::RotateLeft()", &MONO_CAMERA_TOOLS::RotateLeft);
		mono_add_internal_call("CSScript.Tools.CameraTools::RotateRight()", &MONO_CAMERA_TOOLS::RotateRight);
	}
}