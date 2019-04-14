// GridDimensions.h
//
// EvolutionCoreInterface
//
// knows static size of the Grid and number of neighbors
// contains functions directly dependent on Grid size

#pragma once

#include <algorithm>  // min/max templates
#include <vector>
#include "gridRect.h"
#include "gridNeighbor.h"

using std::min;
using std::max;
using std::vector;

class GridDimensions
{
public:
	static void DefineGridSize
	(
		GRID_COORD const, 
		GRID_COORD const,
		int        const
	);

	static void GetNeighborLists( Grid const &, GridPoint const &, Neighborhood *, Neighborhood * );

	static int        const GetNrOfNeigbors() { return m_iNrOfNeighbors; }
	static GRID_COORD const GridWidth()       { return m_gridSize.GetX(); }
	static GRID_COORD const GridHeight()      { return m_gridSize.GetY(); }
	static GRID_COORD const GridMinX()        { return 0_GRID_COORD; }
	static GRID_COORD const GridMinY()        { return 0_GRID_COORD; }
	static GRID_COORD const GridMaxX()        { return GridWidth()  - 1_GRID_COORD; }
	static GRID_COORD const GridMaxY()        { return GridHeight() - 1_GRID_COORD; }
	static GridPoint  const GridRectSize()    { return m_gridSize; }
	static GridPoint  const GridOrigin()      { return GridPoint::ZERO_VAL(); }
	static GridPoint  const GridMaximum()     { return m_gridSize - 1_GRID_COORD; }
	static GridRect   const GridRectFull()    { return GridRect( GridOrigin(), GridMaximum() ); }

	static int const GridWidthVal()  { return GridWidth() .GetValue(); }; // abbreviations for e.g. array dims
	static int const GridHeightVal() { return GridHeight().GetValue(); };
	static int const GetGridArea()   { return GridWidthVal() * GridHeightVal(); };

	static inline GridPoint const Wrap2Grid( GridPoint const gp ) 
	{ 
		return (gp + m_gridSize) % m_gridSize; 
	}

private:
	using NEIGHBOR_GRID = vector< vector< NEIGHBORS > >;

	static GridPoint       m_gridSize; 
	static int             m_iNrOfNeighbors;
	static NEIGHBOR_GRID * m_pGridNeighbors;
};

inline bool const Neighbors( GridPoint const a, GridPoint const b )
{ 
	GridPoint  const gpDiff { a - b };
	GRID_COORD const dx     { GRID_COORD(std::abs(gpDiff.GetXvalue())) }; 
	GRID_COORD const dy     { GRID_COORD(std::abs(gpDiff.GetYvalue())) };
    return ( 
		      (( dx <= 1_GRID_COORD ) || ( dx == GridDimensions::GridMaxX() )) && 
		      (( dy <= 1_GRID_COORD ) || ( dy == GridDimensions::GridMaxY() ))
		   );
}

inline GridPoint const ClipToGrid( GridPoint gp ) 
{ 
	gp = GridPoint
	(
		max( gp.GetX(), GridDimensions::GridMinX() ),
		max( gp.GetY(), GridDimensions::GridMinY() )
	);

	gp = GridPoint
	(
		min( gp.GetX(), GridDimensions::GridMaxX() ),
		min( gp.GetY(), GridDimensions::GridMaxY() )
	);

	return gp;	
}

inline bool const IsInGrid( GridPoint const & gp ) 
{ 
	return ClipToGrid( gp ) == gp;
};

inline void Apply2Rect( GridPointFunc const & func, GridRect const & rect )
{
	::Apply2Rect
	( 
		func, 
		ClipToGrid( rect.GetStartPoint() ),
		ClipToGrid( rect.GetEndPoint  () )
	);
}

inline void Apply2Grid( GridPointFunc const & func, bool const fWithBorders = false)
{
	Apply2Rect( func, GridDimensions::GridOrigin(), GridDimensions::GridMaximum(), fWithBorders );
}
