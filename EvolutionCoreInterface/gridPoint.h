// gridPoint.h : 
//
// library EvolutionCoreInterface

#pragma once

#include <stdlib.h>   // abs
#include <algorithm>  // min/max templates
#include <iostream>
#include <iomanip>
#include <limits.h>
#include <assert.h>
#include <functional>
#include "debug.h"
#include "util.h"
#include "Int24.h"
#include "gridCoord.h"

class GridPoint
{
public:

    GridPoint( )                                          : x( UNDEF().x ), y( UNDEF().y ) {}
	GridPoint( GRID_COORD const _x, GRID_COORD const _y ) : x(_x), y(_y) {}

    virtual ~GridPoint() {};

    bool      const operator== (GridPoint const a) const { return (x == a.x) && (y == a.y); }
    bool      const operator!= (GridPoint const a) const { return (x != a.x) || (y != a.y); }

    GridPoint const operator+= (GridPoint const a) { x += a.x; y += a.y; return * this; }
    GridPoint const operator-= (GridPoint const a) { x -= a.x; y -= a.y; return * this; }
    GridPoint const operator%= (GridPoint const a) { x %= a.x; y %= a.y; return * this; }

    GridPoint const operator+= (GRID_COORD const l) { x += l; y += l; return * this; }
    GridPoint const operator-= (GRID_COORD const l) { x -= l; y -= l; return * this; }

	GridPoint const operator%= (int const i) { x %= i; y %= i; return * this; }
    GridPoint const operator/= (int const i) { x /= i; y /= i; return * this; }

	GRID_COORD const GetX() const { return x; }
	GRID_COORD const GetY() const { return y; }

	short const GetXshort() const { return x.GetValue(); }
	short const GetYshort() const { return y.GetValue(); }

	bool IsEvenColumn( ) const { return IsEven( x ); }
	bool IsOddColumn ( ) const { return IsOdd( x ); }

    inline static int const GRID_AREA() 
	{ 
		static int res = GRID_WIDTH.GetValue() * GRID_HEIGHT.GetValue(); 
		return res;
	};

	inline static GridPoint const & UNDEF() 
	{ 
		static GridPoint res = GridPoint( GRID_COORD::UNDEF(), GRID_COORD::UNDEF() ); 
		return res;
	};

	inline static GridPoint const & GRID_ORIGIN() 
	{ 
		static GridPoint res = GridPoint( GRID_X_MIN,  GRID_Y_MIN ); 
		return res;
	};

	inline static GridPoint const & GRID_MAXIMUM() 
	{ 
		static GridPoint res = GridPoint( GRID_X_MAX, GRID_Y_MAX ); 
		return res;
	};

	inline static GridPoint const & GRID_SIZE() 
	{ 
		static GridPoint res = GridPoint( GRID_WIDTH, GRID_HEIGHT ); 
		return res;
	};

    bool IsNull   ( ) const { return * this == UNDEF(); };
    bool IsNotNull( ) const { return * this != UNDEF(); };

    void Set2Null( ) { * this = UNDEF(); }

private:
    GRID_COORD x;
    GRID_COORD y;
};

inline GridPoint const operator+ (GridPoint const a, GridPoint const b) { GridPoint res(a); res += b; return res; }
inline GridPoint const operator- (GridPoint const a, GridPoint const b) { GridPoint res(a); res -= b; return res; }
inline GridPoint const operator% (GridPoint const a, GridPoint const b) { GridPoint res(a); res %= b; return res; }

inline GridPoint const operator+ (GridPoint const a, GRID_COORD const l) { GridPoint res(a); res += l; return res; }
inline GridPoint const operator- (GridPoint const a, GRID_COORD const l) { GridPoint res(a); res -= l; return res; }

inline GridPoint const operator% (GridPoint const a, int const i) { GridPoint res(a); res %= i; return res; }
inline GridPoint const operator/ (GridPoint const a, int const i) { GridPoint res(a); res /= i; return res; }

inline GridPoint const abs(GridPoint const a) { return GridPoint( a.GetX().abs_value(), a.GetY().abs_value() ); }

inline GridPoint const Min(GridPoint const a, GridPoint const b) { return GridPoint( min(a.GetX(), b.GetX()), min(a.GetY(), b.GetY()) ); }
inline GridPoint const Max(GridPoint const a, GridPoint const b) { return GridPoint( max(a.GetX(), b.GetX()), max(a.GetY(), b.GetY()) ); }

inline bool const Neighbors( GridPoint const a, GridPoint const b )
{ 
    GridPoint gpDiff( abs(a - b) );
    return ( 
		      ((gpDiff.GetX() <= GRID_COORD(1_GRID_COORD)) || (gpDiff.GetX() == GRID_X_MAX)) && 
		      ((gpDiff.GetY() <= GRID_COORD(1_GRID_COORD)) || (gpDiff.GetY() == GRID_Y_MAX))
		   );
}

inline GridPoint const Wrap2Grid(GridPoint const gp) 
{ 
	return (gp + GridPoint::GRID_SIZE()) % GridPoint::GRID_SIZE(); 
}

inline bool const IsInGrid( GridPoint const & gp ) 
{ 
	return (GRID_X_MIN <= gp.GetX()) && (gp.GetX() <= GRID_X_MAX) && 
		   (GRID_Y_MIN <= gp.GetY()) && (gp.GetY() <= GRID_Y_MAX);
};

typedef std::function<void (GridPoint const)> GridPointFunc;
typedef std::function<short(short const, short const)> ManipulatorFunc;

inline std::wostream & operator << ( std::wostream & out, GridPoint const gp )
{
//lint -e747  Significant prototype coercion with setw
    out << L" " << std::setw(3) << gp.GetX() << L" " << std::setw(3) << gp.GetY() << L" ";
    return out;
}
