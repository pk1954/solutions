// Debug.ixx 
//
// Toolbox\BasicUtils

export module Debug;

import std;
import FatalError;

using std::uint8_t;

#ifndef NDEBUG

export void Assert(bool const b)
{
	if (!b)
		FatalError::Happened(1, L"assertion failed");
}

export template<typename SOURCE_TYPE>
constexpr void AssertLimits(SOURCE_TYPE const v, SOURCE_TYPE const min, SOURCE_TYPE const max)  
{                                   
	Assert((v) <= (max));         
	Assert((v) >= (min));         
}

//TODO: try to replace with one generalized template Assert<DST_TYPE>(SRC_TYPE const) ...
//      use std::conditional ??

export template<typename SOURCE_TYPE>
constexpr void AssertFloat(SOURCE_TYPE const value) 
{
	Assert(static_cast<long double>(value) <= static_cast<long double>((std::numeric_limits<float>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		Assert(static_cast<long double>(value) >= static_cast<long double>((std::numeric_limits<float>::lowest)()));         
}

export template<typename SOURCE_TYPE>
constexpr void AssertByte(SOURCE_TYPE const value) 
{
	Assert(static_cast<long long>(value) <= static_cast<long long>((std::numeric_limits<uint8_t>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		Assert(static_cast<long long>(value) >= static_cast<long long>((std::numeric_limits<uint8_t>::min)()));         
}

export template<typename SOURCE_TYPE>
constexpr void AssertShort(SOURCE_TYPE const value) 
{
	Assert(static_cast<long long>(value) <= static_cast<long long>((std::numeric_limits<short>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		Assert(static_cast<long long>(value) >= static_cast<long long>((std::numeric_limits<short>::min)()));         
}

export template<typename SOURCE_TYPE>
constexpr void AssertSignedChar(SOURCE_TYPE const value) 
{
	Assert(static_cast<long long>(value) <= static_cast<long long>((std::numeric_limits<signed char>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		Assert(static_cast<long long>(value) >= static_cast<long long>((std::numeric_limits<signed char>::min)()));         
}

export template<typename SOURCE_TYPE>
constexpr void AssertInt(SOURCE_TYPE const value) 
{
	Assert(static_cast<long long>(value) <= static_cast<long long>((std::numeric_limits<int>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		Assert(static_cast<long long>(value) >= static_cast<long long>((std::numeric_limits<int>::min)()));         
}

export template<typename SOURCE_TYPE>
constexpr void AssertLong(SOURCE_TYPE const value) 
{
	Assert(static_cast<long long>(value) <= static_cast<long long>((std::numeric_limits<long>::max)()));         
	if (std::numeric_limits<SOURCE_TYPE>::is_signed)
		Assert(static_cast<long long>(value) >= static_cast<long long>((std::numeric_limits<long>::min)()));
}

export template<typename SOURCE_TYPE>
constexpr void AssertUnsignedShort(SOURCE_TYPE const value)
{
	Assert(value >= 0);
	Assert(static_cast<unsigned short>(value) <= (std::numeric_limits<unsigned short>::max)());
}

export template<typename SOURCE_TYPE>
constexpr void AssertUnsignedInt(SOURCE_TYPE const value)
{
	Assert(value >= 0);
	Assert(static_cast<unsigned int>(value) <= (std::numeric_limits<unsigned int>::max)());
}

export template<typename SOURCE_TYPE>
constexpr void AssertUnsignedLong(SOURCE_TYPE const value)
{
	Assert(value >= 0);
	Assert(static_cast<unsigned long>(value) <= (std::numeric_limits<unsigned long>::max)());
}

export template<typename SOURCE_TYPE>
constexpr short AssertShortSum(SOURCE_TYPE const a, SOURCE_TYPE const b) 
{                                
	long sum = static_cast<long>(a) + static_cast<long>(b); 
	AssertShort(sum);
	return static_cast<short>(sum);
}

#else
export void Assert(bool const) {};
export template<typename SOURCE_TYPE> constexpr void AssertLimits(SOURCE_TYPE const v, SOURCE_TYPE const min, SOURCE_TYPE const max) {} 
export template<typename SOURCE_TYPE> constexpr void AssertFloat        (SOURCE_TYPE const value) {}
export template<typename SOURCE_TYPE> constexpr void AssertByte         (SOURCE_TYPE const value) {}
export template<typename SOURCE_TYPE> constexpr void AssertShort        (SOURCE_TYPE const value) {}
export template<typename SOURCE_TYPE> constexpr void AssertSignedChar   (SOURCE_TYPE const value) {}
export template<typename SOURCE_TYPE> constexpr void AssertInt          (SOURCE_TYPE const value) {}
export template<typename SOURCE_TYPE> constexpr void AssertLong         (SOURCE_TYPE const value) {}
export template<typename SOURCE_TYPE> constexpr void AssertUnsignedShort(SOURCE_TYPE const value) {}
export template<typename SOURCE_TYPE> constexpr void AssertUnsignedInt  (SOURCE_TYPE const value) {}
export template<typename SOURCE_TYPE> constexpr void AssertUnsignedLong (SOURCE_TYPE const value) {}
export template<typename SOURCE_TYPE> 
constexpr short AssertShortSum(SOURCE_TYPE const a, SOURCE_TYPE const b) 
{
	return a + b;
}
#endif
