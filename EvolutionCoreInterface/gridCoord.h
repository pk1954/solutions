// gridCoord.h
//

#pragma once

#include <iostream>

class GRID_COORD
{
public:
    explicit GRID_COORD( ) 
		: value_(0) 
	{}
		
    constexpr explicit GRID_COORD( short const & value ) 
		: value_(value) 
	{}

    short const& get() const { return value_; }

    GRID_COORD operator++ () { ++value_; return * this; }
    GRID_COORD operator-- () { --value_; return * this; }

    GRID_COORD const operator- () const { return GRID_COORD( -value_ ); }

    bool const operator== (GRID_COORD const a) const { return value_ == a.value_; }
    bool const operator!= (GRID_COORD const a) const { return value_ != a.value_; }
    bool const operator<= (GRID_COORD const a) const { return value_ <= a.value_; }
    bool const operator<  (GRID_COORD const a) const { return value_ <  a.value_; }
    bool const operator>= (GRID_COORD const a) const { return value_ >= a.value_; }
    bool const operator>  (GRID_COORD const a) const { return value_ >  a.value_; }

	GRID_COORD const operator+= (GRID_COORD const a) { value_ += a.value_; return * this; }
    GRID_COORD const operator-= (GRID_COORD const a) { value_ -= a.value_; return * this; }
    GRID_COORD const operator%= (GRID_COORD const a) { value_ %= a.value_; return * this; }

    // no * operator!   risk of short overflow

	GRID_COORD const operator/= (int const i) { value_ /= i; return * this; }
    GRID_COORD const operator%= (int const i) { value_ %= i; return * this; }

	bool IsEven( ) const { return value_ % 2 == 0; }
	bool IsOdd ( ) const { return value_ % 2 != 0; }

private:
    short value_;
};

inline GRID_COORD const abs(GRID_COORD const a) { return GRID_COORD( ::abs(a.get()) ); }

inline GRID_COORD const operator+ (GRID_COORD const a, GRID_COORD const b) { GRID_COORD res(a); res += b; return res; }
inline GRID_COORD const operator- (GRID_COORD const a, GRID_COORD const b) { GRID_COORD res(a); res -= b; return res; }

constexpr GRID_COORD operator"" _GRID_COORD( unsigned long long );

std::wostream & operator << ( std::wostream &, GRID_COORD const );

GRID_COORD const MAX_GRID_COORD  = GRID_COORD(255_GRID_COORD);
GRID_COORD const GRID_COORD_NULL = GRID_COORD(SHRT_MIN);

static long const GRID_WIDTH_  = 200;
static long const GRID_HEIGHT_ = 100;
GRID_COORD const GRID_WIDTH  = GRID_COORD(200_GRID_COORD);
GRID_COORD const GRID_HEIGHT = GRID_COORD(100_GRID_COORD);
GRID_COORD const GRID_X_MIN  = GRID_COORD(0_GRID_COORD);
GRID_COORD const GRID_Y_MIN  = GRID_COORD(0_GRID_COORD);
GRID_COORD const GRID_X_MAX  = GRID_WIDTH  - GRID_COORD(1_GRID_COORD);
GRID_COORD const GRID_Y_MAX  = GRID_HEIGHT - GRID_COORD(1_GRID_COORD);
