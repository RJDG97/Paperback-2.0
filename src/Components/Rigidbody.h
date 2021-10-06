#pragma once

// for my knowledge only : jy -> transform shall store bounding shape
//                                  rigidbody shall store the velocity

#include "Physics/PhysicsMaterial.h"
#include "Math/Vector3f.h"
#include "Math/MathUtils.h"

// -------------------- Any object without a mass shall not be moveable by forces
// -- Rate of decay of the forces shall be VERY HIGH VERY VERY HIGH 
//                so as not to have obj leave the screen

// input system shall be 3d. meaning that input of forces through input system will be 3 dimensional

struct rigidbody
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Rigidbody"
    };

    xcore::vector3		m_Accel;
    xcore::vector3		m_Velocity;
    float				    m_Mass;
    //float _fColliderWidth;									// -- Width of bounding box
    //float _fColliderHeight;									// -- Height of bounding box
    //PhysicMaterial _Material;								// -- Material of the Entity
    //float _fInitWidth = -1.f, _fInitHeight = -1.f;			// -- Initial width and height
    float _fMass;											// -- Mass value of the object
    float _fInvMass;										// -- Inverse Mass value of the object
    //float _fRestituition;									// -- Elasticity, bounce factor for velocity when colliding
    float _fFriction = 1.f;									// -- Friction, defaulted to 1.f
    //float _fDrag;											// -- Drag (forces)
    //paperback::Vector3f _vAccel;							// -- Acceleration of rigidbody
    paperback::Vector3f m_Forces;						// -- Sum of Forces of rigidbody
    paperback::Vector3f m_Momentum;                         // -- Momentum of rigidbody
    //float _fRadius;											// -- Radius of the object
    //int _iCollisionFlag = 0;								// -- Collision flag for the rigidbody
    //int _iMapCollisionFlag = 0;								// -- Collision flag for environments
    //rigidbody* _pCollision = nullptr;						// -- Are there any collisions active on this rb

    // -- uni-direction for now
    void AddForce(const paperback::Vector3f& force)		// -- Applies a force
    {
        // -- No need to apply forces on objects with no mass
        if (paperback::MathUtils::IsEqualf(m_Mass, 0.f))
            return;

        m_Forces += force;
    }
    //void AddImpulse(const paperback::Vector3f impulse);		// -- Applies an impulse
    //void ApplyForces(float dt);								// -- Apply all forces
    void ResetForces()										// -- Resets all forces
    {
        m_Forces = paperback::Vector3f{};
    }
    void AddMomentum(const paperback::Vector3f& moment)		// -- Applies a force
    {
        // -- No need to apply forces on objects with no mass
        if (paperback::MathUtils::IsEqualf(m_Mass, 0.f))
            return;

        m_Momentum += moment;
    }
    void ResetMomentum()										// -- Resets all forces
    {
        m_Momentum = paperback::Vector3f{};
    }
};

namespace RR_Rigidbody
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<rigidbody>(rigidbody::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property("Acceleration", &rigidbody::m_Accel)
            .property("Velocity", &rigidbody::m_Velocity)
            .property("Mass", &rigidbody::m_Mass);
    }
}