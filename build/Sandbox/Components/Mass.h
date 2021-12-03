#pragma once

#include "Math/Vector3f.h"

struct mass
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Mass"
    };

    float		m_Mass;
	float       m_InvMass;
};

namespace RR_Mass
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<mass>( mass::typedef_v.m_pName )
			.constructor()( rttr::policy::ctor::as_object )
			.property( "Mass", &mass::m_Mass )( rttr::policy::prop::as_reference_wrapper )
			.property( "Inv Mass", &mass::m_InvMass )( rttr::policy::prop::as_reference_wrapper );
	}
}