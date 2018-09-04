// gridRect.cpp :
//

#include "stdafx.h"
#include "limits.h"
#include <iostream>
#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "gridRect.h"

using namespace std;

GridRect const GridRect::GRID_RECT_FULL ( GridPoint::GRID_ORIGIN, GridPoint::GRID_SIZE - 1 );
GridRect const GridRect::GRID_RECT_EMPTY( GridPoint::GRID_ORIGIN, GridPoint( 0, 0 ) );

GridPoint GridRect::clipStartPoint( ) const
{
	return GridPoint
	(
		max( m_lLeft, GRID_RECT_FULL.m_lLeft ),
		max( m_lTop,  GRID_RECT_FULL.m_lTop )
	);
};

GridPoint GridRect::clipEndPoint( ) const
{
	return GridPoint
	(
		min( m_lRight,  GRID_RECT_FULL.m_lRight ),
		min( m_lBottom, GRID_RECT_FULL.m_lBottom )
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
		--gpStart;
		++gpEnd;
	}
    GridPoint gp;
    for ( gp.y = gpStart.y; gp.y <= gpEnd.y; ++gp.y )
        for ( gp.x = gpStart.x; gp.x <= gpEnd.x; ++gp.x )
            func( gp );
}

void Apply2Grid( GridPointFunc const & func, bool const fWithBorders )
{
	Apply2Rect( func, GridPoint::GRID_ORIGIN, GridPoint::GRID_MAXIMUM, fWithBorders );
}

std::wostream & operator << ( std::wostream & out, GridRect const & rect )
{
    out << rect.GetStartPoint() << L' ' << rect.GetEndPoint();
    return out;
}
