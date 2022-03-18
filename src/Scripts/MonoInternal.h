#pragma once

#include "Mono.h"
#include "Scripts/Scripts_CameraTools.h"
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
#include "Scripts/Scripts_CollisionLayer.h"
#include "Scripts/Scripts_Mass.h"
#include "Scripts/Scripts_Rigidbody.h"
#include "Scripts/Scripts_Rigidforce.h"
#include "Scripts/Scripts_Pushable.h"
#include "Scripts/Scripts_Sound.h"
#include "Scripts/Scripts_Sphere.h"
#include "Scripts/Scripts_Text.h"
#include "Scripts/Scripts_Child.h"
#include "Scripts/Scripts_Parent.h"
#include "Scripts/Scripts_Player.h"
#include "Scripts/Scripts_Camera.h"
#include "Scripts/Scripts_Elevator.h"
#include "Scripts/Scripts_Raycast.h"
#include "Scripts/Scripts_PlayerController.h"
#include "Scripts/Scripts_AbilityCollectible.h"
#include "Scripts/Scripts_Tag.h"
#include "Scripts/Scripts_DialogueCollider.h"
#include "Scripts/Scripts_DialogueText.h"
#include "Scripts/Scripts_ParticleEmitter.h"

namespace MONO_INTERNALS
{
	void MonoAddInternalCall()
	{
		MONO_CAMERA_TOOLS::AddInternals();
		MONO_INPUTS::AddInternals();
		MONO_PLAYER::AddInternals();
		MONO_RAYCAST::AddInternals();
		MONO_TAG::AddInternals();
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
		MONO_COLLISIONLAYER::AddInternalCall();
		MONO_PATHFOLLOWER::AddInternalCall();
		MONO_MASS::AddInternalCall();
		MONO_RIGIDBODY::AddInternalCall();
		MONO_RIGIDFORCE::AddInternalCall();
		MONO_SPHERE::AddInternalCall();
		MONO_SOUND::AddInternalCall();
		MONO_TEXT::AddInternalCall();
		MONO_CHILD::AddInternalCall();
		MONO_PARENT::AddInternalCall();
		MONO_ELEVATOR::AddInternalCall();
		MONO_CAMERA::AddInternalCall();
		MONO_PUSHABLE::AddInternalCall();
		MONO_PLAYERCONTROLLER::AddInternalCall();
		MONO_ABILITYCOLLECTIBLE::AddInternalCall();
		MONO_DIALOGUECOLLIDER::AddInternalCall();
		MONO_DIALOGUETEXT::AddInternalCall();
		MONO_PARTICLEEMITTER::AddInternalCall();
	}
}