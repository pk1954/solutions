// wrapperHelpers.h : 
//

#pragma once

#include "script.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "gridCircle.h"
#include "pixelPoint.h"

extern GridPoint  ScrReadGridPoint ( Script & );
extern GridCircle ScrReadGridCircle( Script & );
extern GridRect   ScrReadGridRect  ( Script & );
extern PixelPoint ScrReadPixelPoint( Script & );
