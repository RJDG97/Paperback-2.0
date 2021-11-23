#pragma once
#ifndef NAME_H
#define NAME_H

struct name
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Name"
	};

	std::string m_Value = "Unnamed Entity";
};

namespace RR_Name
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<name>( name::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Name", &name::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}

#endif