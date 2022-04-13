/**********************************************************************************
*\file         Name.h
*\brief        Name.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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