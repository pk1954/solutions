// PackGridPoint.ixx
//
// EvoWindows

export module PackGridPoint;

import WinBasics;
import EvoCoreLib;

export static LPARAM Pack2LParam(GridPoint const gp)
{
	return MAKELONG(gp.GetXvalue(), gp.GetYvalue());
}

export static GridPoint UnpackFromLParam(LPARAM const lParam)
{
	return GridPoint
	(
		GridCoord(GET_X_LPARAM(lParam)), 
		GridCoord(GET_Y_LPARAM(lParam)) 
	);
}