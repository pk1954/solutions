// GridDimensions.h
//
// EvolutionCoreInterface
//
// knows static size of the Grid
// contains functions directly dependent on Grid size

#pragma once

#include <algorithm>  // min/max templates
#include "gridRect.h"

using std::min;
using std::max;

class GridDimensions
{
public:
	static void DefineGridSize(GRID_X const width, GRID_Y const height)
	{
		m_gridSize = GridPoint( width, height);
	}

	static GRID_X    const GridWidth()    { return m_gridSize.GetX(); }
	static GRID_Y    const GridHeight()   { return m_gridSize.GetY(); }
	static GRID_X    const GridMinX()     { return 0_GRID_X; }
	static GRID_Y    const GridMinY()     { return 0_GRID_Y; }
	static GRID_X    const GridMaxX()     { return GridWidth()  - 1_GRID_X; }
	static GRID_Y    const GridMaxY()     { return GridHeight() - 1_GRID_Y; }
	static GridPoint const GridRectSize() { return m_gridSize; }
	static GridPoint const GridOrigin()   { return GridPoint::ZERO_VAL(); }
	static GridPoint const GridMaximum()  { return m_gridSize - 1_GRID_COORD; }
	static GridRect  const GridRectFull() { return GridRect( GridOrigin(), GridMaximum() ); }

	static int const GridWidthVal()  { return GridWidth() .GetBaseValue(); }; // abbreviations for e.g. array dims
	static int const GridHeightVal() { return GridHeight().GetBaseValue(); };
	static int const GetGridArea()   { return GridWidthVal() * GridHeightVal(); };

	static inline GridPoint const Wrap2Grid( GridPoint const gp ) 
	{ 
		return (gp + m_gridSize) % m_gridSize; 
	}

private:
	static GridPoint m_gridSize; 
};

inline bool const Neighbors( GridPoint const a, GridPoint const b )
{ 
	GridPoint const gpDiff { a - b };
	GRID_X    const dx { GRID_COORD(std::abs(gpDiff.GetXvalue())) }; 
	GRID_Y    const dy { GRID_COORD(std::abs(gpDiff.GetYvalue())) };
    return ( 
		      (( dx <= 1_GRID_X ) || ( dx == GridDimensions::GridMaxX() )) && 
		      (( dy <= 1_GRID_Y ) || ( dy == GridDimensions::GridMaxY() ))
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
