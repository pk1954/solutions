// UtilityWrappers.ixx
//
// Utilities\Wrappers

module;

#include "Script.h"

export module UtilityWrappers;

import BoolOp;
import SaveCast;
import MoreTypes;
import PixelTypes;

export tBoolOp       ScrReadBoolOp       (Script &);
export PIXEL         ScrReadPixel        (Script &);
export PixelPoint    ScrReadPixelPoint   (Script &);
export PixelRectSize ScrReadPixelRectSize(Script &);
export PixelRect     ScrReadPixelRect    (Script &);
export fPixel        ScrReadfPixel       (Script &);
export fPixelPoint   ScrReadfPixelPoint  (Script &);

export template <typename T>
T ScrRead(Script& script)
{
    return static_cast<T>(Cast2Float(script.ScrReadFloat()));
}

export template<typename T>
BASE_PEAK<T> ScrReadBasePeak(Script & script)
{
    T const base { ScrRead<T>(script) };
    T const peak { ScrRead<T>(script) };
    return BASE_PEAK<T>(base, peak);
}

export void DefineUtilityWrapperFunctions();
