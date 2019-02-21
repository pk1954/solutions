// util.h : 
//

#pragma once

#include <assert.h>
#include <string>
#include <limits>
#include <streambuf>
#include "debug.h"

extern void UpperCase( std::wstring & );

// not needed, makes strange problems 
//template <typename SOURCE_TYPE> int sgn(SOURCE_TYPE val) 
//{
//    return (SOURCE_TYPE(0) < val) - (val < SOURCE_TYPE(0));
//}

template<typename SOURCE_TYPE>
SOURCE_TYPE ClipToMinMax(SOURCE_TYPE const val, SOURCE_TYPE const min, SOURCE_TYPE const max)
{
	return ( val > max ) 
		   ? max 
		   : (val < min) 
		     ? min 
		     : val;
}

template<typename SOURCE_TYPE>
short CastToShort(SOURCE_TYPE const value ) 
{
	AssertShort( value );
	return static_cast<short>( value );
}

template<typename SOURCE_TYPE>
constexpr const unsigned short CastToUnsignedShort(SOURCE_TYPE const value ) 
{
	AssertUnsignedShort( value );
	return static_cast<unsigned short>( value );
}

template<typename SOURCE_TYPE>
int CastToInt(SOURCE_TYPE const value ) 
{
	AssertInt( value );
	return static_cast<int>( value );
}

template<typename SOURCE_TYPE>
unsigned int CastToUnsignedInt(SOURCE_TYPE const value ) 
{
	AssertUnsignedInt( value );
	return static_cast<unsigned int>( value );
}

template<typename SOURCE_TYPE>
int CastToLong(SOURCE_TYPE const value ) 
{
	AssertLong( value );
	return static_cast<long>( value );
}

template<typename SOURCE_TYPE>
unsigned int CastToUnsignedLong(SOURCE_TYPE const value ) 
{
	AssertUnsignedLong( value );
	return static_cast<unsigned long>( value );
}

std::wstring DecFraction( unsigned long );
