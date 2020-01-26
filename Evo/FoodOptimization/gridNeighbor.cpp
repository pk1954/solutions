// gridNeighbor.cpp :
//
// test version

#include "stdafx.h"
#include <assert.h>
#include "gridPoint.h"
#include "gridNeighbor.h"

const GridPoint NEIGHBORHOOD_ITERATOR::GP_NORTH_WEST = { NORTH, WEST };
const GridPoint NEIGHBORHOOD_ITERATOR::GP_SOUTH_EAST = { SOUTH, EAST };

NEIGHBORHOOD_ITERATOR::NEIGHBORHOOD_ITERATOR( GridPoint gp ) 
{ 
	assert( gp.IsInGrid( ) );
	m_gpBase  = gp + GridPoint::GRID_SIZE;
	m_gpDelta = GP_NORTH_WEST; 
	calcNeighbor( );
};

bool NEIGHBORHOOD_ITERATOR::Next( ) 
{ 
    static const GridPoint table[3][3] = 
    {
        { { CENTER, NORTH  }, { EAST,   NORTH  }, { EAST,   CENTER } }, 
        { { WEST,   NORTH  }, { CENTER, CENTER }, { EAST,   SOUTH  } }, 
        { { WEST,   CENTER }, { WEST,   SOUTH  }, { CENTER, SOUTH  } }
    }; 
   
    m_gpDelta = table[m_gpDelta.y + 1][m_gpDelta.x + 1];

	calcNeighbor( );
	return (m_gpDelta != GP_NORTH_WEST);
};

