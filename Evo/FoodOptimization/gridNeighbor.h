// gridNeighbor.h : 
//
//  test version
#pragma once

#include "assert.h"
#include "gridPoint.h"

class NEIGHBORHOOD_ITERATOR
{
public:

	NEIGHBORHOOD_ITERATOR( GridPoint );
	bool Next( );
	inline GridPoint Get( ) { return m_gpNeighbor; };
	
private:
	inline void calcNeighbor() { m_gpNeighbor = (m_gpBase + m_gpDelta) % GridPoint::GRID_SIZE; }; 

	GridPoint m_gpBase;
	GridPoint m_gpDelta;
	GridPoint m_gpNeighbor;

	static const long WEST   = -1;
	static const long EAST   =  1;
	static const long NORTH  = -1;
	static const long SOUTH  =  1;
	static const long CENTER =  0;

	static const GridPoint GP_NORTH_WEST;
	static const GridPoint GP_SOUTH_EAST;
};
