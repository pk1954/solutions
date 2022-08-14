// Percent.h
//
// Utilities

#pragma once

import NamedType;
import SaveCast;

using PERCENT = NamedType< short, struct PERCENT_Parameter >;

PERCENT constexpr operator"" _PERCENT(unsigned long long ull)
{
	return PERCENT(Cast2Short(ull));
}
