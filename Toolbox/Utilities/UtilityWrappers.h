// UtilityWrappers.h
//
// Utilities

#pragma once

#include "PixelTypes.h"
#include "BoolOp.h"

class Script;

extern tBoolOp       ScrReadBoolOp       ( Script & );
extern PIXEL         ScrReadPixel        ( Script & );
extern PixelPoint    ScrReadPixelPoint   ( Script & );
extern PixelRectSize ScrReadPixelRectSize( Script & );
extern PixelRect     ScrReadPixelRect    ( Script & );
extern fPIXEL        ScrReadfPixel       ( Script & );
extern fPixelPoint   ScrReadfPixelPoint  ( Script & );

extern void DefineUtilityWrapperFunctions( );
