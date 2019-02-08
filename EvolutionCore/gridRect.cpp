// gridRect.cpp :
//

#include "stdafx.h"
#include "limits.h"
#include <iostream>
#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "gridRect.h"

using namespace std;

GridPoint GridRect::clipStartPoint( ) const
{
	return GridPoint
	(
		max( m_Left, GRID_RECT_FULL().m_Left ),
		max( m_Top,  GRID_RECT_FULL().m_Top  )
	);
};

GridPoint GridRect::clipEndPoint( ) const
{
	return GridPoint
	(
		min( m_Right,  GRID_RECT_FULL().m_Right ),
		min( m_Bottom, GRID_RECT_FULL().m_Bottom )
	);
};

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

    for ( short y = GetYvalue(gpStart); y <= GetYvalue(gpEnd); ++y )
    for ( short x = GetXvalue(gpStart); x <= GetXvalue(gpEnd); ++x )
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
