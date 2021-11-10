#pragma once

struct friendly
{
	constexpr static auto typedef_v = paperback::component::type::tag
	{
		.m_pName = "Friendly"
	};
};

namespace RR_Friendly
{
    RTTR_REGISTRATION
    {
        rttr::registration::class_<friendly>( friendly::typedef_v.m_pName )
            .constructor()( rttr::policy::ctor::as_object );
    }
}