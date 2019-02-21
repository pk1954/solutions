// debug.h : 
//

#pragma once

#ifndef NDEBUG

#include <assert.h>
#include <limits.h>


template<typename SOURCE_TYPE>
void AssertLimits( SOURCE_TYPE const v, SOURCE_TYPE const min, SOURCE_TYPE const max)  
{                                   
	assert( (v) <= (max) );         
	assert( (v) >= (min) );         
}

template<typename SOURCE_TYPE>
void AssertShort( SOURCE_TYPE const value ) 
{
	assert( value <= (std::numeric_limits<short>::max)() );         
	if ( std::numeric_limits<SOURCE_TYPE>::is_signed )
		assert( value >= (std::numeric_limits<short>::min)() );
}

template<typename SOURCE_TYPE>
void AssertInt( SOURCE_TYPE const value ) 
{
	assert( value <= (std::numeric_limits<int>::max)() );         
	if ( std::numeric_limits<SOURCE_TYPE>::is_signed )
		assert( value >= (std::numeric_limits<int>::min)() );
}

template<typename SOURCE_TYPE>
void AssertLong( SOURCE_TYPE const value ) 
{
	assert( value <= (std::numeric_limits<long>::max)() );         
	if ( std::numeric_limits<SOURCE_TYPE>::is_signed )
		assert( value >= (std::numeric_limits<long>::min)() );
}

template<typename SOURCE_TYPE>
constexpr void AssertUnsignedShort( SOURCE_TYPE const value )
{
	assert( value >= (std::numeric_limits<unsigned short>::min)() );
	assert( value <= (std::numeric_limits<unsigned short>::max)() );
}

template<typename SOURCE_TYPE>
void AssertUnsignedInt( SOURCE_TYPE const value )
{
	assert( value >= (std::numeric_limits<unsigned int>::min)() );
	assert( value <= (std::numeric_limits<unsigned int>::max)() );
}

template<typename SOURCE_TYPE>
void AssertUnsignedLong( SOURCE_TYPE const value )
{
	assert( value >= (std::numeric_limits<unsigned long>::min)() );
	assert( value <= (std::numeric_limits<unsigned long>::max)() );
}

template<typename SOURCE_TYPE>
short AssertShortSum( SOURCE_TYPE const a, SOURCE_TYPE const b ) 
{                                
	long sum = static_cast<long>( a ) + static_cast<long>( b ); 
	AssertShort( sum );
	return static_cast<short>( sum );
}

#else
template<typename SOURCE_TYPE> void AssertLimits( SOURCE_TYPE const v, SOURCE_TYPE const min, SOURCE_TYPE const max) {} 
template<typename SOURCE_TYPE> void AssertShort(SOURCE_TYPE const value ) {}
template<typename SOURCE_TYPE> void AssertInt(SOURCE_TYPE const value ) {}
template<typename SOURCE_TYPE> void AssertLong(SOURCE_TYPE const value ) {}
template<typename SOURCE_TYPE> void AssertUnsignedShort(SOURCE_TYPE const value ) {}
template<typename SOURCE_TYPE> void AssertUnsignedInt(SOURCE_TYPE const value ) {}
template<typename SOURCE_TYPE> void AssertUnsignedLong(SOURCE_TYPE const value ) {}
template<typename SOURCE_TYPE> 
short AssertShortSum( SOURCE_TYPE const a, SOURCE_TYPE const b ) 
{
	return a + b;
}
#endif
