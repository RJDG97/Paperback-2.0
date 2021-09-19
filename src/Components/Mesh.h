#pragma once

struct mesh
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "mesh"
	};

	std::string m_Model;
};