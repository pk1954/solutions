// fHertz.ixx
//
// Utilities

module;

#include <limits>
#include "BasicTypes.h"

export module fHertz;

import Util;
import NamedType;
import SaveCast;

using std::numeric_limits;

export using fHertz = NamedType< float, struct fHertz_Parameter >;

export fHertz constexpr operator"" _fHertz(long double dl)
{
	return fHertz(Cast2Float(dl));
}

export template <> const wstring TypeAttribute<fHertz>::unit   = L"Hz";
export template <> const float   TypeAttribute<fHertz>::factor = 1.f;

export fMicroSecs PulseDuration(fHertz const freq)
{
	return IsCloseToZero(freq.GetValue()) 
		? fMicroSecs((numeric_limits<float>::max)())
		: fMicroSecs(1e6f / freq.GetValue());
}

export fHertz Frequency(fMicroSecs const us)
{
	return IsCloseToZero(us.GetValue())
		? fHertz((numeric_limits<float>::max)())
		: fHertz(1e6f / us.GetValue());
}
