#pragma once

namespace paperback::partition
{
    //-----------------------------------
    //             Defaults
    //-----------------------------------

    template < typename T_TYPE >
    PPB_FORCEINLINE
    range<T_TYPE>::range( const paperback::Vector3f& Min, const paperback::Vector3f& Max, const bool Unsigned ) noexcept :
        m_Min{ static_cast<T_TYPE>( Unsigned && std::floor(Min.x) < 0 ? 0 : std::floor(Min.x) )
             , static_cast<T_TYPE>( Unsigned && std::floor(Min.y) < 0 ? 0 : std::floor(Min.y) )
             , static_cast<T_TYPE>( Unsigned && std::floor(Min.z) < 0 ? 0 : std::floor(Min.z) )
             }
    ,   m_Max{ static_cast<T_TYPE>( Unsigned && std::ceil(Max.x) < 0 ? 0 : std::ceil(Max.x) )
             , static_cast<T_TYPE>( Unsigned && std::ceil(Max.y) < 0 ? 0 : std::ceil(Max.y) )
             , static_cast<T_TYPE>( Unsigned && std::ceil(Max.z) < 0 ? 0 : std::ceil(Max.z) )
             }
    { }



    //-----------------------------------
    //             Defaults
    //-----------------------------------

    PPB_INLINE
    void cell::Reset( void ) noexcept
    {
        m_EntityList.clear();
        m_EntityList.resize( 0 );

        m_Entities = tools::n_bits<>{};

        m_EntityCount = 0;
    }



    //-----------------------------------
    //             Defaults
    //-----------------------------------

    PPB_INLINE
    void spatial_hash_grid::Initialize( void ) noexcept
    {
        INFO_PRINT( "Initializing Hash Grid" );

		auto AssignSmaller = [&]( float& x, const float y ) { x = x < y ? x : y; };
		auto AssignLarger  = [&]( float& x, const float y ) { x = x > y ? x : y; };
		auto MakePositive  = [&]( float& x ) { x = x < 0 ? -x : x; };

        // Query For Static Map Boundaries
		tools::query Query;
		Query.m_Must.AddFromComponents<transform, boundingbox>();

        // Find Min And Max Boundaries
		m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( transform& Transform, boundingbox& Box ) noexcept
		{
			auto MinBoundary = Transform.m_Position + Box.Min;
			auto MaxBoundary = Transform.m_Position + Box.Max;

			AssignSmaller( m_Boundaries[0].x, MinBoundary.x );
			AssignSmaller( m_Boundaries[0].y, MinBoundary.y );
			AssignSmaller( m_Boundaries[0].z, MinBoundary.z );

			AssignLarger( m_Boundaries[1].x, MaxBoundary.x );
			AssignLarger( m_Boundaries[1].y, MaxBoundary.y );
			AssignLarger( m_Boundaries[1].z, MaxBoundary.z );
		});

        // Convert To Positive Bounds
		MakePositive( m_Boundaries[0].x );
		MakePositive( m_Boundaries[0].y );
		MakePositive( m_Boundaries[0].z );

		MakePositive( m_Boundaries[1].x );
		MakePositive( m_Boundaries[1].y );
		MakePositive( m_Boundaries[1].z );

        // Assign New Boundaries
		SetBoundaries( m_Boundaries[0], m_Boundaries[1] ); // Currently Not Using Bounds

        

		// Insert Entities Into Hash Grid
		tools::query HashQuery;
		HashQuery.m_Must.AddFromComponents<paperback::component::entity, transform, boundingbox, reference_prefab> ();

