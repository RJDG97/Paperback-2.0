#pragma once

#include "paperback_bitset.h"

// this is based on the playable map
enum class CollisionLayer
{
    NONE = 0,
    BACKGROUND = 1,
    ENEMYBASE,
    PLAYERBASE,

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
// references

// Typedefs
// impt: m_bits = uint64_t

//using AABBType = std::unordered_map<EntityID, AABB*>;
//using CollidableLayer = tools::bits<static_cast<int>(CollisionLayer::NONE)>;
//using CollidableLayers = std::pair<CollidableLayer, bool>;

// Maps a collision layer to a bitset that tells the system which layers are collidable
//std::unordered_map<CollisionLayer, CollidableLayers> collision_layer_arr_;
// Saves a copy of all valid AABBs on each layer
//std::map<CollisionLayer, AABBType> collision_map_;

struct Collidable
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Collidable"
    };

    // current collision layer, u8 (unsigned) has a maximum of 255
    paperback::u8 m_CollisionLayer;
    // convert this to int so that no need static_cast

    // determine which layers are collidable with this AABB
    tools::bits  m_CollidableLayers;

    // default constructor
    Collidable() :
        m_CollisionLayer{ static_cast<paperback::u8>(0x00) } // 0x00 reads as int(0)....
    {
        m_CollidableLayers.Set(0);
    }

    // enum variant
    Collidable(CollisionLayer assigned) :
        m_CollisionLayer{ static_cast<paperback::u8>(assigned) }
    {
        m_CollidableLayers.Set(static_cast<int>(m_CollisionLayer));
    }

    // int variant
    Collidable(int assigned) :
        m_CollisionLayer{ static_cast<paperback::u8>(assigned) }
    {
        m_CollidableLayers.Set(static_cast<int>(m_CollisionLayer));
    }

    // u8 variant
    Collidable(paperback::u8 assigned) :
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