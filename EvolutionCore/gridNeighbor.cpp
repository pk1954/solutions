// gridNeighbor.cpp :
//

#include "stdafx.h"
#include "grid_model.h"
#include "gridPoint.h"
#include "gridNeighbor.h"

GridPoint const NeighborhoodIterator::GP_NORTH_WEST( WEST, NORTH );

NeighborhoodIterator::NeighborhoodIterator( GridPoint const & gp )
  : m_gpCenter( gp ), //  + GridPoint::GRID_SIZE ),
    m_gpDelta( GP_NORTH_WEST ),
    m_gpNeighbor( calcNeighbor( ) )
{ 
    assert( gp.IsInGrid( ) );
};

bool NeighborhoodIterator::NextNeighbor( ) 
{ 
    static GridPoint const table[3][3] = 
    {
        { GridPoint( CENTER, NORTH  ), GridPoint( EAST,   NORTH  ), GridPoint( EAST,   CENTER ) }, 
        { GridPoint( WEST,   NORTH  ), GridPoint( CENTER, CENTER ), GridPoint( EAST,   SOUTH  ) }, 
        { GridPoint( WEST,   CENTER ), GridPoint( WEST,   SOUTH  ), GridPoint( CENTER, SOUTH  ) }
    }; 
   
    m_gpDelta = table[m_gpDelta.y + 1][m_gpDelta.x + 1];
    m_gpNeighbor = calcNeighbor( );
    return (m_gpDelta != GP_NORTH_WEST);
};

void Apply2AllNeighbors( GridPointNeighbor_Functor const & func ) 
{
    GridPoint const gpCenter = func.GetCenter( );
    NeighborhoodIterator iter( gpCenter );
    do
    {
        if ( (func)( iter.GetNeighbor( ) ) )
            break;
    } while ( iter.NextNeighbor( ) );
};

