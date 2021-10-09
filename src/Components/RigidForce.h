#pragma once

// for my knowledge only : jy -> transform shall store bounding shape
//                                  rigidbody shall store the velocity

#include "Math/Vector3f.h"
#include "Math/MathUtils.h"

// -------------------- Any object without a mass shall not be moveable by forces
// -- Rate of decay of the forces shall be VERY HIGH VERY VERY HIGH 
//                so as not to have obj leave the screen

// input system shall be 3d. meaning that input of forces through input system will be 3 dimensional



struct rigidforce
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "RigidForce"
    };

    paperback::Vector3f m_MaxForce = paperback::Vector3f(3.f, 3.f, 3.f);
    float m_MaxForceSq = m_MaxForce.MagnitudeSq();
    paperback::Vector3f m_MaxMoment = paperback::Vector3f(5.f, 5.f, 5.f);
    float m_MaxMomentSq = m_MaxMoment.MagnitudeSq();
    float m_threshold = .25f;

    float m_Mass;                                              // -- Mass value of the object
    float m_InvMass;										// -- Inverse Mass value of the object
    float m_staticFriction;									// -- Friction, defaulted to 1.f
    float m_dynamicFriction;
    float m_modifier = 0.99f;                                       // -- if there's a need to curb its accel/velo
    paperback::Vector3f m_Forces;						// -- Sum of Forces of rigidbody
    paperback::Vector3f m_Momentum;                         // -- Momentum of rigidbody
    float m_MagForce;
    paperback::Vector3f m_NegForces;
    float m_MagMoment;
    bool m_isAccel;

    //float _fColliderWidth;									// -- Width of bounding box
    //float _fColliderHeight;									// -- Height of bounding box
    //PhysicMaterial _Material;								// -- Material of the Entity
    //float _fInitWidth = -1.f, _fInitHeight = -1.f;			// -- Initial width and height
    //float _fRestituition;									// -- Elasticity, bounce factor for velocity when colliding



    //float m_Drag;											// -- Drag (forces)
    //paperback::Vector3f _vAccel;							// -- Acceleration of rigidbody



    //float _fRadius;											// -- Radius of the object
    //int _iCollisionFlag = 0;								// -- Collision flag for the rigidbody
    //int _iMapCollisionFlag = 0;								// -- Collision flag for environments
    //rigidbody* _pCollision = nullptr;						// -- Are there any collisions active on this rb

    rigidforce() :
        m_Mass{ 0.f },
        m_InvMass{ 0.f },
        m_staticFriction{ .5f },
        m_dynamicFriction{ 0.25f },
        m_Forces{ paperback::Vector3f{} },
        m_Momentum{ paperback::Vector3f{} },
        m_MagForce{ 0.f },
        m_NegForces{ paperback::Vector3f{} },
        m_MagMoment{ 0.f },
        m_isAccel{ false }
    {}

    rigidforce(float mass) :
        m_Mass{ mass },
        m_InvMass{ 1 / mass },
        m_staticFriction{ .5f },
        m_dynamicFriction{ 0.25f },
        m_Forces{ paperback::Vector3f{} },
        m_Momentum{ paperback::Vector3f{} },
        m_MagForce{ 0.f },
        m_NegForces{ paperback::Vector3f{} },
        m_MagMoment{ 0.f },
        m_isAccel{ false }
    {}

    //rigidforce(float mass) :

    // formulae -> modifier will be the terminal velocity and terminal force acceleration
    // gravity?
    // Remember*** A force is simply a COLLISIOM resolution for the response and FINAL ACCELERATION

    // -- uni-direction for now
    void AddForce(const paperback::Vector3f& force)		// -- Applies a force
    {
        // -- No need to apply forces on objects with no mass
        //if (paperback::MathUtils::IsEqualf(m_Mass, 0.f))
        //    return;

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
        //if (paperback::MathUtils::IsEqualf(m_Mass, 0.f))
        //    return;

        m_Momentum += moment;
    }
    void ResetMomentum()										// -- Resets all forces
    {
        m_Momentum = paperback::Vector3f{};
    }

    paperback::Vector3f ConvertToAccel()
    {
        return (m_Mass > 0) ? m_Forces / m_Mass : paperback::Vector3f{ 0.0f, 0.0f, 0.0f };
    }

    paperback::Vector3f ConvertToVelocity()
    {
        return  (m_Mass > 0) ? m_Momentum / m_Mass : paperback::Vector3f{ 0.0f, 0.0f, 0.0f };
    }
};


namespace RR_RIGIDFORCE
{
    RTTR_REGISTRATION
    {
       rttr::registration::class_<rigidforce>(rigidforce::typedef_v.m_pName)
           .constructor()(rttr::policy::ctor::as_object)
           .property("Mass", &rigidforce::m_Mass)(rttr::policy::prop::as_reference_wrapper)
           .property("Inverse Mass", &rigidforce::m_InvMass)(rttr::policy::prop::as_reference_wrapper)
           .property("Static Friction", &rigidforce::m_staticFriction)(rttr::policy::prop::as_reference_wrapper)
           .property("Dynamic Friction", &rigidforce::m_dynamicFriction)(rttr::policy::prop::as_reference_wrapper);
    }
}