// MilliSecs.ixx
//
// Utilities

export module MilliSecs;

import NamedType;
import SaveCast;

export using MilliSecs = NamedType< unsigned long, struct MilliSecs_Parameter >;

export constexpr MilliSecs operator"" _MilliSecs(const unsigned long long d)
{
	return MilliSecs(Cast2UnsignedLong(d));
}
