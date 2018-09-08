// gridPoint.h : 
//

#pragma once

#include <stdlib.h>   // abs
#include <algorithm>  // min/max templates
#include <iostream>
#include <limits.h>
#include <assert.h>
#include <functional>
#include "debug.h"
#include "util.h"
#include "Int24.h"
#include "EvolutionTypes.h"

class GridPoint
{
public:

    GridPoint( ) : x( GP_NULL.x ), y( GP_NULL.y ) {}
	GridPoint( long long const _x, long long const _y )
	{
		ASSERT_SHORT( _x );
		ASSERT_SHORT( _y );
		x = static_cast< GRID_COORD >( _x );
		y = static_cast< GRID_COORD >( _y );
	}

    virtual ~GridPoint() {};

    GridPoint operator++ () { ++x; ++y; return * this; }
    GridPoint operator-- () { --x; --y; return * this; }

    bool      const operator== (GridPoint const & a) const { return (a.x == x) && (a.y == y); }
    bool      const operator!= (GridPoint const & a) const { return (a.x != x) || (a.y != y); }

    GridPoint const operator+= (GridPoint const & a) { x += a.x; y += a.y; return * this; }
    GridPoint const operator-= (GridPoint const & a) { x -= a.x; y -= a.y; return * this; }
    GridPoint const operator/= (GridPoint const & a) { x /= a.x; y /= a.y; return * this; }
    GridPoint const operator%= (GridPoint const & a) { x %= a.x; y %= a.y; return * this; }

    // no * operator!   risk of short overflow

    GridPoint const operator/= (GRID_COORD const l) { x /= l; y /= l; return * this; }
    GridPoint const operator+= (GRID_COORD const l) { x += l; y += l; return * this; }
    GridPoint const operator-= (GRID_COORD const l) { x -= l; y -= l; return * this; }
    GridPoint const operator%= (GRID_COORD const l) { x %= l; y %= l; return * this; }

    bool const operator== (GRID_COORD const i) const { return (x == i) && (y == i); }
    bool const operator!= (GRID_COORD const i) const { return (x != i) || (y != i); }
    bool const operator<= (GRID_COORD const i) const { return (x <= i) || (y <= i); }
    bool const operator<  (GRID_COORD const i) const { return (x <  i) || (y <  i); }
    bool const operator>= (GRID_COORD const i) const { return (x >= i) || (y >= i); }
    bool const operator>  (GRID_COORD const i) const { return (x >  i) || (y >  i); }

	bool IsEvenCol( ) const { return x % 2 == 0; }
	bool IsOddCol ( ) const { return x % 2 != 0; }

    static GRID_COORD const GRID_WIDTH  = 200;
    static GRID_COORD const GRID_HEIGHT = 100;
    static int        const GRID_AREA   = GRID_WIDTH * GRID_HEIGHT;

    static GridPoint const GRID_ORIGIN;
    static GridPoint const GRID_MAXIMUM;
    static GridPoint const GRID_CENTER;
    static GridPoint const GRID_SIZE;
    static GridPoint const GP_NULL;

    bool const IsInGrid( ) const { return (0 <= x) && (0 <= y) && (x < GRID_WIDTH) && (y < GRID_HEIGHT); };

    bool IsNull   ( ) const { return * this == GP_NULL; };
    bool IsNotNull( ) const { return * this != GP_NULL; };

    void Set2Null( ) { * this = GP_NULL; }

    GRID_COORD x;
    GRID_COORD y;
};

inline GridPoint const operator+ (GridPoint const & a, GridPoint const & b) { GridPoint res(a); res += b; return res; }
inline GridPoint const operator- (GridPoint const & a, GridPoint const & b) { GridPoint res(a); res -= b; return res; }
inline GridPoint const operator% (GridPoint const & a, GridPoint const & b) { GridPoint res(a); res %= b; return res; }

inline GridPoint const operator/ (GridPoint const & a, GRID_COORD const l) { GridPoint res(a); res /= l; return res; }
inline GridPoint const operator+ (GridPoint const & a, GRID_COORD const l) { GridPoint res(a); res += l; return res; }
inline GridPoint const operator- (GridPoint const & a, GRID_COORD const l) { GridPoint res(a); res -= l; return res; }
inline GridPoint const operator% (GridPoint const & a, GRID_COORD const l) { GridPoint res(a); res %= l; return res; }

inline GridPoint const Abs(GridPoint const & a ) { return GridPoint( abs(a.x), abs(a.y ) ); }

inline GridPoint const Min(GridPoint const & a, GridPoint const & b) { return GridPoint( min(a.x, b.x), min(a.y, b.y) ); }
inline GridPoint const Max(GridPoint const & a, GridPoint const & b) { return GridPoint( max(a.x, b.x), max(a.y, b.y) ); }

inline bool const Neighbors( GridPoint const & a, GridPoint const & b )
{ 
    GridPoint gpDiff( Abs(a - b) );
    return ((gpDiff.x <= 1) || (gpDiff.x == GridPoint::GRID_WIDTH-1)) && ((gpDiff.y <= 1) || (gpDiff.y == GridPoint::GRID_HEIGHT-1));
}

inline GridPoint const Wrap2Grid(GridPoint const & gp) { return (gp + GridPoint::GRID_SIZE) % GridPoint::GRID_SIZE; }

typedef std::function<void (GridPoint const &       )> GridPointFunc;
typedef std::function<short(short const, short const)> ManipulatorFunc;

std::wostream & operator << ( std::wostream &, GridPoint const & );
