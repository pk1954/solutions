// gridPoint.h : 
//

#pragma once

#include <iostream>
#include <limits.h>
#include <assert.h>
#include "EvolutionTypes.h"

class GridPoint
{
public:
    GridPoint( ) : x( GP_NULL.x ), y( GP_NULL.y ) {}
	GridPoint( long long const _x, long long const _y )
	{
		assert( _x <= SHRT_MAX );
		assert( _x >= SHRT_MIN );
		assert( _y <= SHRT_MAX );
		assert( _y >= SHRT_MIN );
		x = static_cast< GRID_COORD >( _x );
		y = static_cast< GRID_COORD >( _y );
	}

	GridPoint( unsigned short const usParam )
    	: x ( usParam >> 8 ),
	      y ( usParam & 0x00ff )
	{ }

	unsigned short Pack( ) const
	{
		assert( x < GRID_WIDTH );
		assert( y < GRID_HEIGHT );
		return ( x << 8 ) | y;
	}

    virtual ~GridPoint() {};

    bool      const operator== (GridPoint const & a) const { return (a.x == x) && (a.y == y); }
    bool      const operator!= (GridPoint const & a) const { return (a.x != x) || (a.y != y); }

    GridPoint const operator+= (GridPoint const & a) { x += a.x; y += a.y; return * this; }
    GridPoint const operator-= (GridPoint const & a) { x -= a.x; y -= a.y; return * this; }
    GridPoint const operator/= (GridPoint const & a) { x /= a.x; y /= a.y; return * this; }
    GridPoint const operator%= (GridPoint const & a) { x %= a.x; y %= a.y; return * this; }

    // no * operator!   danger of short overflow

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

inline short           shrt_abs( short const s )  { return (s < 0) ? -s : s; }
inline GridPoint const Abs( GridPoint const & a ) { return GridPoint( shrt_abs(a.x), shrt_abs(a.y ) ); }
inline bool      const Neighbors( GridPoint const & a, GridPoint const & b )
{ 
    GridPoint gpDiff( Abs(a - b) );
    return ((gpDiff.x <= 1) || (gpDiff.x == GridPoint::GRID_WIDTH-1)) && ((gpDiff.y <= 1) || (gpDiff.y == GridPoint::GRID_HEIGHT-1));
}

 std::wostream & operator << ( std::wostream &, GridPoint const & );

class Grid;

class GridPoint_Functor
{
public:
             GridPoint_Functor( )                    : m_pGrid( nullptr ) { };
    explicit GridPoint_Functor( Grid * const pGrid ) : m_pGrid( pGrid )   { };

    virtual ~GridPoint_Functor() 
    { 
        m_pGrid = nullptr; 
    };

    virtual bool operator() ( GridPoint const & ) = 0; 

    void SetGrid( Grid * const pGrid ) 
    { 
        m_pGrid = pGrid; 
    }

    Grid * GetGrid( )
    { 
        assert( m_pGrid != nullptr );
        return m_pGrid; 
    }

private:
    Grid * m_pGrid;
};

class GridPointIntensity_Functor : public GridPoint_Functor
{
public:
    GridPointIntensity_Functor( );
    explicit GridPointIntensity_Functor( Grid * const );

    void  SetBrushIntensity( short const sInt ) { m_sIntensity = sInt; }
    short GetBrushIntensity( ) const { return m_sIntensity; }

    using GridPoint_Functor::operator(); // otherwise the operator() would be hidden by following operator()

    virtual bool operator() ( GridPoint const &, short const ) = 0;

private:
    short m_sIntensity;
};
