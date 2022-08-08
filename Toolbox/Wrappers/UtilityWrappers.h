// UtilityWrappers.h
//
// Utilities\Wrappers

#pragma once

#include "Script.h"

import BoolOp;
import SaveCast;
import MoreTypes;
import PixelTypes;

class Script;

tBoolOp       ScrReadBoolOp       (Script &);
PIXEL         ScrReadPixel        (Script &);
PixelPoint    ScrReadPixelPoint   (Script &);
PixelRectSize ScrReadPixelRectSize(Script &);
PixelRect     ScrReadPixelRect    (Script &);
fPixel        ScrReadfPixel       (Script &);
fPixelPoint   ScrReadfPixelPoint  (Script &);

template <typename T>
T ScrRead(Script& script)
{
    return static_cast<T>(Cast2Float(script.ScrReadFloat()));
}

template<typename T>
BASE_PEAK<T> ScrReadBasePeak(Script & script)
{
    T const base { ScrRead<T>(script) };
    T const peak { ScrRead<T>(script) };
    return BASE_PEAK<T>(base, peak);
}

void DefineUtilityWrapperFunctions();
