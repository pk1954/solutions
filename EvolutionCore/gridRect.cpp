// gridRect.cpp :
//

#include "stdafx.h"
#include "limits.h"
#include <iostream>
#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "gridRect.h"

using std::min;
using std::max;

GridRect const ClipToGrid( GridRect const & rect ) 
{ 
	GridPoint const startPoint
	(
		max( rect.GetLeft(), GridRect::GRID_RECT_FULL().GetLeft() ),
		max( rect.GetTop(),  GridRect::GRID_RECT_FULL().GetTop()  )
	);

	GridPoint const endPoint
	(
		min( rect.GetRight(),  GridRect::GRID_RECT_FULL().GetRight() ),
		min( rect.GetBottom(), GridRect::GRID_RECT_FULL().GetBottom() )
	);

	return GridRect( startPoint, endPoint );	
}

void Apply2Rect( GridPointFunc const & func, GridRect const & rect )
{
	GridRect rectClipped = ClipToGrid( rect );
	::Apply2Rect( func, rectClipped.GetStartPoint(), rectClipped.GetEndPoint() );
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

    for ( short y = gpStart.GetYvalue(); y <= gpEnd.GetYvalue(); ++y )
    for ( short x = gpStart.GetXvalue(); x <= gpEnd.GetXvalue(); ++x )
	{
		func( GridPoint{ GRID_COORD(x), GRID_COORD(y) } );
	}
}

void Apply2Grid( GridPointFunc const & func, bool const fWithBorders )
{
	Apply2Rect( func, GRID_ORIGIN(), GRID_MAXIMUM(), fWithBorders );
}

std::wostream & operator << ( std::wostream & out, GridRect const & rect )
{
    out << rect.GetStartPoint() << L' ' << rect.GetEndPoint();
    return out;
}
