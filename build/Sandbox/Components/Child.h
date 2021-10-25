#pragma once
#ifndef CHILD_H
#define CHILD_H

struct child
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Child"
	};

	union parent_info final
    {
        paperback::u64             m_ParentGID : 32          // Global Index of Parent Entity ( EntityInfo )
                      ,            m_Unused    : 32;
        paperback::u64             m_Unused_2;

        #pragma warning(disable : 4201)
            // Data used to initialize m_ParentGID
            struct
            {
                paperback::u64     m_ParentGuid;            // Parent Archetype Guid
                paperback::u64     m_ParentPoolKey   : 32   // Parent Pool Key
                             ,     m_ParentPoolIndex : 32;  // Parent Pool Index
            };
        #pragma warning(default : 4201)
    };

    // Pass paperback::component::entity::m_GlobalIndex
    void AddParent( paperback::u32 ParentGlobalIndex ) noexcept
    {
        m_Info.m_ParentGID = ParentGlobalIndex;
    }

	parent_info                    m_Info;
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