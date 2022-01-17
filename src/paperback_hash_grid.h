#pragma once

#include "Math/Vector3f.h"

#define BEGIN_FOR_EVERY_CELL( Range, GridSize )                                                                                                                                    \
    for ( int x = Range.m_Min.x; x < Range.m_Max.x || ( x > Range.m_Max.x && x - GridSize < Range.m_Max.x ); x += GridSize, x = x > Range.m_Max.x ? Range.m_Max.x : x )            \
    {                                                                                                                                                                              \
    	for ( int y = Range.m_Min.y; y < Range.m_Max.y || ( y > Range.m_Max.y && y - GridSize < Range.m_Max.y ); y += GridSize, y = y > Range.m_Max.y ? Range.m_Max.y : y )        \
    	{                                                                                                                                                                          \
    		for ( int z = Range.m_Min.z; z < Range.m_Max.z || ( z > Range.m_Max.z && z - GridSize < Range.m_Max.z ); z += GridSize, z = z > Range.m_Max.z ? Range.m_Max.z : z )    \
    		{
                // All Internal Code Goes Here
                #define END_FOR_EVERY_CELL                                                                                                                                         \
    		}                                                                                                                                                                      \
    	}                                                                                                                                                                          \
    }                                                                                                                                                                              \
    

PPB_INLINE
void PrintVector( std::string Name, paperback::Vector3f Vector )
{
    Name  += ": x: " + std::to_string( Vector.x )
          + " | y: " + std::to_string( Vector.y )
          + " | z: " + std::to_string( Vector.z );

    TRACE_PRINT( Name.c_str() );
}


namespace paperback::partition
{
    template < typename T_TYPE >
    struct vec3
    {
        T_TYPE x
             , y
             , z;
    };



    template < typename T_TYPE >
    struct range
    {
        //-----------------------------------
        //             Defaults
        //-----------------------------------

        PPB_FORCEINLINE
        range( void ) noexcept = default;

        PPB_FORCEINLINE
        range( const paperback::Vector3f&, const paperback::Vector3f&, const bool Unsigned = false ) noexcept;


        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //            Variables
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        vec3<T_TYPE> m_Min { }
                   , m_Max { };
    };



    struct cell
    {
        PPB_INLINE
        void Reset( void ) noexcept;

        std::vector<paperback::u32> m_EntityList;           // List of Global Index of Entities within Cell
        tools::n_bits<>             m_Entities;             // Bits to track Entity Presence
        paperback::u32              m_EntityCount;          // For Swapping On Removal
    };



    class spatial_hash_grid
    {
    public:

        //-----------------------------------
        //             Defaults
        //-----------------------------------

        PPB_FORCEINLINE
        spatial_hash_grid( paperback::coordinator::instance& Coordinator
                         , const paperback::u32 CellSize = paperback::settings::partition_cell_size_v ) noexcept :
            m_Coordinator{ Coordinator }
        ,   m_CellDimension{ paperback::Vector3f{ static_cast<float>( CellSize ), static_cast<float>( CellSize ), static_cast<float>( CellSize ) } }
        ,   m_CellSize{ CellSize }
        ,   m_InvCellSize{ 1.0f / CellSize }
        { };

        PPB_INLINE
        void Initialize( void ) noexcept;

        PPB_INLINE
        void Reset( void ) noexcept;


        //-----------------------------------
        //            Register
        //-----------------------------------

        // Returns Vector of GID
        PPB_INLINE
        std::vector<paperback::u32> SearchNeighbours( const paperback::Vector3f&  Position                              // Entity Position
                                                    , const paperback::Vector3f&  MinScale                              // Area To Query - Min Offset
                                                    , const paperback::Vector3f&  MaxScale                              // Area To Query - Max Offset
                                                    , const float                 Multiplier = 1.5f ) const noexcept;   // Area Multiplier


        //-----------------------------------
        //            Register
        //-----------------------------------

        PPB_INLINE
        void InsertUnit( const paperback::u32        GlobalIndex                // Entity Global Index
                       , const paperback::Vector3f&  Position                   // Entity Position
                       , const paperback::Vector3f&  MinScale                   // Bounding Box Min
                       , const paperback::Vector3f&  MaxScale ) noexcept;       // Bounding Box Max

        PPB_INLINE
        void UpdateUnit( const paperback::u32        GlobalIndex                // Entity Global Index
                       , const paperback::Vector3f&  PrevPosition               // Prev Entity Position
                       , const paperback::Vector3f&  CurrPosition               // Curr Entity Position
                       , const paperback::Vector3f&  MinScale                   // Bounding Box Min
                       , const paperback::Vector3f&  MaxScale ) noexcept;       // Bounding Box Max

        PPB_INLINE
        void UpdateClonedUnit( const paperback::u32 GlobalIndex ) noexcept;     // Entity Global Index


        //-----------------------------------
        //             Setters
        //-----------------------------------

        PPB_INLINE
        void SetBoundaries( const paperback::Vector3f& Min
                          , const paperback::Vector3f& Max ) noexcept;

        PPB_FORCEINLINE
        void SetCellSize( const paperback::u32 CellSize = paperback::settings::partition_cell_size_v ) noexcept;

    private:

        //-----------------------------------
        //             Compute
        //-----------------------------------

        PPB_INLINE
        std::vector<paperback::u32> ComputeCells( const paperback::Vector3f&  Position
                                                , const paperback::Vector3f&  MinScale
                                                , const paperback::Vector3f&  MaxScale
                                                , const bool                  Sorted = false ) const noexcept;

        PPB_INLINE
        paperback::u32 HashPosition( const int X
                                   , const int Y
                                   , const int Z ) const noexcept;

        PPB_INLINE
        void AppendEntityToCell( partition::cell& Cell
                               , const paperback::u32 GlobalIndex ) noexcept;

        PPB_INLINE
        void RemoveEntityFromCell( partition::cell& Cell
                                 , const paperback::u32 GlobalIndex ) noexcept;


        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //            Variables
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        std::array< paperback::Vector3f, 2 >                           m_Boundaries;     // Bounding Volume of Hash Grid - ( Min & Max Positions )
        std::array< partition::cell, settings::max_partition_cells_v > m_HashedGrid;     // Array of Hashed Cells
        paperback::coordinator::instance&                              m_Coordinator;    // Reference to Coordinator
        paperback::Vector3f                                            m_CellDimension;  // Vector3f Cell Dimensions
        paperback::u32                                                 m_CellSize;       // Size of each Cell - ( Just Cell Dimensions )
        float                                                          m_InvCellSize;    // Inverse Size of each Cell ( Can Use Multiply Instead )
    };
}