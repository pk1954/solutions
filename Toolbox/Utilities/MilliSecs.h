// MilliSecs.h
//
// Utilities

#pragma once

import NamedType;
import SaveCast;

using MilliSecs = NamedType< unsigned long, struct MilliSecs_Parameter >;

constexpr MilliSecs operator"" _MilliSecs(const unsigned long long d)
{
	return MilliSecs(Cast2UnsignedLong(d));
}
