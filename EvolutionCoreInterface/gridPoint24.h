// gridPoint24.h : 
//
// GridPoint packed as Int24

#pragma once

#include <assert.h>
#include "debug.h"
#include "Int24.h"
#include "gridPoint.h"

class GridPoint24: public Int24  
{
public:
	GridPoint24( long const x, long const y ) :
		Int24( (x << 12) + (y & 0x0fff) )
	{
		assert( x <= 0x0fff );
		assert( y <= 0x0fff );
	}

	GridPoint24( GridPoint const gp ) :
		GridPoint24( GetXvalue( gp ), GetYvalue( gp ) )
	{ }

	GridPoint Unpack() const
	{
		int i = GetValue();
		GridPoint gp( GRID_COORD(i >> 12), GRID_COORD(i & 0x0fff) );
		return gp;
	}

	static GridPoint Unpack( Int24 const i24 )
	{
		return static_cast<GridPoint24 const &>(i24).Unpack() ;
	}
};
