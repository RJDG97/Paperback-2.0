#pragma once

// bool attached to an entity to specify which waypoint
// set to select to use for friendly units
struct selected
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Selected"
    };

    bool m_Value;
};

namespace RR_Selected
{    
    RTTR_REGISTRATION
    {
       rttr::registration::class_<selected>( selected::typedef_v.m_pName )
		   .constructor()( rttr::policy::ctor::as_object )
		   .property( "Selected", &selected::m_Value )( rttr::policy::prop::as_reference_wrapper );
    }
}