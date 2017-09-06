// gridNeighbor.cpp :
//

#include "stdafx.h"
#include "gridPoint.h"
#include "gridNeighbor.h"

static GridPoint m_aNeighbors[ GridPoint::GRID_WIDTH ][ GridPoint::GRID_HEIGHT ][ NR_OF_NEIGHBORS ];

static void initNeighborMatrix( )     // Initialization of m_aNeighbors
{
    static long const WEST   = -1;
    static long const EAST   =  1;
    static long const NORTH  = -1;
    static long const SOUTH  =  1;
    static long const CENTER =  0;

	static GridPoint const table[3][3] = 
	{
		{ GridPoint( CENTER, NORTH  ), GridPoint( EAST,   NORTH  ), GridPoint( EAST,   CENTER ) }, 
		{ GridPoint( WEST,   NORTH  ), GridPoint( CENTER, CENTER ), GridPoint( EAST,   SOUTH  ) }, 
		{ GridPoint( WEST,   CENTER ), GridPoint( WEST,   SOUTH  ), GridPoint( CENTER, SOUTH  ) }
	}; 

	GridPoint const GP_NORTH_WEST( WEST, NORTH );

    GridPoint gp;
    for ( gp.y = 0; gp.y <= GridPoint::GRID_HEIGHT - 1; ++ gp.y )
    for ( gp.x = 0; gp.x <= GridPoint::GRID_WIDTH  - 1; ++ gp.x )
	{
		GridPoint gpDelta    = GP_NORTH_WEST;
		GridPoint gpNeighbor = gp + gpDelta + GridPoint::GRID_SIZE;
		gpNeighbor %= GridPoint::GRID_SIZE;
		for ( auto & neighbor : m_aNeighbors[ gp.x ][ gp.y ] )
		{
			neighbor   = gpNeighbor;
			gpDelta    = table[gpDelta.y + 1][gpDelta.x + 1];
			gpNeighbor = gp + gpDelta + GridPoint::GRID_SIZE;
			gpNeighbor %= GridPoint::GRID_SIZE;
		}
	}
}

void Apply2AllNeighbors( GridPointNeighbor_Functor const & func ) 
{
	static bool bIsInitialized = false;

	if ( ! bIsInitialized )
	{
		initNeighborMatrix( );
		bIsInitialized = true;
	}

	GridPoint         const gpCenter       = func.GetCenter( );
	GridPoint const *       m_pGpNeighbors = m_aNeighbors[ gpCenter.x ][ gpCenter.y ];
	GridPoint const * const m_pGpStop      = m_pGpNeighbors + NR_OF_NEIGHBORS;

	do
	{
		if ( (func)( * m_pGpNeighbors ) )
			break;
	} while ( ++ m_pGpNeighbors < m_pGpStop );
};
