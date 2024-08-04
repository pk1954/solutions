// SaveCast.ixx
//
// Toolbox\BasicUtils

export module SaveCast;

import std.compat;
import Debug;

using std::uint8_t;

export template<typename SOURCE_TYPE>
constexpr float Cast2Float(SOURCE_TYPE const value) 
{
	AssertFloat(value);
	return static_cast<float>(value);
}

export template<typename SOURCE_TYPE>
constexpr uint8_t Cast2Byte(SOURCE_TYPE const value) 
{
	AssertByte(value);
	return static_cast<uint8_t>(value);
}

export template<typename SOURCE_TYPE>
constexpr short Cast2Short(SOURCE_TYPE const value) 
{
	AssertShort(value);
	return static_cast<short>(value);
}

export template<typename SOURCE_TYPE>
constexpr unsigned short Cast2UnsignedShort(SOURCE_TYPE const value) 
{
	AssertUnsignedShort(value);
	return static_cast<unsigned short>(value);
}

export template<typename SOURCE_TYPE>
constexpr int Cast2Int(SOURCE_TYPE const value) 
{
	AssertInt(value);
	return static_cast<int>(value);
}

export template<typename SOURCE_TYPE>
constexpr unsigned int Cast2UnsignedInt(SOURCE_TYPE const value) 
{
	AssertUnsignedInt(value);
	return static_cast<unsigned int>(value);
}

export template<typename SOURCE_TYPE>
constexpr long Cast2Long(SOURCE_TYPE const value) 
{
	AssertLong(value);
	return static_cast<long>(value);
}

export template<typename SOURCE_TYPE>
constexpr unsigned long Cast2UnsignedLong(SOURCE_TYPE const value) 
{
	AssertUnsignedLong(value);
	return static_cast<unsigned long>(value);
}
