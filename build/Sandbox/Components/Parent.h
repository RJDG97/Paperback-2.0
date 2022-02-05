#pragma once
#ifndef PARENT_H
#define PARENT_H

struct parent
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Parent"
	};

    // Pass paperback::component::entity::m_GlobalIndex
    void AddChild( paperback::u32 ChildGlobalIndex ) noexcept
    {
        m_ChildrenGlobalIndexes.emplace( ChildGlobalIndex );
    }

    // Pass paperback::component::entity::m_GlobalIndex
    void RemoveChild( paperback::u32 ChildGlobalIndex ) noexcept
    {
        m_ChildrenGlobalIndexes.erase( ChildGlobalIndex );
    }

    bool HasChild( paperback::u32 ChildGlobalIndex ) noexcept
    {
        return m_ChildrenGlobalIndexes.contains( ChildGlobalIndex );
    }

	std::unordered_set<paperback::u32> m_ChildrenGlobalIndexes;
};

namespace RR_Parent
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<parent>( parent::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Children GID", &parent::m_ChildrenGlobalIndexes );
    }
}

#endif