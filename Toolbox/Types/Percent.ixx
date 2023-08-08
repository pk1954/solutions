// Percent.ixx
//
// Toolbox\Types

export module Types:Percent;

import :NamedType;
import SaveCast;

export using PERCENT = NamedType< short, struct PERCENT_Parameter >;

export PERCENT constexpr operator"" _PERCENT(unsigned long long ull)
{
	return PERCENT(Cast2Short(ull));
}
