#pragma once

// win32_packGridPoint.h : 
//

#pragma once

#include "minwindef.h"
#include "windowsx.h"
#include "gridPoint.h"

static LPARAM Pack2LParam( GridPoint const gp )
{
	return MAKELONG( gp.x, gp.y );
}

static GridPoint UnpackFromLParam( LPARAM const lParam )
{
	return GridPoint( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
}