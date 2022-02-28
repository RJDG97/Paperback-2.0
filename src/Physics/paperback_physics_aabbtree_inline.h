#pragma once
#ifndef PPB_PHYSICS_AABBTREE_INLINE
#define PPB_PHYSICS_AABBTREE_INLINE

namespace paperback::physics
{
    //-----------------------------------
    //             Defaults
    //-----------------------------------

    PPB_INLINE
    AABB_Tree::AABB_Tree( coordinator::instance& Instance ) noexcept :
        m_Coordinator{ Instance }
    {
        
    }

    PPB_INLINE
    void AABB_Tree::Initialize( void ) noexcept
    {
        // Query For All Bounding Boxes
		tools::query Query;
		Query.m_Must.AddFromComponents<component::entity, transform, boundingbox>( );
		Query.m_NoneOf.AddFromComponents<prefab>( );

        // Insert All Bounding Boxes Into Tree
		m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( component::entity& Entity
                                                                 , transform&         Transform
                                                                 , boundingbox&       AABB ) noexcept
		{
            InsertNode( AABB, Transform, Entity );
		});
    }

    PPB_INLINE
    void AABB_Tree::Reset( void ) noexcept
    {
        m_Tree.clear( );
        m_Tree.resize( TREE_CAPACITY );

        for ( size_t i = 0; i < TREE_CAPACITY - 1; ++i )
        {
            m_Tree[ i ].m_Height = -1;
            m_Tree[ i ].m_NextIndex = static_cast< NodeID >( i + 1 );
        }

        m_Tree[ TREE_CAPACITY - 1 ].m_Height = -1;
        m_Tree[ TREE_CAPACITY - 1 ].m_NextIndex = settings::invalid_index_v;

        m_FreeIndex = 0;
        m_RootIndex = settings::invalid_index_v;
    }


    //-----------------------------------
    //              Tree
    //-----------------------------------

    PPB_INLINE
    AABB_Tree::NodeID AABB_Tree::InsertNode( const boundingbox& AABB
                                           , const transform&   Transform
                                           , component::entity  Entity ) noexcept
    {
        // Ensure No Double Insertion
        auto NodeIT = m_EntityToIndexMap.find( Entity.m_GlobalIndex );

        PPB_ASSERT_MSG( NodeIT != m_EntityToIndexMap.end()
                      , "Insert Node (ERROR): Node Already Exists" );

        // Append Node
        auto  Index = AppendNode( );
        auto& Node  = m_Tree[ Index ];

        // Update Node
        Node.m_AABB = AABB.Extend( Transform.m_Position, 0.5f );
        Node.m_Position = Node.m_AABB.ComputeCentre( );
        Node.m_Height = 0;

        // Insert Node As Leaf
        InsertLeaf( Index );

        // Append Mapping For Quick Access
        m_EntityToIndexMap.emplace( Entity.m_GlobalIndex, Index );

        Node.m_Entity = Entity;

        return Index;
    }

    PPB_INLINE
    bool AABB_Tree::UpdateNode( const boundingbox& AABB
                              , const transform&   Transform
                              , component::entity  Entity ) noexcept
    {
        // verify that aabb values not overwriten when u remove and insert

        auto NodeIT = m_EntityToIndexMap.find( Entity.m_GlobalIndex );

        PPB_ASSERT_MSG( NodeIT == m_EntityToIndexMap.end()
                      , "Update Node (ERROR): Node Does Not Exist" );

        auto NodeIndex = NodeIT->second;

        PPB_ASSERT_MSG( NodeIndex >= m_Tree.size() 
                      , "Update Node (ERROR): Invalid Node Index" );
        PPB_ASSERT_MSG( !m_Tree[NodeIndex].IsLeaf()
                      , "Update Node (ERROR): Not Updating Leaf Node" );

        auto& Node = m_Tree[ NodeIndex ];

        PPB_ASSERT_MSG( Node.m_Entity != Entity
                      , "Update Node (ERROR): Entity GIDs Are Different" );

        // Entity Has Moved Out Of Bounds - Update Node
        if ( !Node.m_AABB.Contains( Transform.m_Position, AABB ) )
        {
            RemoveLeaf( NodeIndex );
            Node.m_AABB = AABB.Extend( Transform.m_Position, 0.5f );
            Node.m_Position = Node.m_AABB.ComputeCentre( );
            InsertLeaf( NodeIndex );

            return true;
        }

        return false;
    }

    PPB_INLINE
    void AABB_Tree::RemoveNode( component::entity Entity ) noexcept
    {
        auto NodeIT = m_EntityToIndexMap.find( Entity.m_GlobalIndex );

        PPB_ASSERT_MSG( NodeIT == m_EntityToIndexMap.end()
                      , "Remove Node (ERROR): Node Does Not Exist" );

        auto NodeIndex = NodeIT->second;

        PPB_ASSERT_MSG( NodeIndex >= m_Tree.size() 
                      , "Remove Node (ERROR): Invalid Node Index" );
        PPB_ASSERT_MSG( !m_Tree[NodeIndex].IsLeaf()
                      , "Remove Node (ERROR): Not Removing Leaf Node" );

        // Remove Node
        RemoveLeaf( NodeIndex );
        ReleaseNode( NodeIndex );
        m_EntityToIndexMap.erase( NodeIT );
    }


    //-----------------------------------
    //              Query
    //-----------------------------------

    PPB_INLINE
    AABB_Tree::NeighbourList AABB_Tree::QueryNeighbours( const boundingbox& AABB
                                                       , const transform&   Transform ) noexcept
    {
        NeighbourList List;
        boundingbox   Box = AABB.Extend( Transform.m_Position
                                       , 0.0f );

        TraverseTree( [&]( AABBTree_Node& Node ) -> bool
                      {
                          return Box.Intersecting( Node.m_AABB );
                      }
                    , [&]( AABBTree_Node& Node )
                      {
                          List.push_back( Node.m_Entity.m_GlobalIndex );
                      });

        return List;
    }

    PPB_INLINE
    AABB_Tree::NeighbourList AABB_Tree::QueryRaycast( const paperback::Vector3f& RayStart
                                                    , const paperback::Vector3f& RayEnd ) noexcept
    {
        NeighbourList List;

        TraverseTree( [&]( AABBTree_Node& Node ) -> bool
                      {
                          auto [ Status, Distance ] = Node.m_AABB.Intersecting( RayStart
                                                                              , RayEnd );
                          return Status;
                      }
                    , [&]( AABBTree_Node& Node )
                      {
                          List.push_back( Node.m_Entity.m_GlobalIndex );
                      });

        return List;
    }

    PPB_INLINE
    AABB_Tree::EntityGID AABB_Tree::QueryRaycastClosest( const paperback::Vector3f& RayStart
                                                       , const paperback::Vector3f& RayEnd ) noexcept
    {
        std::pair<EntityGID, float> CurrentPair = std::make_pair( settings::invalid_index_v, 0.0f );
        std::pair<EntityGID, float> ClosestPair = std::make_pair( settings::invalid_index_v, FLT_MAX );

        TraverseTree( [&]( AABBTree_Node& Node ) -> bool
                      {
                          auto [ Status, Distance ] = Node.m_AABB.Intersecting( RayStart
                                                                              , RayEnd );
                          if ( Status ) CurrentPair = std::make_pair( Node.m_Entity.m_GlobalIndex, Distance );
                          return Status;
                      }
                    , [&]( AABBTree_Node& Node )
                      {
                          if ( CurrentPair.second < ClosestPair.second )
                          {
                              ClosestPair.first = CurrentPair.first;
                              ClosestPair.second = CurrentPair.second;
                          }
                      });

        return ClosestPair.first;
    }


    //-----------------------------------
    //              Debug
    //-----------------------------------

    PPB_INLINE
    paperback::i32 AABB_Tree::MaxBalanceFactor( void ) noexcept
    {
        
    }








    //-----------------------------------
    //          Private - Tree
    //-----------------------------------

    PPB_INLINE
    AABB_Tree::NodeID AABB_Tree::AppendNode( void ) noexcept
    {
        // All Previous Indexes Are Filled
        if ( m_FreeIndex == settings::invalid_index_v )
        {
            ++m_NodeCount;

            std::cout << "RIP Appended" << std::endl;

            return settings::invalid_index_v;    // Return Next Available Index
        }
        // Some Previous Empty Indexes
        else
        {
            ++m_NodeCount;
            auto  ID   = m_FreeIndex;
            PPB_ASSERT_MSG( ID >= m_Tree.size()
                          , "Free Index Is Invalid - Too Large" );
            auto& Node = m_Tree[ ID ];

            m_FreeIndex = Node.m_NextIndex;      // Reassign Free Index
            Node.Reset();                        // Reset Old Variables

            std::cout << "Appended: " << ID << std::endl;

            return ID;                           // Return Free Index
        }
    }

    PPB_INLINE
    void AABB_Tree::ReleaseNode( NodeID ID ) noexcept
    {
        PPB_ASSERT( ID >= m_Tree.size() );

        --m_NodeCount;
        m_Tree[ ID ].m_NextIndex = m_FreeIndex;
        m_Tree[ ID ].m_Height = -1;
        m_FreeIndex = ID;
    }

    PPB_INLINE
    void AABB_Tree::InsertLeaf( NodeID ID ) noexcept
    {
        // Inserting As Root Node
        if ( m_RootIndex == settings::invalid_index_v )
        {
            m_RootIndex = ID;                                          // Assign New Root Index
            m_Tree[ ID ].m_ParentIndex = settings::invalid_index_v;    // Set Parent Index Of Root To NULL
            return;
        }
        // Not Inserting As Root Node
        else
        {
            auto& LeafNode      = m_Tree[ ID ];
            auto  SiblingIndex  = FindBestSibling( LeafNode.m_AABB );

            // Create New Parent
            NodeID OldParentIndex = m_Tree[ SiblingIndex ].m_ParentIndex;
            NodeID NewParentIndex = AppendNode( );

            // Node References
            auto& SiblingNode   = m_Tree[ SiblingIndex ];
            auto& NewParentNode = m_Tree[ NewParentIndex ];

            // Update New Parent
            NewParentNode.m_ParentIndex = OldParentIndex;
            NewParentNode.m_Height      = SiblingNode.m_Height + 1;
            NewParentNode.m_AABB.Merge( LeafNode.m_AABB, SiblingNode.m_AABB );

            // If Sibling Was Root Node
            if ( OldParentIndex == settings::invalid_index_v )
            {
                m_RootIndex = NewParentIndex;
            }
            // Not Root Node
            else
            {
                auto& OldParentNode = m_Tree[ OldParentIndex ];

                if ( OldParentNode.m_LeftIndex == SiblingIndex ) OldParentNode.m_LeftIndex  = NewParentIndex;
                else                                             OldParentNode.m_RightIndex = NewParentIndex;
            }

            // Update Mappings
            NewParentNode.m_LeftIndex  = SiblingIndex;
            NewParentNode.m_RightIndex = ID;
            SiblingNode.m_ParentIndex  = NewParentIndex;
            LeafNode.m_ParentIndex     = NewParentIndex;

            // Balance Tree - New Parent Node --(UPWARDS)-> Root Node
            BalanceTree( LeafNode.m_ParentIndex );
        }
    }

    PPB_INLINE
    void AABB_Tree::RemoveLeaf( NodeID ID ) noexcept
    {
        // Attempting To Remove Root Node
        if ( m_RootIndex == ID )
        {
            m_RootIndex = settings::invalid_index_v;
            return;
        }
        // Removing Other Leaf Nodes
        else
        {
            auto ParentIndex    = m_Tree[ ID ].m_ParentIndex;
            auto GParentIndex   = m_Tree[ ParentIndex ].m_ParentIndex;
            NodeID SiblingIndex = m_Tree[ ParentIndex ].m_LeftIndex == ID ? m_Tree[ ParentIndex ].m_RightIndex
                                                                          : m_Tree[ ParentIndex ].m_LeftIndex;

            // Parent Is Root - Remove Parent & Set Sibling As New Root
            if ( GParentIndex == settings::invalid_index_v )
            {
                m_RootIndex = SiblingIndex;
                m_Tree[ SiblingIndex ].m_ParentIndex = settings::invalid_index_v;
                ReleaseNode( ParentIndex );
            }
            // Remove Parent Node & Connect Sibling To GrandParent
            else
            {
                if ( m_Tree[ GParentIndex ].m_LeftIndex == ParentIndex )  m_Tree[ GParentIndex ].m_LeftIndex  = SiblingIndex;
                else                                                      m_Tree[ GParentIndex ].m_RightIndex = SiblingIndex;

                m_Tree[ SiblingIndex ].m_ParentIndex = GParentIndex;
                ReleaseNode( ParentIndex );

                // Balance Ancestor - Continue Balancing Till Root
                auto AncestorIndex = GParentIndex;

                while ( AncestorIndex != settings::invalid_index_v )
                {
                    AncestorIndex        = BalanceNode( AncestorIndex );
                    auto& AncestorNode   = m_Tree[ AncestorIndex ];

                    auto& LeftChildNode  = m_Tree[ AncestorNode.m_LeftIndex ];
                    auto& RightChildNode = m_Tree[ AncestorNode.m_RightIndex ];

                    AncestorNode.m_AABB.Merge( LeftChildNode.m_AABB, RightChildNode.m_AABB );
                    AncestorNode.m_Height = std::max( LeftChildNode.m_Height, RightChildNode.m_Height ) + 1;

                    AncestorIndex = AncestorNode.m_ParentIndex;
                }
            }
        }
    }

    PPB_INLINE
    void AABB_Tree::BalanceTree( NodeID ID ) noexcept
    {
        // While Not Root Node
        while ( ID != settings::invalid_index_v )
        {
            ID = BalanceNode( ID );
            auto& Node = m_Tree[ ID ];

            NodeID LeftIndex  = Node.m_LeftIndex;
            NodeID RightIndex = Node.m_RightIndex;

            PPB_ASSERT( LeftIndex == settings::invalid_index_v ||
                        RightIndex == settings::invalid_index_v );

            auto& LeftNode  = m_Tree[ LeftIndex ];
            auto& RightNode = m_Tree[ RightIndex ];

            Node.m_Height = std::max( LeftNode.m_Height, RightNode.m_Height ) + 1;
            Node.m_AABB.Merge( LeftNode.m_AABB, RightNode.m_AABB );
            Node.m_Position = Node.m_AABB.ComputeCentre( );

            ID = Node.m_ParentIndex;
        }
    }

    PPB_INLINE
    AABB_Tree::NodeID AABB_Tree::BalanceNode( NodeID ID ) noexcept
    {
        PPB_ASSERT( ID == settings::invalid_index_v );

        /*
                            Node
                      /              \
                  L_Node            R_Node
                 /      \          /      \
             LL_Node  LR_Node  RL_Node  RR_Node

                             aka

                             ID
                          /      \
                         L        R
                       /   \    /   \
                      LL   LR  RL   RR
        */

        auto& Node = m_Tree[ ID ];
        auto  L_ID = Node.m_LeftIndex;
        auto  R_ID = Node.m_RightIndex;

        // Leaf Node / Already Balanced
        if ( Node.IsLeaf() || Node.m_Height < 2 )
        {
            return ID;
        }
        // Gotta Balance Dat Tree
        else
        {
            PPB_ASSERT_MSG( L_ID >= m_Tree.size() ||
                            R_ID >= m_Tree.size()
                          , "Invalid Left / Right Node Index - Too Big!" );

            auto& LeftNode  = m_Tree[ L_ID ];                   // Left Child
            auto& RightNode = m_Tree[ R_ID ];                   // Right Child

            int BalanceFactor = RightNode.m_Height - LeftNode.m_Height;     // Balance Factor

            // Skewed Right - Rotate Left
            if ( BalanceFactor > 1 )
            {
                PPB_ASSERT( RightNode.IsLeaf() );

                auto RL_ID    = RightNode.m_LeftIndex;
                auto RR_ID    = RightNode.m_RightIndex;
                auto& RL_Node = m_Tree[ RL_ID ];
                auto& RR_Node = m_Tree[ RR_ID ];

                PPB_ASSERT_MSG( RL_ID >= m_Tree.size() ||
                                RR_ID >= m_Tree.size()
                              , "Invalid Right Left / Right Right Node Index - Too Big!" );

                // Swap Original Node & Right Child
                RightNode.m_LeftIndex = ID;
                RightNode.m_ParentIndex = Node.m_ParentIndex;
                Node.m_ParentIndex = R_ID;

                // Swap Original Node's Parent (If Valid) To Point To Right Child
                if ( RightNode.m_ParentIndex != settings::invalid_index_v )
                {
                    auto& RightParentNode = m_Tree[ RightNode.m_ParentIndex ];

                    if ( RightParentNode.m_LeftIndex == ID )
                    {
                        RightParentNode.m_LeftIndex = R_ID;
                    }
                    else
                    {
                        PPB_ASSERT_MSG( RightParentNode.m_RightIndex != ID
                                      , "Re-Linking Problems" );
                        RightParentNode.m_RightIndex = R_ID;
                    }
                }
                // Update Root - Right Node's Parent Is Root
                else
                {
                    m_RootIndex = R_ID;
                }

                // Skewed Left - Rotate Right
                if ( RL_Node.m_Height > RR_Node.m_Height )
                {
                    RightNode.m_RightIndex = RL_ID;
                    Node.m_RightIndex      = RR_ID;
                    RR_Node.m_ParentIndex  = ID;
                    Node.m_AABB.Merge( LeftNode.m_AABB, RR_Node.m_AABB );
                    RightNode.m_AABB.Merge( Node.m_AABB, RL_Node.m_AABB );

                    Node.m_Height      = std::max( LeftNode.m_Height, RR_Node.m_Height ) + 1;
                    RightNode.m_Height = std::max( Node.m_Height, RL_Node.m_Height ) + 1;
                }
                // Skewed Right - Rotate Left
                else
                {
                    RightNode.m_RightIndex = RR_ID;
                    Node.m_RightIndex      = RL_ID;
                    RL_Node.m_ParentIndex  = ID;
                    Node.m_AABB.Merge( LeftNode.m_AABB, RL_Node.m_AABB );
                    RightNode.m_AABB.Merge( Node.m_AABB, RR_Node.m_AABB );

                    Node.m_Height      = std::max( LeftNode.m_Height, RL_Node.m_Height ) + 1;
                    RightNode.m_Height = std::max( Node.m_Height, RR_Node.m_Height ) + 1;
                }

                return R_ID;
            }
            // Skewed Left - Rotate Right
            else if ( BalanceFactor < -1 )
            {
                auto LL_ID    = LeftNode.m_LeftIndex;
                auto LR_ID    = LeftNode.m_RightIndex;
                auto& LL_Node = m_Tree[ LL_ID ];
                auto& LR_Node = m_Tree[ LR_ID ];

                PPB_ASSERT_MSG( LL_ID >= m_Tree.size() ||
                                LR_ID >= m_Tree.size()
                              , "Invalid Right Left / Right Right Node Index - Too Big!" );

                // Swap Original Node & Left Child
                LeftNode.m_LeftIndex   = ID;
                LeftNode.m_ParentIndex = Node.m_ParentIndex;
                Node.m_ParentIndex     = L_ID;

                // Swap Original Node's Parent (If Valid) To Point To Left Child
                if ( LeftNode.m_ParentIndex != settings::invalid_index_v )
                {
                    auto& LeftParentNode = m_Tree[ LeftNode.m_ParentIndex ];

                    if ( LeftParentNode.m_LeftIndex == ID )
                    {
                        LeftParentNode.m_LeftIndex = L_ID;
                    }
                    else
                    {
                        PPB_ASSERT_MSG( LeftParentNode.m_RightIndex != ID
                                      , "Re-Linking Problems" );
                        LeftParentNode.m_RightIndex = L_ID;
                    }
                }
                // Update Root - Left Node's Parent Is Root
                else
                {
                    m_RootIndex = L_ID;
                }

                // Skewed Left - Rotate Right
                if ( LL_Node.m_Height > LR_Node.m_Height )
                {
                    LeftNode.m_RightIndex = LL_ID;
                    Node.m_LeftIndex      = LR_ID;
                    LR_Node.m_ParentIndex = ID;
                    Node.m_AABB.Merge( RightNode.m_AABB, LR_Node.m_AABB );
                    LeftNode.m_AABB.Merge( Node.m_AABB, LL_Node.m_AABB );

                    Node.m_Height     = std::max( RightNode.m_Height, LR_Node.m_Height ) + 1;
                    LeftNode.m_Height = std::max( Node.m_Height, LL_Node.m_Height ) + 1;
                }
                // Skewed Right - Rotate Left
                else
                {
                    LeftNode.m_RightIndex = LR_ID;
                    Node.m_LeftIndex      = LL_ID;
                    LL_Node.m_ParentIndex = ID;
                    Node.m_AABB.Merge( RightNode.m_AABB, m_Tree[ LL_ID ].m_AABB );
                    LeftNode.m_AABB.Merge( Node.m_AABB, m_Tree[ LR_ID ].m_AABB );

                    Node.m_Height     = std::max( RightNode.m_Height, LL_Node.m_Height ) + 1;
                    LeftNode.m_Height = std::max( Node.m_Height, LR_Node.m_Height ) + 1;
                }

                return L_ID;
            }

            return ID;
        }
    }

    PPB_INLINE
    AABB_Tree::NodeID AABB_Tree::FindBestSibling( const boundingbox& AABB ) noexcept
    {
        // Begin Query From Root
        NodeID Index = m_RootIndex;

        while ( !m_Tree[ Index ].IsLeaf() )
        {
            auto& Node = m_Tree[ Index ];

            boundingbox MergedAABB;
            MergedAABB.Merge( AABB, Node.m_AABB );
            auto MergedVol = MergedAABB.Volume();

            auto Cost            = 2.0f * MergedVol;
            auto InheritanceCost = 2.0f * ( MergedVol - Node.m_AABB.Volume() );

            // Cost To Descend Left
            auto& LeftNode = m_Tree[ Node.m_LeftIndex ];
            MergedAABB.Merge( AABB, LeftNode.m_AABB );
            auto LeftCost  = LeftNode.IsLeaf() ?  MergedAABB.Volume() + InheritanceCost                                       // Cost For Left Leaf Node
                                              : ( MergedAABB.Volume() - LeftNode.m_AABB.Volume() ) + InheritanceCost;         // Cost For Left Node

            // Cost To Descend Right
            auto& RightNode = m_Tree[ Node.m_RightIndex ];
            MergedAABB.Merge( AABB, RightNode.m_AABB );
            auto RightCost  = RightNode.IsLeaf() ?  MergedAABB.Volume() + InheritanceCost                                     // Cost For Right Leaf Node
                                                : ( MergedAABB.Volume() - RightNode.m_AABB.Volume() ) + InheritanceCost;      // Cost For Left Node

            // Already Descended To Correct Depth
            if (( Cost < LeftCost ) && ( Cost < RightCost )) break;
            
            // Descend Further Towards Side w/ Lower Cost
            if   ( LeftCost < RightCost ) Index = Node.m_LeftIndex;
            else                          Index = Node.m_RightIndex;
        }

        return Index;
    }

    PPB_INLINE
    paperback::i32 AABB_Tree::ComputeTreeHeight( NodeID RootID ) noexcept
    {
        
    }
}

#endif