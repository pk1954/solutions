// Toolbox\util.h : 
//
// Toolbox\Utilities

#pragma once

#include <functional>
#include <string>
#include <limits>
#include <vector>
#include <streambuf>

using std::vector;
using std::function;
using std::abs;

extern void UpperCase(std::wstring &);

using VisCrit = function <bool()>;

enum class tOnOffAuto : long
{
	on,
	off,
	automatic
};

extern bool ApplyAutoCriterion(tOnOffAuto const, VisCrit const &);

// not needed, makes strange problems 
//template <typename SOURCE_TYPE> int sgn(SOURCE_TYPE val) 
//{
//    return (SOURCE_TYPE(0) < val) - (val < SOURCE_TYPE(0));
//}

inline bool IsCloseToZero(float const f)
{
	return abs(f) < 1.e-10f;
}

template<typename SOURCE_TYPE>
SOURCE_TYPE ClipToMinMax(SOURCE_TYPE const val, SOURCE_TYPE const min, SOURCE_TYPE const max)
{
	if (val > max)
		return max;
	if (val < min)
		return min;
	return val;
}

template<typename SOURCE_TYPE>
bool IsInRange(SOURCE_TYPE const v, SOURCE_TYPE const min, SOURCE_TYPE const max)
{
	if (v > max)
		return false;
	if (v < min)
		return false;
	return true;
}

template<typename T>
bool IsValidIndex(long const index, vector<T> const &vec)
{
	return IsInRange<long>(index, 0L, static_cast<long>(vec.size()-1));
}

template<typename T>
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
