// IoUtil.ixx
//
// Toolbox\IoUtil

module;

#include <compare>
#include <iostream>

export module IoUtil;

export import :IoConstants;
export import :IoBasePeak;
export import :IoPixFpDim;
export import :IoPixelTypes;

import BoolOp;
import SaveCast;
import Types;
import Script;

using std::wostream;

export wostream& operator<< (wostream&, fPixelPoint const&);

export wostream& operator<< (wostream&, MicroMeterPnt    const&);
export wostream& operator<< (wostream&, MicroMeterRect   const&);
export wostream& operator<< (wostream&, MicroMeterCircle const&);

export fMicroSecs       ScrReadfMicroSecs      (Script&);
export MicroMeter       ScrReadMicroMeter      (Script&);
export MicroMeterPnt    ScrReadMicroMeterPnt   (Script&);
export MicroMeterCircle ScrReadMicroMeterCircle(Script&);
export tBoolOp          ScrReadBoolOp          (Script&);

export template <typename T>
T ScrRead(Script& script)
{
    return static_cast<T>(Cast2Float(script.ScrReadFloat()));
}

export void DefineUtilityWrapperFunctions();
