// pixel.h
//

#pragma once

#include <limits.h>
#include <stdlib.h>    // abs
#include <sstream>

class PIXEL
{
public:
    explicit PIXEL( ) 
		: value_(0) 
	{}
		
    constexpr explicit PIXEL( long const value ) 
		: value_(value) 
	{}

    long const& get() const { return value_; }

    PIXEL operator++ () { ++value_; return * this; }
    PIXEL operator-- () { --value_; return * this; }

    PIXEL const operator- () const { return PIXEL( -value_ ); }

    bool const operator== (PIXEL const a) const { return value_ == a.value_; }
    bool const operator!= (PIXEL const a) const { return value_ != a.value_; }
    bool const operator<= (PIXEL const a) const { return value_ <= a.value_; }
    bool const operator<  (PIXEL const a) const { return value_ <  a.value_; }
    bool const operator>= (PIXEL const a) const { return value_ >= a.value_; }
    bool const operator>  (PIXEL const a) const { return value_ >  a.value_; }

	PIXEL const operator+= (PIXEL const a) { value_ += a.value_; return * this; }
    PIXEL const operator-= (PIXEL const a) { value_ -= a.value_; return * this; }
    PIXEL const operator%= (PIXEL const a) { value_ %= a.value_; return * this; }

	PIXEL const operator*= (int const i) { value_ *= i; return * this; }
	PIXEL const operator/= (int const i) { value_ /= i; return * this; }
    PIXEL const operator%= (int const i) { value_ %= i; return * this; }

private:
    long value_;
};

inline PIXEL const abs(PIXEL const a) { return PIXEL{ ::abs(a.get()) }; }

inline PIXEL const operator+ (PIXEL const a, PIXEL const b) { PIXEL res(a); res += b; return res; }
inline PIXEL const operator- (PIXEL const a, PIXEL const b) { PIXEL res(a); res -= b; return res; }

inline PIXEL const operator* (PIXEL const a, long const l) { PIXEL res(a); res *= l; return res; }
inline PIXEL const operator/ (PIXEL const a, long const l) { PIXEL res(a); res /= l; return res; }

PIXEL operator"" _PIXEL( unsigned long long );

std::wostream & operator << ( std::wostream &, PIXEL const );

PIXEL const PIXEL_NULL = PIXEL(LONG_MIN);
