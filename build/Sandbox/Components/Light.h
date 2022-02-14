#pragma once

struct light
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Light"
	};

	paperback::Vector3f m_Offset;
	paperback::Vector3f m_Ambient;
	paperback::Vector3f m_Diffuse;
	paperback::Vector3f m_Specular;
};

namespace RR_Light
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<light>(light::typedef_v.m_pName)
			.constructor()(rttr::policy::ctor::as_object)
			.property("Light Offset", &light::m_Offset)(rttr::policy::prop::as_reference_wrapper)
			.property("Ambient", &light::m_Ambient)(rttr::policy::prop::as_reference_wrapper)
			.property("Diffuse", &light::m_Diffuse)(rttr::policy::prop::as_reference_wrapper)
			.property("Specular", &light::m_Specular)(rttr::policy::prop::as_reference_wrapper);
	}
}