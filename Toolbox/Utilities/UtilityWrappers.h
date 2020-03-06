// UtilityWrappers.h
//
// Utilities

#pragma once

#include "PixelTypes.h"

class Script;

extern PIXEL         ScrReadPixel        ( Script & );
extern PixelPoint    ScrReadPixelPoint   ( Script & );
extern PixelRectSize ScrReadPixelRectSize( Script & );
extern PixelRect     ScrReadPixelRect    ( Script & );
extern fPIXEL        ScrReadfPixel       ( Script & );
extern fPixelPoint   ScrReadfPixelPoint  ( Script & );

extern void DefineUtilityWrapperFunctions( );
