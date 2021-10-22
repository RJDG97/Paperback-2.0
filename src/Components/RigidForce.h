#pragma once

#include "Math/Vector3f.h"
#include "Math/MathUtils.h"

struct rigidforce
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "RigidForce"
    };

    paperback::Vector3f m_MaxForce = paperback::Vector3f(3.f, 3.f, 3.f);
    float m_MaxForceSq = m_MaxForce.MagnitudeSq();
    paperback::Vector3f m_MaxMoment = paperback::Vector3f(8.f, 8.f, 8.f);
    float m_MaxMomentSq = m_MaxMoment.MagnitudeSq();
    paperback::Vector3f m_minthreshold = paperback::Vector3f(0.5f, 0.5f, 0.5f);
    float m_threshold = .25f;

    float m_Mass;                                              // -- Mass value of the object
    float m_InvMass;										// -- Inverse Mass value of the object
    float m_staticFriction;									// -- Friction, defaulted to 1.f
    float m_dynamicFriction;
    float m_modifier = 0.99f;                                       // -- if there's a need to curb its accel/velo
    paperback::Vector3f m_Forces;						// -- Sum of Forces of rigidbody
    paperback::Vector3f m_Momentum;                         // -- Momentum of rigidbody
    float m_MagForce;
    float m_MagMoment;
    bool m_isAccel;

    rigidforce() :
        m_Mass{ 5.f },
        m_InvMass{ 0.2f },
        m_staticFriction{ .5f },
        m_dynamicFriction{ 0.25f },
        m_Forces{ paperback::Vector3f{} },
        m_Momentum{ paperback::Vector3f{} },
        m_MagForce{ 0.f },
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
        m_MagMoment{ 0.f },
        m_isAccel{ false }
    {}
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
           .property("Dynamic Friction", &rigidforce::m_dynamicFriction)(rttr::policy::prop::as_reference_wrapper)
           .property("Threshold", &rigidforce::m_threshold)(rttr::policy::prop::as_reference_wrapper);
    }
}