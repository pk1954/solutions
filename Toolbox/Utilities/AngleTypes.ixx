// AngleTypes.ixx
//
// Utilities

module;

#include "BasicTypes.h"
#include "Hertz.h"

export module AngleTypes;

import NamedType;
import SaveCast;

export using Radian  = NamedType<float, struct radian_Parameter >;
export using Degrees = NamedType<float, struct degrees_Parameter >;

export constexpr Radian  operator"" _Radian (const long double r) { return Radian (Cast2Float(r)); }
export constexpr Degrees operator"" _Degrees(const long double d) { return Degrees(Cast2Float(d)); }

export inline float const RADIAN_FACTOR     { static_cast<float>(180.0/pi) };
export inline float const INV_RADIAN_FACTOR { 1.0f/RADIAN_FACTOR };

export Radian  Degrees2Radian(Degrees const d) { return Radian (d.GetValue() * INV_RADIAN_FACTOR); }
export Degrees Radian2Degrees(Radian  const r) { return Degrees(r.GetValue() * RADIAN_FACTOR); }

export Radian  Normalize(Radian  const & r) { return Radian (fmodf(fabsf(r.GetValue()), RADIAN_FACTOR * 2.0f)); }
export Degrees Normalize(Degrees const & d) { return Degrees(fmodf(fabsf(d.GetValue()), 360.0f)); }

export MicroMeter Cos(Radian const r) { return MicroMeter(cosf(r.GetValue())); } 
export MicroMeter Sin(Radian const r) { return MicroMeter(sinf(r.GetValue())); } 
