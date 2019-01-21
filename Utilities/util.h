// util.h : 
//

#pragma once

#include <assert.h>
#include <string>
#include <limits>
#include <streambuf>
#include "debug.h"

using namespace std;

extern void UpperCase( wstring & );

template <typename SOURCE_TYPE> int sgn(SOURCE_TYPE val) 
{
    return (SOURCE_TYPE(0) < val) - (val < SOURCE_TYPE(0));
}

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
unsigned short CastToUnsignedShort(SOURCE_TYPE const value ) 
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

std::wstring DecFraction( unsigned long );
