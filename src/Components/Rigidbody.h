#pragma once

// for my knowledge only : jy -> transform shall store bounding shape
//                                  rigidbody shall store the velocity

#include "Physics/PhysicsMaterial.h"
#include "Math/Vector3f.h"
#include "Physics/PhysicsStructs.h"

struct rigidbody
{
	constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Rigidbody"
    };

	xcore::vector3		m_Velocity;
	float				    m_Mass;
    ForceHandler    m_ForceHandler; 
    //float _fColliderWidth;									// -- Width of bounding box
    //float _fColliderHeight;									// -- Height of bounding box
    //PhysicMaterial _Material;								// -- Material of the Entity
    //float _fInitWidth = -1.f, _fInitHeight = -1.f;			// -- Initial width and height
    //float _fMass;											// -- Mass value of the object
    //float _fInvMass;										// -- Inverse Mass value of the object
    //float _fRestituition;									// -- Elasticity, bounce factor for velocity when colliding
    //float _fFriction = 1.f;									// -- Friction, defaulted to 1.f
    //float _fDrag;											// -- Drag (forces)
    //paperback::Vector3f _vAccel;							// -- Acceleration of rigidbody
    //paperback::Vector3f _vSumofForces;						// -- Sum of Forces of rigidbody
    //float _fRadius;											// -- Radius of the object
    //int _iCollisionFlag = 0;								// -- Collision flag for the rigidbody
    //int _iMapCollisionFlag = 0;								// -- Collision flag for environments
    //rigidbody* _pCollision = nullptr;						// -- Are there any collisions active on this rb
    //void AddForce(const paperback::Vector3f& force);		// -- Applies a force
    //void AddImpulse(const paperback::Vector3f impulse);		// -- Applies an impulse
    //void ApplyForces(float dt);								// -- Apply all forces
    //void ResetForces();										// -- Resets all forces
};

namespace RR_Rigidbody
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<rigidbody>(rigidbody::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property("Velocity", &rigidbody::m_Velocity)
            .property("Mass", &rigidbody::m_Mass);
    }
}