/**********************************************************************************
*\file         paperback_physics_aabbtree.h
*\brief        paperback_physics_aabbtree.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef PPB_PHYSICS_AABBTREE
#define PPB_PHYSICS_AABBTREE

namespace paperback::physics
{
    class AABB_Tree final
    {
    public:

        static constexpr auto TREE_CAPACITY = 2000;                         // Tree Capacity

        using NodeID        = paperback::u32;                               // Node Index
        using EntityGID     = paperback::u32;                               // Entity GID
        using SiblingList   = std::vector< NodeID >;                        // List of Sibling IDs
        using NeighbourList = std::vector< EntityGID >;                     // List of Entity GIDs
        using AABBTree      = std::vector< AABBTree_Node >;                 // AABB Tree
        using GIDToTreeMap  = std::unordered_map<EntityGID, NodeID >;       // Entity GID -> Tree ID Mapping

        //-----------------------------------
        //             Defaults
        //-----------------------------------

        PPB_INLINE
        AABB_Tree( coordinator::instance& Instance ) noexcept;

        PPB_INLINE
        void Initialize( void ) noexcept;

        PPB_INLINE
        void Reset( void ) noexcept;


        //-----------------------------------
        //              Tree
        //-----------------------------------

        PPB_INLINE
        NodeID InsertNode( const boundingbox& AABB
                         , const transform&   Transform
                         , component::entity  Entity ) noexcept;

        PPB_INLINE
        bool UpdateNode( const boundingbox& AABB
                       , const transform&   Transform
                       , component::entity  Entity ) noexcept;

        PPB_INLINE
        void RemoveNode( component::entity Entity ) noexcept;


        //-----------------------------------
        //              Query
        //-----------------------------------

        PPB_INLINE
        NeighbourList QueryNeighbours( const boundingbox&   AABB
                                     , const transform&     Transform
                                     , const float          Thickness ) noexcept;

        PPB_INLINE
        NeighbourList QueryRaycast( const paperback::Vector3f& RayStart
                                  , const paperback::Vector3f& RayEnd ) noexcept;
            
        PPB_INLINE // broken fuck
        std::tuple<EntityGID, float> QueryRaycastClosest( const paperback::Vector3f&   RayStart
                                                        , const paperback::Vector3f&   RayEnd
                                                        , std::span<EntityGID>         ExcludeList
                                                        , bool                         ExcludeBV = false ) noexcept;

        PPB_INLINE
        std::tuple<AABB_Tree::EntityGID, float> QueryMultipleRaycastClosest( std::span<std::pair<paperback::Vector3f, paperback::Vector3f>>  StartEndPairs                 // First = Ray Start  |  Second = Ray End
                                                                           , const paperback::component::entity&                             Entity                        // Relevant Entity
                                                                           , const transform&                                                Transform                     // Position That You Are Querying From
                                                                           , const float&                                                    QueryRadius                   // Radius To Query
                                                                           , std::span<EntityGID>                                            ExcludeList                   // Exclude List
                                                                           , bool                                                            ExcludeBV ) noexcept;         // Exclude List

        //-----------------------------------
        //              Debug
        //-----------------------------------

        PPB_INLINE
        paperback::i32 MaxBalanceFactor( void ) noexcept;


    private:


        //-----------------------------------
        //          Private - Tree
        //-----------------------------------

        PPB_INLINE
        NodeID AppendNode( void ) noexcept;                                 // Creates A New Node - Update Free List

        PPB_INLINE
        void ReleaseNode( NodeID ID ) noexcept;                             // Releases An Existing Node - Update Free List

        PPB_INLINE
        void InsertLeaf( NodeID ID ) noexcept;                              // Inserts A New Leaf Node ( Performs Balancing Upwards From Node To Root )

        PPB_INLINE
        bool RemoveLeaf( NodeID ID ) noexcept;                              // Releases An Existing Leaf Node ( Performs Balancing Upwards From Node To Root )

        PPB_INLINE
        void BalanceTree( NodeID ID ) noexcept;                             // Balances Tree From Node To Root

        PPB_INLINE
        NodeID BalanceNode( NodeID ID ) noexcept;                           // Balances Node - Called Within BalanceTree

        template < typename Condition
                 , typename Function >
        void TraverseTree( Condition Check
                         , Function  Func ) noexcept
        {
            SiblingList Stack;
            Stack.push_back( m_RootIndex );

            while ( !Stack.empty() )
            {
                // Get Last Index
                auto Index = Stack.back();
                Stack.pop_back();

                if ( Index == settings::invalid_index_v ) continue;

                auto& Node = m_Tree[ Index ];

                if ( Check( Node ) )
                {
                    if ( Node.IsLeaf() )
                    {
                        Func( Node );
                    }
                    else
                    {
                        Stack.push_back( Node.m_LeftIndex );
                        Stack.push_back( Node.m_RightIndex );
                    }
                }
            }
        }

        PPB_INLINE
        NodeID FindBestSibling( const boundingbox& AABB ) noexcept;         // Find Suitable Sibling Depending On AABB Coordinates & Size

        PPB_INLINE
        paperback::i32 ComputeTreeHeight( NodeID RootID ) noexcept;         // Compute Height - Determine Whether Node Is Balanced ( Height Difference < 2 )


        AABBTree                m_Tree;
        GIDToTreeMap            m_EntityToIndexMap;
        paperback::u32          m_NodeCount = 0;
        paperback::u32          m_RootIndex = settings::invalid_index_v;
        paperback::u32          m_FreeIndex = settings::invalid_index_v;
        coordinator::instance&  m_Coordinator;
    };
}

#endif