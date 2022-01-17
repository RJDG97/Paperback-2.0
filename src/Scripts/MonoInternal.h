#pragma once

#include "Mono.h"
#include "Scripts/Scripts_Camera.h"
#include "Scripts/Scripts_Inputs.h"
#include "Scripts/Scripts_Application.h"
#include "Scripts/Scripts_Debug.h"
#include "Scripts/Scripts_AABB.h"
#include "Scripts/Scripts_Animator.h"
#include "Scripts/Scripts_BoundingBox.h"
#include "Scripts/Scripts_Transform.h"
#include "Scripts/Scripts_Mesh.h"
#include "Scripts/Scripts_Name.h"
#include "Scripts/Scripts_Offset.h"
#include "Scripts/Scripts_Rotation.h"
#include "Scripts/Scripts_Scale.h"
#include "Scripts/Scripts_PathFollower.h"

namespace MONO_INTERNALS
{
	void MonoAddInternalCall()
	{
		MONO_CAMERA::AddInternals();
		MONO_INPUTS::AddInternals();
		MONO_APPLICATION::AddInternals();
		MONO_DEBUG::AddInternalCall();
		MONO_AABB::AddInternalCall();
		MONO_ANIMATOR::AddInternalCall();
		MONO_BOUNDINGBOX::AddInternalCall();
		MONO_TRANSFORM::AddInternalCall();
		MONO_MESH::AddInternalCall();
		MONO_NAME::AddInternalCall();
		MONO_OFFSET::AddInternalCall();
		MONO_ROTATION::AddInternalCall();
		MONO_SCALE::AddInternalCall();
		MONO_PATHFOLLOWER::AddInternalCall();
	}
}