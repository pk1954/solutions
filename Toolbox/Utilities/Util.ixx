// Util.ixx : 
//
// Toolbox\Utilities

module;

#include <functional>
#include <string>
#include <limits>
#include <vector>
#include <streambuf>

export module Util;

using std::vector;
using std::function;
using std::wstring;
using std::abs;

export extern void UpperCase(wstring &);

export using VisCrit = function <bool()>;

export enum class tOnOffAuto : long
{
	on,
	off,
	automatic
};

export extern bool ApplyAutoCriterion(tOnOffAuto const, VisCrit const &);

// not needed, makes strange problems 
//template <typename SOURCE_TYPE> int sgn(SOURCE_TYPE val) 
//{
//    return (SOURCE_TYPE(0) < val) - (val < SOURCE_TYPE(0));
//}

export inline bool IsCloseToZero(float const f)
{
	return abs(f) < 1.e-10f;
}

export template<typename SOURCE_TYPE>
SOURCE_TYPE ClipToMinMax(SOURCE_TYPE const val, SOURCE_TYPE const min, SOURCE_TYPE const max)
{
	if (val > max)
		return max;
	if (val < min)
		return min;
	return val;
}

export template<typename SOURCE_TYPE>
bool IsInRange(SOURCE_TYPE const v, SOURCE_TYPE const min, SOURCE_TYPE const max)
{
	if (v > max)
		return false;
	if (v < min)
		return false;
	return true;
}

export template<typename T>
bool IsValidIndex(long const index, vector<T> const &vec)
{
	return IsInRange<long>(index, 0L, static_cast<long>(vec.size()-1));
}

export template<typename T>
T GetVectorValue(long const index, vector<T> const &vec)
{
	if (vec.empty())
		return T::NULL_VAL();
	else
	{
		long const clippedIndex { ClipToMinMax(index, 0L, static_cast<long>(vec.size()-1)) }; 
		return vec[clippedIndex];
	}
}