        m_Coordinator.ForEach( m_Coordinator.Search(HashQuery), [&]( paperback::component::entity& Entity, transform& Transform, boundingbox& Box ) noexcept
		{
			InsertUnit( Entity.m_GlobalIndex, Transform.m_Position, Box.Min, Box.Max );
		});
    }

    PPB_INLINE
    void spatial_hash_grid::Reset( void ) noexcept
    {
        INFO_PRINT( "Resetting Hash Grid" );

        m_Boundaries[0] = paperback::Vector3f{  FLT_MAX,  FLT_MAX,  FLT_MAX };
        m_Boundaries[1] = paperback::Vector3f{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

        for ( auto& Cell : m_HashedGrid )
            Cell.Reset();
    }


    //-----------------------------------
    //            Register
    //-----------------------------------

    // Returns Vector of GID
    PPB_INLINE
    std::vector<paperback::u32> spatial_hash_grid::SearchNeighbours( const paperback::Vector3f&  Position                       // Entity Position
                                                                   , const paperback::Vector3f&  MinScale                       // Area To Query - Min Offset
                                                                   , const paperback::Vector3f&  MaxScale                       // Area To Query - Max Offset
                                                                   , const float                 Multiplier ) const noexcept    // Area Multiplier
    {
        tools::n_bits<>               QueryStatus;    // Entity Added - Bit = True
        std::vector<paperback::u32>   QueryList;      // List Of Entity GIDs

        auto CellIndexes = ComputeCells( Position, MinScale + -m_CellDimension * Multiplier, MaxScale + m_CellDimension * Multiplier );

        // For All Valid Cells
        for ( const auto& Index : CellIndexes )
        {
            const auto& Cell = m_HashedGrid[ Index ];

            // Iterate Cell List
            for ( size_t i = 0, max = Cell.m_EntityCount; i < max; ++i )
            {
                const auto GID = Cell.m_EntityList[i];

                // Not In QueryList Yet
                if ( QueryStatus.None( GID ) )
                {
                    QueryList.push_back( GID );
                    QueryStatus.Set( GID );
                }
            }
        }

        return QueryList;
    }


    //-----------------------------------
    //            Register
    //-----------------------------------

    PPB_INLINE
    void spatial_hash_grid::InsertUnit( const paperback::u32        GlobalIndex                // Entity Global Index
                                      , const paperback::Vector3f&  Position                   // Entity Position
                                      , const paperback::Vector3f&  MinScale                   // Bounding Box Min
                                      , const paperback::Vector3f&  MaxScale ) noexcept        // Bounding Box Max
    {
        // Compute All Cells Entity Is To Be Inserted Into
        auto CellIndexes = ComputeCells( Position, MinScale, MaxScale );

        // For All Valid Cells
        for ( auto& Index : CellIndexes )
        {
            auto& Cell = m_HashedGrid[ Index ];

            // Append
            AppendEntityToCell( Cell, GlobalIndex );
        }
    }

    PPB_INLINE
    void spatial_hash_grid::UpdateUnit( const paperback::u32        GlobalIndex                // Entity Global Index
                                      , const paperback::Vector3f&  PrevPosition               // Prev Entity Position
                                      , const paperback::Vector3f&  CurrPosition               // Curr Entity Position
                                      , const paperback::Vector3f&  MinScale                   // Bounding Box Min
                                      , const paperback::Vector3f&  MaxScale ) noexcept        // Bounding Box Max
    {
        auto PrevCellIndexes = ComputeCells( PrevPosition, MinScale, MaxScale, true );
        auto CurrCellIndexes = ComputeCells( CurrPosition, MinScale, MaxScale, true );

        const auto PrevSize = PrevCellIndexes.size();
        const auto CurrSize = CurrCellIndexes.size();

        // Same Cell Counts
        if ( PrevSize == CurrSize )
        {
            for ( int i = 0; i < PrevSize; ++i )
            {
                // Exists in BOTH Cells - Continue
                if ( PrevCellIndexes[i] == CurrCellIndexes[i] )
                    continue;
                // Current & Previous Cell Are Different - Remove & Append
                else
                {
                    RemoveEntityFromCell( m_HashedGrid[ PrevCellIndexes[i] ], GlobalIndex );
                    AppendEntityToCell  ( m_HashedGrid[ CurrCellIndexes[i] ], GlobalIndex );
                }
            }
        }
        // Different Cell Counts
        else
        {
            for ( int i = 0; i < PrevSize; ++i )
                RemoveEntityFromCell( m_HashedGrid[ PrevCellIndexes[i] ], GlobalIndex );
        
            for ( int i = 0; i < CurrSize; ++i )
                AppendEntityToCell( m_HashedGrid[ CurrCellIndexes[i] ], GlobalIndex );
        }
    }

    PPB_INLINE
    void spatial_hash_grid::UpdateClonedUnit( const paperback::u32 GlobalIndex ) noexcept
    {
        const auto& Info = m_Coordinator.GetEntityInfo( GlobalIndex );
        auto [ Entity, Transform, Box, Prefab ] = Info.m_pArchetype->FindComponents< paperback::component::entity, transform, boundingbox, prefab>( Info.m_PoolDetails );

        if ( !Prefab && Entity && Transform && Box )
            UpdateUnit( Entity->m_GlobalIndex, { 0.0f, 0.0f, 0.0f }, Transform->m_Position, Box->Min, Box->Max );
    }


    //-----------------------------------
    //             Setters
    //-----------------------------------

    PPB_INLINE
    void spatial_hash_grid::SetBoundaries( const paperback::Vector3f& Min
                                         , const paperback::Vector3f& Max ) noexcept
    {
        //m_Boundaries[0] = Min;
        //m_Boundaries[1] = Max;

        // Just to prevent negative values when iterating cells
        m_Boundaries[0] = paperback::Vector3f{ 50, 50, 50 };
        m_Boundaries[1] = paperback::Vector3f{ 50, 50, 50 };
    }

    PPB_FORCEINLINE
    void spatial_hash_grid::SetCellSize( const paperback::u32 CellSize ) noexcept
    {
        m_CellSize      = CellSize;
        m_CellDimension = paperback::Vector3f{ static_cast<float>( CellSize ), static_cast<float>( CellSize ), static_cast<float>( CellSize ) };
        m_InvCellSize   = 1.0f / CellSize;
    }


    //-----------------------------------
    //             Compute
    //-----------------------------------

    std::vector<paperback::u32> spatial_hash_grid::ComputeCells( const paperback::Vector3f&  Position
                                                               , const paperback::Vector3f&  MinScale
                                                               , const paperback::Vector3f&  MaxScale
                                                               , const bool Sorted ) const noexcept
    {
        auto ZeroScale = [&]( const paperback::Vector3f& Vec ) -> bool
                         {
                             if ( Vec.x == 0.0f && Vec.y == 0.0f && Vec.z == 0.0f ) return true;
                             else                                                   return false;
                         };

        if ( ZeroScale( MinScale ) && ZeroScale( MaxScale ) ) return {};

        tools::n_bits<paperback::settings::max_partition_cells_v>  QueryStatus;
        std::vector<paperback::u32>                                GridList;

        // Compute Bounds
        auto MinBounds = Position + m_Boundaries[0] + MinScale;
        auto MaxBounds = Position + m_Boundaries[0] + MaxScale;

        // Floor & Ciel Unsigned Bounds
        range<int> Bounds{ MinBounds, MaxBounds, true };

        // Compute Cells within "Bounds" w/ CellSize
        BEGIN_FOR_EVERY_CELL( Bounds, m_CellSize )
            auto Cell = HashPosition( x, y, z );
            if ( QueryStatus.None( Cell ) )
            {
                GridList.push_back( Cell );
                QueryStatus.Set( Cell );
            }
        END_FOR_EVERY_CELL

        if ( Sorted ) std::sort( GridList.begin(), GridList.end() );

        return GridList;
    }

    PPB_INLINE
    paperback::u32 spatial_hash_grid::HashPosition( const int X
                                                  , const int Y
                                                  , const int Z ) const noexcept
    {
        auto x = static_cast<paperback::u32>( static_cast<paperback::u32>( X ) * m_InvCellSize );
		auto y = static_cast<paperback::u32>( static_cast<paperback::u32>( Y ) * m_InvCellSize );
		auto z = static_cast<paperback::u32>( static_cast<paperback::u32>( Z ) * m_InvCellSize );

        paperback::u64 BitList = x;                     // Assign "x" to Long - Lower 32 Bits
        BitList = BitList << 32ul;                      // Shift All "x" Bits - Upper 32 Bits
        BitList |= y;                                   // Assign "y" to Long - Lower 32 Bits
        BitList ^= ( z << 16ul );                       // XOR "z" - Middle 32 Bits

		// Compute Hash Value
        paperback::u64 Hash = std::hash<paperback::u64>{}( BitList );

        // Compute Hashed Grid Cell Index - Use w/ "m_HashedGrid"
        return ( Hash % settings::max_partition_cells_v );
    }

    PPB_INLINE
    void spatial_hash_grid::AppendEntityToCell( partition::cell& Cell
                                              , const paperback::u32 GlobalIndex ) noexcept
    {
        // Already Inserted ( Hashed To Same Grid ) - Return
        if ( Cell.m_Entities.Has( GlobalIndex ) ) return;

        // Update Cell Details
        Cell.m_EntityList.push_back( GlobalIndex );
        Cell.m_Entities.Set( GlobalIndex );
        ++Cell.m_EntityCount;
    }

    PPB_INLINE
    void spatial_hash_grid::RemoveEntityFromCell( partition::cell& Cell
                                                , const paperback::u32 GlobalIndex ) noexcept
    {
        // Already Removed ( Hashed To Same Grid ) - Return
        if ( Cell.m_Entities.None( GlobalIndex ) ) return;

        PPB_ASSERT_MSG( Cell.m_EntityCount <= 0
                      , "RemoveEntityInCell - Current Cell Contains Too Few Entities" );

        // Remove Entity From Cell
        for ( size_t i = 0, max = Cell.m_EntityCount; i < max; ++i )
        {
            if ( Cell.m_EntityList[ i ] == GlobalIndex )
            {
                // Not Removing Last Entity - Swap Deleted & Last
                if ( i < Cell.m_EntityCount - 1 ) Cell.m_EntityList[ i ] = Cell.m_EntityList[ --Cell.m_EntityCount ];
                // Removing Last - Update Counter
                else                            --Cell.m_EntityCount;

                // Update N-Bitset
                Cell.m_Entities.Remove( GlobalIndex );
                break;
            }
        }
    }
}