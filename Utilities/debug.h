// debug.h : 
//

#pragma once

#include <assert.h>

#ifndef NDEBUG

#define ASSERT_LIMITS(v, min, max)  \
{                                   \
	assert( (v) <= (max) );         \
	assert( (v) >= (min) );         \
}

#define ASSERT_SHORT(X) ASSERT_LIMITS((X), (numeric_limits<short>::min)(), (numeric_limits<short>::max)())
#define ASSERT_INT(X)   ASSERT_LIMITS((X), (numeric_limits<int>  ::min)(), (numeric_limits<int>  ::max)())

#define ASSERT_USHORT(X) assert((X) <= (numeric_limits<unsigned short>::max)())
#define ASSERT_UINT(X)   assert((X) <= (numeric_limits<unsigned int>  ::max)())

#define ASSERT_SHORT_SUM(a, b) \
{                              \
	long sum = static_cast<long>( a ) + static_cast<long>( b ); \
	ASSERT_SHORT( sum );  \
}

#else
#define ASSERT_LIMITS(v, min, max) 
#define ASSERT_SHORT(i)
#define ASSERT_INT(i)
#define ASSERT_USHORT(i)
#define ASSERT_UINT(i)
#define ASSERT_SHORT_SUM(a,b)
#endif
