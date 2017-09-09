// gridNeighbor.cpp :
//

#include "stdafx.h"
#include "gridPoint.h"
#include "gridNeighbor.h"

int                           Neighborhood::m_iNrOfNeighbors = 0;
Neighborhood::NEIGHBOR_GRID * Neighborhood::m_pGridNeighbors = nullptr;

void Neighborhood::InitClass( int const iNrOfNeighbors )     // Initialization of m_pGridNeighbors
{
	assert( ( iNrOfNeighbors == 4 ) || ( iNrOfNeighbors == 8 ) );

	m_iNrOfNeighbors = iNrOfNeighbors;

    static long const WEST   = -1;
    static long const EAST   =  1;
    static long const NORTH  = -1;
    static long const SOUTH  =  1;
    static long const CENTER =  0;

	static GridPoint const table8[3][3] = 
	{
		{ GridPoint( CENTER, NORTH  ), GridPoint( EAST,   NORTH  ), GridPoint( EAST,   CENTER ) }, 
		{ GridPoint( WEST,   NORTH  ), GridPoint( CENTER, CENTER ), GridPoint( EAST,   SOUTH  ) }, 
		{ GridPoint( WEST,   CENTER ), GridPoint( WEST,   SOUTH  ), GridPoint( CENTER, SOUTH  ) }
	}; 

	static GridPoint const table4[3][3] = 
	{
		{ GridPoint::GP_NULL,          GridPoint( EAST,   CENTER ), GridPoint::GP_NULL          }, 
		{ GridPoint( WEST,   NORTH  ), GridPoint( CENTER, CENTER ), GridPoint( CENTER, SOUTH  ) }, 
		{ GridPoint::GP_NULL         , GridPoint( WEST,   CENTER ), GridPoint::GP_NULL          }
	}; 

	GridPoint const GP_NORTH_WEST( WEST, NORTH );
	GridPoint const GP_CENTER_NORTH( CENTER, NORTH );

	m_pGridNeighbors = new NEIGHBOR_GRID;

    GridPoint gp;
    for ( gp.y = 0; gp.y <= GridPoint::GRID_HEIGHT - 1; ++ gp.y )
	{
		NEIGHBOR_ROW & pRow = (* m_pGridNeighbors)[ gp.y ];
		for ( gp.x = 0; gp.x <= GridPoint::GRID_WIDTH  - 1; ++ gp.x )
		{
			NEIGHBORS & neighbors = pRow[ gp.x ];
			GridPoint gpDelta    = GP_CENTER_NORTH;
			GridPoint gpNeighbor = gp + gpDelta + GridPoint::GRID_SIZE;
			gpNeighbor %= GridPoint::GRID_SIZE;
			neighbors.reserve( m_iNrOfNeighbors );
			for ( int i = 0; i < m_iNrOfNeighbors; ++i )
			{
				neighbors.push_back( gpNeighbor );
				if ( m_iNrOfNeighbors == 8 )
					gpDelta = table8[gpDelta.y + 1][gpDelta.x + 1];
				else
					gpDelta = table4[gpDelta.y + 1][gpDelta.x + 1];
				gpNeighbor = gp + gpDelta + GridPoint::GRID_SIZE;
				gpNeighbor %= GridPoint::GRID_SIZE;
			}
		}
	}
}

bool Neighborhood::Apply2All( GridPoint gpCenter, GridPoint_Functor  & func ) 
{
	for ( auto n: getNeighbors( gpCenter ) )
	{
		if ( (func)( n ) )
			return true;
	}
	return false;
};
