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
    paperback::Vector3f m_MaxMoment = paperback::Vector3f(8.f, 8.f, 8.f);
    paperback::Vector3f m_minthreshold = paperback::Vector3f(0.5f, 0.5f, 0.5f);
    float m_threshold = .25f;

    float m_staticFriction;							        // -- Friction when static
    float m_dynamicFriction;                                // -- Friction when moving
    paperback::Vector3f m_Forces;						    // -- Sum of Forces of rigidbody
    paperback::Vector3f m_Momentum;                         // -- Momentum of rigidbody
    float m_Restitution;                                    // -- 0.f to 1.f -> 0 = no bounce, 1 = bounce                                           //NEW
    //bool m_isStatic;

};


namespace RR_RIGIDFORCE
{
    RTTR_REGISTRATION
    {
       rttr::registration::class_<rigidforce>(rigidforce::typedef_v.m_pName)
           .constructor()(rttr::policy::ctor::as_object)
           //.property("Mass", &rigidforce::m_Mass)(rttr::policy::prop::as_reference_wrapper)
           //.property("Inverse Mass", &rigidforce::m_InvMass)(rttr::policy::prop::as_reference_wrapper)
           .property("Static Friction", &rigidforce::m_staticFriction)(rttr::policy::prop::as_reference_wrapper)
           .property("Dynamic Friction", &rigidforce::m_dynamicFriction)(rttr::policy::prop::as_reference_wrapper)
           .property("Restitution", &rigidforce::m_Restitution)(rttr::policy::prop::as_reference_wrapper)
           .property("Force", &rigidforce::m_Forces)(rttr::policy::prop::as_reference_wrapper)
           .property("Moment", &rigidforce::m_Momentum)(rttr::policy::prop::as_reference_wrapper)
           .property("Threshold", &rigidforce::m_threshold)(rttr::policy::prop::as_reference_wrapper);
    }
}