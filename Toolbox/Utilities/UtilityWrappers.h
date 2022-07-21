// UtilityWrappers.h
//
// Utilities

#pragma once

#include "PixelTypes.h"

import BoolOp;

class Script;

extern tBoolOp       ScrReadBoolOp       (Script &);
extern PIXEL         ScrReadPixel        (Script &);
extern PixelPoint    ScrReadPixelPoint   (Script &);
extern PixelRectSize ScrReadPixelRectSize(Script &);
extern PixelRect     ScrReadPixelRect    (Script &);
extern fPixel        ScrReadfPixel       (Script &);
extern fPixelPoint   ScrReadfPixelPoint  (Script &);

template <typename T>
extern T ScrRead(Script& script)
{
    return static_cast<T>(Cast2Float(script.ScrReadFloat()));
}

template<typename T>
extern BASE_PEAK<T> ScrReadBasePeak(Script & script)
{
    T const base { ScrRead<T>(script) };
    T const peak { ScrRead<T>(script) };
    return BASE_PEAK<T>(base, peak);
}

extern void DefineUtilityWrapperFunctions();
