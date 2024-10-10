// PackGridPoint.ixx
//
// EvoWindows

export module PackGridPoint;

import SaveCast;
import WinBasics;
import EvoCoreLib;

export LPARAM Pack2LParam(GridPoint const gp)
{
	return MakeLong(gp.GetXvalue(), gp.GetYvalue());
}

export GridPoint UnpackFromLParam(LPARAM const lParam)
{
	return GridPoint
	(
		GridCoord(Cast2Short(CrsrXpos(lParam))), 
		GridCoord(Cast2Short(CrsrYpos(lParam))) 
	);
}