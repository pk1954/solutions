// VoltageType.h
//
// Utilities

#pragma once

#include <string>

import NamedType;
import SaveCast;

using std::wstring;

using mV = NamedType<float, struct mV_Parameter >;

constexpr mV operator"" _mV(const long double d)
{ 
	return mV(Cast2Float(d));
}

template <> const wstring TypeAttribute<mV>::unit   = L"V";
template <> const float   TypeAttribute<mV>::factor = 1.e3f;
