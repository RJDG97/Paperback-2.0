/**********************************************************************************
*\file         CollisionLayer.h
*\brief        CollisionLayer.h
*
*\author       Bryan Low, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "paperback_bitset.h"

// this is based on the playable map
enum class CollisionLayer : paperback::u8
{
    NONE = 0,
    BACKGROUND = 1,
    ENEMYBASE,
    PLAYERBASE,
    UNIT,

    ROUTE_CENTER_1,
    ROUTE1_CENTER_2,
    ROUTE1_CENTER_3,

    ROUTE2_SIDE_1,
    ROUTE2_SIDE_2,
    ROUTE2_SIDE_3,

    ROUTE3_COMPLEX_1,
    ROUTE3_COMPLEX_2,
    ROUTE3_COMPLEX_3,
    ROUTE3_COMPLEX_4,
    ROUTE3_COMPLEX_5,

    // extras
    INTERACTABLE,
    HOLE
};


struct collidable
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Collidable"
    };

    // current collision layer, u8 (unsigned) has a maximum of 255
    paperback::u8 m_CollisionLayer;

    // determine which layers are collidable with this AABB
    tools::bits  m_CollidableLayers;

    // default constructor
    collidable() :
        m_CollisionLayer{ static_cast<paperback::u8>(0x00) } // 0x00 reads as int(0)....
    {
        m_CollidableLayers.Set(0);
    }

    // enum variant
    collidable(CollisionLayer assigned) :
        m_CollisionLayer{ static_cast<paperback::u8>(assigned) }
    {
        m_CollidableLayers.Set(static_cast<int>(m_CollisionLayer));
    }

    // int variant
    collidable(int assigned) :
        m_CollisionLayer{ static_cast<paperback::u8>(assigned) }
    {
        m_CollidableLayers.Set(static_cast<int>(m_CollisionLayer));
    }

    // u8 variant
    collidable(paperback::u8 assigned) :
        m_CollisionLayer{ static_cast<paperback::u8>(assigned) }
    {
        m_CollidableLayers.Set(static_cast<int>(m_CollisionLayer));
    }

    // set for testing purposes
    void Set(CollisionLayer assigned = CollisionLayer::NONE)
    {
        m_CollisionLayer = static_cast<paperback::u8>(assigned);
        m_CollidableLayers.Set(static_cast<int>(assigned));
    }

    void Set(int assigned = 0)
    {
        m_CollisionLayer = static_cast<paperback::u8>(assigned);
        m_CollidableLayers.Set(assigned);
    }

    void Set(paperback::u8 assigned = static_cast<paperback::u8>(0x00))
    {
        m_CollisionLayer = assigned;
        m_CollidableLayers.Set(static_cast<int>(assigned));
    }
};

namespace RR_Collidable
{
    RTTR_REGISTRATION
    {
       rttr::registration::class_<collidable>(collidable::typedef_v.m_pName)
           .constructor()(rttr::policy::ctor::as_object)
           .property("Collision Layer", &collidable::m_CollisionLayer)(rttr::policy::prop::as_reference_wrapper)
           .property("Collidable Layers", &collidable::m_CollidableLayers)(rttr::policy::prop::as_reference_wrapper);
    }
}