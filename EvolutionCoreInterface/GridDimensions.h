// GridDimensions.h
//
// EvolutionCoreInterface
//
// knows static size of the Grid
// contains all functions directly dependent on Grid size

#pragma once

#include <algorithm>  // min/max templates
#include "gridRect.h"

using std::min;
using std::max;

static constexpr GRID_X const GRID_WIDTH { 200_GRID_COORD };  // the dimension of the GRID in x direction
static constexpr GRID_Y const GRID_HEIGHT{ 100_GRID_COORD };  // the dimension of the GRID in y direction

static GRID_X const GRID_X_MIN { 0_GRID_COORD };
static GRID_Y const GRID_Y_MIN { 0_GRID_COORD };

static GRID_X const GRID_X_MAX { GRID_WIDTH .GetValue() - 1_GRID_COORD };
static GRID_Y const GRID_Y_MAX { GRID_HEIGHT.GetValue() - 1_GRID_COORD };

static int const GRID_WIDTH_VAL  { GRID_WIDTH .GetValue().GetValue() }; // abbreviations for e.g. array dims
static int const GRID_HEIGHT_VAL { GRID_HEIGHT.GetValue().GetValue() };

inline static int const GRID_AREA() 
{ 
	static int res = GRID_WIDTH_VAL * GRID_HEIGHT_VAL; 
	return res;
};

inline static GridPoint const & GRID_ORIGIN() 
{ 
	static GridPoint res = GridPoint( GRID_X_MIN, GRID_Y_MIN ); 
	return res;
};

inline static GridPoint const & GRID_MAXIMUM() 
{ 
	static GridPoint res = GridPoint( GRID_X_MAX, GRID_Y_MAX ); 
	return res;
};

inline static GridPoint const & GRID_SIZE() 
{ 
	static GridPoint res = GridPoint( GRID_WIDTH, GRID_HEIGHT ); 
	return res;
};

inline static GridRect const & GRID_RECT_EMPTY() 
{ 
	static GridRect res( GRID_ORIGIN(), GRID_ORIGIN() ); 
	return res;
};

inline static GridRect const & GRID_RECT_FULL() 
{ 
	static GridRect res( GRID_ORIGIN(), GRID_SIZE() - 1_GRID_COORD ); 
	return res;
};
 
inline bool const Neighbors( GridPoint const a, GridPoint const b )
{ 
	GridPoint const gpDiff{ a - b };
	GRID_X    const dx { GRID_COORD(std::abs(gpDiff.GetXvalue())) }; 
	GRID_Y    const dy { GRID_COORD(std::abs(gpDiff.GetYvalue())) };
    return ( 
		      (( dx <= GRID_X(1_GRID_COORD) ) || ( dx == GRID_X_MAX )) && 
		      (( dy <= GRID_Y(1_GRID_COORD) ) || ( dy == GRID_Y_MAX ))
		   );
}

inline GridPoint const Wrap2Grid(GridPoint const gp) 
{ 
	return (gp + GRID_SIZE()) % GRID_SIZE(); 
}

inline GridPoint const ClipToGrid( GridPoint gp ) 
{ 
	gp = GridPoint
	(
		max( gp.GetX(), GRID_X_MIN ),
		max( gp.GetY(), GRID_Y_MIN  )
	);

	gp = GridPoint
	(
		min( gp.GetX(), GRID_X_MAX ),
		min( gp.GetY(), GRID_Y_MAX )
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
	Apply2Rect( func, GRID_ORIGIN(), GRID_MAXIMUM(), fWithBorders );
}
