// Util.ixx : 
//
// Toolbox\BasicUtils

export module Util;

import std;
import std.compat;

using std::vector;
using std::function;
using std::wstring;
using std::abs;
using std::wostringstream;
using std::chrono::system_clock;
using std::put_time;
using std::time_t;
using std::tm;

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

export time_t CurrentTime()
{
    auto   currentTime   { system_clock::now() };
    time_t currentTime_t { system_clock::to_time_t(currentTime) };
    return currentTime_t;
}

export wstring GetCurrentDateAndTime()
{
    wostringstream woss;
	time_t now_time { CurrentTime() };
 //	tm     ltm;
 // localtime_r(&now_time, ltm);               // TODO
 // woss << put_time(ltm, L"%Y-%m-%d %X");
	return woss.str();
}

export extern void UpperCase(wstring& str)
{
	for (auto& c : str)
		c = toupper(c);
}
