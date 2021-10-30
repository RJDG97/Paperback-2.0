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

//namespace RR_Child
//{    
//    RTTR_REGISTRATION
//    {
//       rttr::registration::class_<child>( child::typedef_v.m_pName )
//		   .constructor()( rttr::policy::ctor::as_object )
//		   .property( "ChildGID", &child::m_Info)( rttr::policy::prop::as_reference_wrapper );
//    }
//}

#endif