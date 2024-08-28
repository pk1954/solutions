// PackGridPoint.ixx
//
// EvoWindows

export module PackGridPoint;

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
		GridCoord(GET_X_LPARAM(lParam)), 
		GridCoord(GET_Y_LPARAM(lParam)) 
	);
}