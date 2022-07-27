// SaveCast.h : 
//
// Toolbox\Utilities

#pragma once

#include "debug.h"

template<typename SOURCE_TYPE>
constexpr float Cast2Float(SOURCE_TYPE const value) 
{
	AssertFloat(value);
	return static_cast<float>(value);
}

template<typename SOURCE_TYPE>
constexpr short Cast2Short(SOURCE_TYPE const value) 
{
	AssertShort(value);
	return static_cast<short>(value);
}

template<typename SOURCE_TYPE>
constexpr unsigned short Cast2UnsignedShort(SOURCE_TYPE const value) 
{
	AssertUnsignedShort(value);
	return static_cast<unsigned short>(value);
}

template<typename SOURCE_TYPE>
constexpr int Cast2Int(SOURCE_TYPE const value) 
{
	AssertInt(value);
	return static_cast<int>(value);
}

template<typename SOURCE_TYPE>
constexpr unsigned int Cast2UnsignedInt(SOURCE_TYPE const value) 
{
	AssertUnsignedInt(value);
	return static_cast<unsigned int>(value);
}

template<typename SOURCE_TYPE>
constexpr long Cast2Long(SOURCE_TYPE const value) 
{
	AssertLong(value);
	return static_cast<long>(value);
}

template<typename SOURCE_TYPE>
constexpr unsigned long Cast2UnsignedLong(SOURCE_TYPE const value) 
{
	AssertUnsignedLong(value);
	return static_cast<unsigned long>(value);
}
