// debug.h : 
//

#pragma once

#ifndef NDEBUG

#include <assert.h>
#include <limits.h>

#define ASSERT_LIMITS(v, min, max)  \
{                                   \
	assert( (v) <= (max) );         \
	assert( (v) >= (min) );         \
}

template<typename SOURCE_TYPE>
void AssertShort( SOURCE_TYPE const value ) 
{
	assert( value <= (numeric_limits<short>::max)() );         
	if ( numeric_limits<SOURCE_TYPE>::is_signed )
		assert( value >= (numeric_limits<short>::min)() );
}

template<typename SOURCE_TYPE>
void AssertInt( SOURCE_TYPE const value ) 
{
	assert( value <= (numeric_limits<int>::max)() );         
	if ( numeric_limits<SOURCE_TYPE>::is_signed )
		assert( value >= (numeric_limits<int>::min)() );
}

template<typename SOURCE_TYPE>
void AssertUnsignedShort( SOURCE_TYPE const value )
{
	assert( value >= (numeric_limits<unsigned short>::min)() );
	assert( value <= (numeric_limits<unsigned short>::max)() );
}

template<typename SOURCE_TYPE>
void AssertUnsignedInt( SOURCE_TYPE const value )
{
	assert( value >= (numeric_limits<unsigned int>::min)() );
	assert( value <= (numeric_limits<unsigned int>::max)() );
}

#define ASSERT_SHORT_SUM( a, b ) \
{                                \
	long sum = static_cast<long>( a ) + static_cast<long>( b ); \
	AssertShort( sum );  \
}

#else
#define ASSERT_LIMITS(v, min, max) 
template<typename SOURCE_TYPE> void AssertShort(SOURCE_TYPE const value ) {}
template<typename SOURCE_TYPE> void AssertInt(SOURCE_TYPE const value ) {}
template<typename SOURCE_TYPE> void AssertUnsignedShort(SOURCE_TYPE const value ) {}
template<typename SOURCE_TYPE> void AssertUnsignedInt(SOURCE_TYPE const value ) {}
#define ASSERT_SHORT_SUM(a,b)
#endif
