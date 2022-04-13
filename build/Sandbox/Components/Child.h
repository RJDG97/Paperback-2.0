/**********************************************************************************
*\file         Child.h
*\brief        Child.h
*
*\author       Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#ifndef CHILD_H
#define CHILD_H

struct child
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Child"
	};

    // Pass paperback::component::entity::m_GlobalIndex
    void AddParent( paperback::u32 ParentGlobalIndex ) noexcept
    {
        m_ParentGlobalIndex = ParentGlobalIndex;
    }

	paperback::u32 m_ParentGlobalIndex;
};

namespace RR_Child
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<child>( child::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Parent GID", &child::m_ParentGlobalIndex);
    }
}

#endif