// Percent.ixx
//
// Toolbox\Types

export module Types:Percent;

import std;
import :NamedType;
import SaveCast;

using std::wstring;
using std::to_wstring;

export using PERCENT = NamedType<short, struct PERCENT_Parameter>;

export PERCENT constexpr operator"" _PERCENT(unsigned long long ull)
{
	return PERCENT(Cast2Short(ull));
}

export template <typename T>
PERCENT CalcPercent(T const a, T const b)
{
	return PERCENT(Cast2Short(a * 100 / b));
}

export wstring Percent2wstring(PERCENT const percent)
{
	return to_wstring(percent.GetValue()) + L"%";
}
