#pragma once
#ifndef PPB_PHYSICS_NODE
#define PPB_PHYSICS_NODE

#include "../build/Sandbox/Components/BoundingBox.h"

namespace paperback::physics
{
    struct AABBTree_Node final
    {
        PPB_FORCEINLINE
        bool IsLeaf( void ) const noexcept
        {
            return ( m_Height == 0 ) ? true 
                                     : false;
        }

        PPB_INLINE
        bool IsIntersecting( const boundingbox& AABB
                           , const transform&   Transform ) noexcept
        {
            boundingbox Box = AABB.Extend( Transform.m_Position, 0.0f );
            return Box.Intersecting( m_AABB );
        }

        PPB_INLINE
        bool IsIntersecting( const boundingbox& AABB ) noexcept
        {
            return AABB.Intersecting( m_AABB );
        }

        PPB_INLINE
        void Reset( void ) noexcept
        {
            m_ParentIndex = settings::invalid_index_v;
            m_LeftIndex   = settings::invalid_index_v;
            m_RightIndex  = settings::invalid_index_v;
            m_AABB.Min    = paperback::Vector3f{ };
            m_AABB.Max    = paperback::Vector3f{ };
            m_Position    = paperback::Vector3f{ };
            m_Height      = 0;
        }

        union
        {
            struct
            {
                
                u32            m_LeftIndex;                                           // Left Child Index
                u32            m_RightIndex;                                          // Right Child Index
            };                             
            component::entity  m_Entity      = { };                                   // Entity - Leaf Node
        };
        union
        {
            u32                m_ParentIndex;                                         // Index Of Parent Node in AABB_Tree
            u32                m_NextIndex;                                           // Index Of Next Available Node in AABB_Tree - If Any
        };
        boundingbox            m_AABB        = { };                                   // Bounding Box Values
        paperback::Vector3f    m_Position    = { };                                   // Center Position Of AABB - Transform + Offsets
        int                    m_Height      = 0;                                     // Height from Root Node
    };
}

#endif