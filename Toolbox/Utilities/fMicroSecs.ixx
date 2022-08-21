// fMicroSecs.ixx
//
// Toolbox\Types

module;

#include <string>

export module fMicroSecs;

import NamedType;
import SaveCast;

using std::wstring;

export using fMicroSecs = NamedType< float, struct MicroSecs_Parameter >;

export constexpr fMicroSecs operator"" _MicroSecs(const long double d)
{
	return fMicroSecs(Cast2Float(d));
}

export template <> const wstring TypeAttribute<fMicroSecs>::unit   = L"s";
export template <> const float   TypeAttribute<fMicroSecs>::factor = 1.e6f;
