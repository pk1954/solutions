// VoltageType.ixx
//
// Utilities

module;

#include <string>

export module VoltageType;

import NamedType;
import SaveCast;

using std::wstring;

export using mV = NamedType<float, struct mV_Parameter >;

export constexpr mV operator"" _mV(const long double d)
{
	return mV(Cast2Float(d));
}

export template <> const wstring TypeAttribute<mV>::unit = L"V";
export template <> const float   TypeAttribute<mV>::factor = 1.e3f;
