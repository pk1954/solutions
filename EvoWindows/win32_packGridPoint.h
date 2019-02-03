#pragma once

// win32_packGridPoint.h : 
//

#pragma once

#include "minwindef.h"
#include "windowsx.h"
#include "gridPoint.h"

static LPARAM Pack2LParam( GridPoint const gp )
{
	return MAKELONG( gp.GetXshort(), gp.GetYshort() );
}

static GridPoint UnpackFromLParam( LPARAM const lParam )
{
	return GridPoint( GRID_COORD(GET_X_LPARAM(lParam)), GRID_COORD(GET_Y_LPARAM(lParam)) );
}