#pragma once

struct capture_point
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Capture Point"
	};

	bool m_Captured = false;
};

namespace RR_CapturePoint
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<capture_point>(capture_point::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Captured", &capture_point::m_Captured );
    }
}