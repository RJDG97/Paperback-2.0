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
};

namespace RR_Rigidbody
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<rigidbody>(rigidbody::typedef_v.m_pName)
            .constructor()(rttr::policy::ctor::as_object)
            .property("Acceleration", &rigidbody::m_Accel)(rttr::policy::prop::as_reference_wrapper)
            .property("Velocity", &rigidbody::m_Velocity)(rttr::policy::prop::as_reference_wrapper);
    }
}