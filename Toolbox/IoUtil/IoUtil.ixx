// IoUtil.ixx
//
// Toolbox\IoUtil

module;

#include <Windows.h>

export module IoUtil;

export import Color;
export import :IoBasePeak;
export import :IoPixFpDim;
export import :IoPixelTypes;
export import :Preferences;

import std;
import IoConstants;
import BoolOp;
import SaveCast;
import Types;
import Script;

using std::wostream;

export wostream& operator<< (wostream&, fPixelPoint const&);

export wostream& operator<< (wostream&, MicroMeterPnt    const&);
export wostream& operator<< (wostream&, MicroMeterRect   const&);
export wostream& operator<< (wostream&, MicroMeterCircle const&);
export wostream& operator<< (wostream&, Color            const&);

export fMicroSecs       ScrReadfMicroSecs      (Script&);
export MicroMeter       ScrReadMicroMeter      (Script&);
export MicroMeterPnt    ScrReadMicroMeterPnt   (Script&);
export MicroMeterRect   ScrReadMicroMeterRect  (Script&);
export MicroMeterCircle ScrReadMicroMeterCircle(Script&);
export tBoolOp          ScrReadBoolOp          (Script&);
export Color            ScrReadColor           (Script&);

export template <typename T>
T ScrRead(Script& script)
{
    return static_cast<T>(Cast2Float(script.ScrReadFloat()));
}

export void DefineUtilityWrapperFunctions();
