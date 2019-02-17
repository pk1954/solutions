// gridRect.h : 
//

#pragma once

#include "gridPoint.h"
#include "RectType.h"

using GridRectSize = SizeType< GRID_COORD >;
using GridRect     = RectType< GRID_COORD >;

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
 
void Apply2Rect( GridPointFunc const &, GridRect const & );
void Apply2Rect( GridPointFunc const &, GridPoint const, GridPoint const, bool const = false );
void Apply2Grid( GridPointFunc const &,                                   bool const = false );
