#pragma once

struct sample_tag_component
{
    constexpr static auto typedef_v = paperback::component::type::tag
    {
        .m_pName = "rigidbody"
    };
};