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

template<typename T>
T ClipToMinMax(T const val, T const min, T const max)
{
	return ( val > max ) 
		   ? max 
		   : (val < min) 
		     ? min 
		     : val;
}

template<typename T>
short CastToShort(T const value ) 
{
	ASSERT_SHORT( value );
	return static_cast<short>( value );
}

template<typename T>
short CastToInt(T const value ) 
{
	ASSERT_INT( value );
	return static_cast<int>( value );
}
