// gridNeighbor.cpp :
//

#include "stdafx.h"
#include <array>
#include "gridPoint.h"
#include "gridRect.h"
#include "gridNeighbor.h"

int                           Neighborhood::m_iNrOfNeighbors = 0;
Neighborhood::NEIGHBOR_GRID * Neighborhood::m_pGridNeighbors = nullptr;

static GRID_X const WEST     { -1_GRID_COORD };
static GRID_X const EAST     {  1_GRID_COORD };
static GRID_Y const NORTH    { -1_GRID_COORD };
static GRID_Y const SOUTH    {  1_GRID_COORD };
static GRID_X const CENTER_X {  0_GRID_COORD };
static GRID_Y const CENTER_Y {  0_GRID_COORD };

static std::array< GridPoint, 8 > const table8 = 
{
	GridPoint( CENTER_X, NORTH    ), 
	GridPoint( WEST,     NORTH    ),
	GridPoint( WEST,     CENTER_Y ),
	GridPoint( WEST,     SOUTH    ), 
	GridPoint( CENTER_X, SOUTH    ), 
	GridPoint( EAST,     SOUTH    ),
	GridPoint( EAST,     CENTER_Y ),
	GridPoint( EAST,     NORTH    ) 
}; 

static std::array< GridPoint, 6 > const table6even = 
{
	GridPoint( CENTER_X, NORTH    ), 
	GridPoint( EAST,     CENTER_Y ),
	GridPoint( EAST,     SOUTH    ), 
	GridPoint( CENTER_X, SOUTH    ), 
	GridPoint( WEST,     SOUTH    ),
	GridPoint( WEST,     CENTER_Y ) 
}; 

static std::array< GridPoint, 6 > const table6odd = 
{
	GridPoint( CENTER_X, NORTH    ), 
	GridPoint( EAST,     NORTH    ), 
	GridPoint( EAST,     CENTER_Y ),
	GridPoint( CENTER_X, SOUTH    ), 
	GridPoint( WEST,     CENTER_Y ), 
	GridPoint( WEST,     NORTH    )
}; 

static std::array< GridPoint, 4 > const table4 = 
{
	GridPoint( CENTER_X, NORTH    ), 
	GridPoint( WEST,     CENTER_Y ),
	GridPoint( CENTER_X, SOUTH    ), 
	GridPoint( EAST,     CENTER_Y )
}; 

void Neighborhood::InitClass( int const iNrOfNeighbors )     // Initialization of m_pGridNeighbors
{
	assert( ( iNrOfNeighbors == 4 ) || ( iNrOfNeighbors == 6 ) || ( iNrOfNeighbors == 8 ) );
	m_iNrOfNeighbors = iNrOfNeighbors;
	m_pGridNeighbors = new NEIGHBOR_GRID;
	m_pGridNeighbors->resize( GRID_WIDTH_VAL );
	for ( auto & col: * m_pGridNeighbors )
		col.resize( GRID_HEIGHT_VAL );

	Apply2Grid  // initialization of grid variables which never change after initialization
	( 
    	[&](GridPoint const gp)
		{
			NEIGHBORS & neighbors = ( * m_pGridNeighbors)[ gp.GetXvalue() ][ gp.GetYvalue() ];
			neighbors.resize( m_iNrOfNeighbors, GridPoint::ZERO_VAL() );
			for ( int i = 0; i < m_iNrOfNeighbors; ++i )
			{
				GridPoint gpDelta = ( m_iNrOfNeighbors == 6 ) 
					                ? ( IsEvenColumn( gp ) )
										? table6even[i] 
										: table6odd[i]
					                : ( m_iNrOfNeighbors == 8 ) 
					                    ? table8[i] 
					                    : table4[i];
				neighbors[i] = Wrap2Grid( gp + gpDelta );
			}
		}
	);
}
