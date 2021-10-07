#pragma once



struct scale
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Scale"
	};

	xcore::vector3	 m_Value;							// -- Scale vector
};

namespace RR_SCALE
{
	RTTR_REGISTRATION
	{
	   rttr::registration::class_<scale>(scale::typedef_v.m_pName)
		   .constructor()(rttr::policy::ctor::as_object)
		   .property("Scale", &scale::m_Value)(rttr::policy::prop::as_reference_wrapper);
	}
}