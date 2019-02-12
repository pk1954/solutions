// gridRect.cpp :
//

#include "stdafx.h"
#include "gridRect.h"

void Apply2Rect( GridPointFunc const & func, GridRect const & rect )
{
	::Apply2Rect
	( 
		func, 
		ClipToGrid( rect.GetStartPoint() ),
		ClipToGrid( rect.GetEndPoint  () )
	);
}

void Apply2Rect
( 
	GridPointFunc const & func,
	GridPoint             gpStart,
	GridPoint             gpEnd,
	bool          const   fWithBorders
)
{
	if (fWithBorders)
	{
		gpStart -= GridPoint( GRID_COORD(1_GRID_COORD), GRID_COORD(1_GRID_COORD) );
		gpEnd   += GridPoint( GRID_COORD(1_GRID_COORD), GRID_COORD(1_GRID_COORD) );
	}

    for ( GRID_COORD y = gpStart.GetY(); y <= gpEnd.GetY(); ++y )
    for ( GRID_COORD x = gpStart.GetX(); x <= gpEnd.GetX(); ++x )
	{
		func( GridPoint( x, y ) );
	}
}

void Apply2Grid( GridPointFunc const & func, bool const fWithBorders )
{
	Apply2Rect( func, GRID_ORIGIN(), GRID_MAXIMUM(), fWithBorders );
}
