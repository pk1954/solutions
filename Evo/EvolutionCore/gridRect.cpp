// gridRect.cpp :
//
// EvolutionCore


#include "gridRect.h"

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
		gpStart -= GridPoint( 1_GRID_COORD );
		gpEnd   += GridPoint( 1_GRID_COORD );
	}

    for ( GRID_COORD y = gpStart.GetY(); y <= gpEnd.GetY(); ++y )
    for ( GRID_COORD x = gpStart.GetX(); x <= gpEnd.GetX(); ++x )
	{
		func( GridPoint( x, y ) );
	}
}
