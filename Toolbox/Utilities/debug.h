// debug.h : 
//

#pragma once

#ifndef NDEBUG

#include <assert.h>
#include <limits.h>

template<typename SOURCE_TYPE>
constexpr void AssertLimits(SOURCE_TYPE const v, SOURCE_TYPE const min, SOURCE_TYPE const max)  
{                                   
	assert((v) <= (max));         
	assert((v) >= (min));         
}

//TODO: try to replace with one generalized template Assert<DST_TYPE>(SRC_TYPE const) ...
//      use std::conditional ??

template<typename SOURCE_TYPE>
constexpr void AssertFloat(SOURCE_TYPE const value) 
{
	assert(static_cast<long double>(value) <= static_cast<long double>((std::numeric_limits<float>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		assert(static_cast<long double>(value) >= static_cast<long double>((std::numeric_limits<float>::lowest)()));         
}

template<typename SOURCE_TYPE>
constexpr void AssertShort(SOURCE_TYPE const value) 
{
	assert(static_cast<long long>(value) <= static_cast<long long>((std::numeric_limits<short>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		assert(static_cast<long long>(value) >= static_cast<long long>((std::numeric_limits<short>::min)()));         
}

template<typename SOURCE_TYPE>
constexpr void AssertInt(SOURCE_TYPE const value) 
{
	assert(static_cast<long long>(value) <= static_cast<long long>((std::numeric_limits<int>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		assert(static_cast<long long>(value) >= static_cast<long long>((std::numeric_limits<int>::min)()));         
}

template<typename SOURCE_TYPE>
constexpr void AssertLong(SOURCE_TYPE const value) 
{
	assert(static_cast<long long>(value) <= static_cast<long long>((std::numeric_limits<long>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		assert(static_cast<long long>(value) >= static_cast<long long>((std::numeric_limits<long>::min)()));
}

template<typename SOURCE_TYPE>
constexpr void AssertUnsignedShort(SOURCE_TYPE const value)
{
	assert(value >= 0);
	assert(static_cast<unsigned short>(value) <= (std::numeric_limits<unsigned short>::max)());
}

template<typename SOURCE_TYPE>
constexpr void AssertUnsignedInt(SOURCE_TYPE const value)
{
	assert(value >= 0);
	assert(static_cast<unsigned int>(value) <= (std::numeric_limits<unsigned int>::max)());
}

template<typename SOURCE_TYPE>
constexpr void AssertUnsignedLong(SOURCE_TYPE const value)
{
	assert(value >= 0);
	assert(static_cast<unsigned long>(value) <= (std::numeric_limits<unsigned long>::max)());
}

template<typename SOURCE_TYPE>
constexpr short AssertShortSum(SOURCE_TYPE const a, SOURCE_TYPE const b) 
{                                
	long sum = static_cast<long>(a) + static_cast<long>(b); 
	AssertShort(sum);
	return static_cast<short>(sum);
}

#else
template<typename SOURCE_TYPE> constexpr void AssertLimits(SOURCE_TYPE const v, SOURCE_TYPE const min, SOURCE_TYPE const max) {} 
template<typename SOURCE_TYPE> constexpr void AssertFloat        (SOURCE_TYPE const value) {}
template<typename SOURCE_TYPE> constexpr void AssertShort        (SOURCE_TYPE const value) {}
template<typename SOURCE_TYPE> constexpr void AssertInt          (SOURCE_TYPE const value) {}
template<typename SOURCE_TYPE> constexpr void AssertLong         (SOURCE_TYPE const value) {}
template<typename SOURCE_TYPE> constexpr void AssertUnsignedShort(SOURCE_TYPE const value) {}
template<typename SOURCE_TYPE> constexpr void AssertUnsignedInt  (SOURCE_TYPE const value) {}
template<typename SOURCE_TYPE> constexpr void AssertUnsignedLong (SOURCE_TYPE const value) {}
template<typename SOURCE_TYPE> 
constexpr short AssertShortSum(SOURCE_TYPE const a, SOURCE_TYPE const b) 
{
	return a + b;
}
#endif
