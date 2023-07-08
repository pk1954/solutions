// Util.ixx : 
//
// Toolbox\Utilities

module;

#include <functional>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <streambuf>
#include <chrono>
#include <ctime>

export module Util;

using std::vector;
using std::function;
using std::wstring;
using std::abs;
using std::wstringstream;
using std::chrono::system_clock;
using std::time_t;

export extern void UpperCase(wstring &);

export using VisCrit = function <bool()>;

export enum class tHorzDir : bool {	left, right };
export enum class tVertDir : bool {	up, down };

export enum class tOnOffAuto : long
{
	on,
	off,
	automatic
};

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

export bool ApplyAutoCriterion
(
	tOnOffAuto const   onOffAuto,
	VisCrit    const& crit
)
{
	if (onOffAuto == tOnOffAuto::automatic)
		return crit();
	else
		return onOffAuto == tOnOffAuto::on;
}

export wstring GetCurrentDateAndTime()
{
	auto   now      { system_clock::now() };
	time_t now_time { system_clock::to_time_t(now) };
	char buffer[26];
	ctime_s(buffer, sizeof(buffer), &now_time);
	wstringstream wss;
	wss << buffer;
	return wss.str();
}
