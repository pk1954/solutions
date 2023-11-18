// win32_packGridPoint.h : 
//

module;

#include "minwindef.h"
#include "windowsx.h"

import GridPoint;

static LPARAM Pack2LParam(GridPoint const gp)
{
	return MAKELONG(gp.GetXvalue(), gp.GetYvalue());
}

static GridPoint UnpackFromLParam(LPARAM const lParam)
{
	return GridPoint
	(
		GridCoord(GET_X_LPARAM(lParam)), 
		GridCoord(GET_Y_LPARAM(lParam)) 
	);
}