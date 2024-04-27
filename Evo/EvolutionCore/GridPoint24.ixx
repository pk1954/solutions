// GridPoint24.ixx : 
//
// GridPoint packed as Int24

module;

#include <cassert>

export module GridPoint24;

import Int24;
import GridPoint;

export class GridPoint24: public Int24  
{
public:
	GridPoint24(long const x, long const y) :
		Int24((x << 12) + (y & 0x0fff))
	{
		assert(x <= 0x0fff);
		assert(y <= 0x0fff);
	}

	GridPoint24(GridPoint const gp) :
		GridPoint24(gp.GetXvalue(), gp.GetYvalue())
	{ }

	GridPoint Unpack() const
	{
		int i = GetValue();
		GridPoint gp(GridCoord(i >> 12), GridCoord(i & 0x0fff));
		return gp;
	}

	static GridPoint Unpack(Int24 const i24)
	{
		return static_cast<GridPoint24 const &>(i24).Unpack() ;
	}
};
