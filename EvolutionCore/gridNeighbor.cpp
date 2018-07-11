// gridNeighbor.cpp :
//

#include "stdafx.h"
#include <array>
#include "gridPoint.h"
#include "gridRect.h"
#include "gridNeighbor.h"

int                           Neighborhood::m_iNrOfNeighbors = 0;
Neighborhood::NEIGHBOR_GRID * Neighborhood::m_pGridNeighbors = nullptr;

static long const WEST   = -1;
static long const EAST   =  1;
static long const NORTH  = -1;
static long const SOUTH  =  1;
static long const CENTER =  0;

static std::array< GridPoint, 8 > const table8 = 
{
	GridPoint( CENTER, NORTH  ), 
	GridPoint( WEST,   NORTH  ),
	GridPoint( WEST,   CENTER ),
	GridPoint( WEST,   SOUTH  ), 
	GridPoint( CENTER, SOUTH  ), 
	GridPoint( EAST,   SOUTH  ),
	GridPoint( EAST,   CENTER ),
	GridPoint( EAST,   NORTH  ) 
}; 

static std::array< GridPoint, 6 > const table6even = 
{
	GridPoint( CENTER, NORTH  ), 
	GridPoint( EAST,   CENTER ),
	GridPoint( EAST,   SOUTH  ), 
	GridPoint( CENTER, SOUTH  ), 
	GridPoint( WEST,   SOUTH  ),
	GridPoint( WEST,   CENTER ) 
}; 

static std::array< GridPoint, 6 > const table6odd = 
{
	GridPoint( CENTER, NORTH  ), 
	GridPoint( EAST,   NORTH  ), 
	GridPoint( EAST,   CENTER ),
	GridPoint( CENTER, SOUTH  ), 
	GridPoint( WEST,   CENTER ), 
	GridPoint( WEST,   NORTH  )
}; 

static std::array< GridPoint, 4 > const table4 = 
{
	GridPoint( CENTER, NORTH  ), 
	GridPoint( WEST,   CENTER ),
	GridPoint( CENTER, SOUTH  ), 
	GridPoint( EAST,   CENTER )
}; 

void Neighborhood::InitClass( int const iNrOfNeighbors )     // Initialization of m_pGridNeighbors
{
	assert( ( iNrOfNeighbors == 4 ) || ( iNrOfNeighbors == 6 ) || ( iNrOfNeighbors == 8 ) );
	m_iNrOfNeighbors = iNrOfNeighbors;
	m_pGridNeighbors = new NEIGHBOR_GRID;
    Apply2Grid  // initialization of grid variables which never change after initialization
	( 
    	[&](GridPoint const & gp)
		{
			NEIGHBORS & neighbors = ( * m_pGridNeighbors)[ gp.y ][ gp.x ];
			neighbors.reserve( m_iNrOfNeighbors );
			for ( int i = 0; i < m_iNrOfNeighbors; ++i )
			{
				GridPoint gpDelta = ( m_iNrOfNeighbors == 6 ) 
					                ? ( gp.IsEvenCol( ) )
										? table6even[i] 
										: table6odd[i]
					                : ( m_iNrOfNeighbors == 8 ) 
					                    ? table8[i] 
					                    : table4[i];
				GridPoint gpNeighbor = ( gp + gpDelta + GridPoint::GRID_SIZE ) % GridPoint::GRID_SIZE;
				neighbors.push_back( gpNeighbor );
			}
		}
	);
}
