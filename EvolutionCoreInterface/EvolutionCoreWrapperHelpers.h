// wrapperHelpers.h : 
//

#pragma once

#include "script.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "pixelPoint.h"

extern GRID_COORD ScrReadGridCoord ( Script & );
extern GridPoint  ScrReadGridPoint ( Script & );
extern GridRect   ScrReadGridRect  ( Script & );
extern PixelPoint ScrReadPixelPoint( Script & );
