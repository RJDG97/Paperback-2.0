#pragma once
#ifndef PARENT_H
#define PARENT_H

struct parent
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Parent"
	};

	union child_info final
    {
        paperback::u64              m_ChildGID : 32          // Global Index of Child Entity ( EntityInfo )
                      ,             m_Unused   : 32;
        paperback::u64              m_Unused_2;

        #pragma warning(disable : 4201)
            // Data used to initialize m_ChildGID
            struct
            {
                paperback::u64     m_ChildGuid;             // Child Archetype Guid
                paperback::u64     m_ChildPoolKey   : 32    // Child Pool Key
                             ,     m_ChildPoolIndex : 32;   // Child Pool Index
            };
        #pragma warning(default : 4201)
    };

    // Pass paperback::component::entity::m_GlobalIndex
    void AddChild( paperback::u32 ChildGlobalIndex ) noexcept
    {
        bool Valid = true;
        for ( const auto CInfo : m_Infos )
        {
            if ( CInfo.m_ChildGID == ChildGlobalIndex )
            {
                Valid = false;
                break;
            }
        }

        if ( Valid )
            m_Infos.push_back({
                .m_ChildGID = ChildGlobalIndex
            });
    }

    // Pass paperback::component::entity::m_GlobalIndex
    void RemoveChild( paperback::u32 ChildGlobalIndex ) noexcept
    {
        auto begin = m_Infos.begin(), end = m_Infos.end();
        for ( ; begin != end; ++begin )
        {
            if ( begin->m_ChildGID == ChildGlobalIndex ) break;
        }
        if ( begin != end )
            m_Infos.erase( begin );
    }

	std::vector< child_info >      m_Infos;
};

//namespace RR_Parent
//{    
//    RTTR_REGISTRATION
//    {
//       rttr::registration::class_<parent>( parent::typedef_v.m_pName )
//		   .constructor()( rttr::policy::ctor::as_object )
//		   .property( "ChildGID", &parent::m_Infos )( rttr::policy::prop::as_reference_wrapper );
//    }
//}

#endif