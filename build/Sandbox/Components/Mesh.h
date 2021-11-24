#pragma once

struct mesh
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Mesh"
	};

	std::string m_Model;
	std::string m_Texture;
	int m_Bias = 1;
};


namespace RR_Mesh
{
	RTTR_REGISTRATION
	{
		rttr::registration::class_<mesh>( mesh::typedef_v.m_pName)
			.constructor()( rttr::policy::ctor::as_object )
			.property( "Model", &mesh::m_Model )
			.property( "Texture", &mesh::m_Texture )
			.property( "Shadow Bias", &mesh::m_Bias );
	}
}