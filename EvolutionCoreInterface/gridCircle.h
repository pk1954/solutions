// gridCircle.h : 
//

#pragma once

#include <iostream>
#include "EvolutionTypes.h"
#include "gridPoint.h"

class GridCircle
{
public:
	GridCircle( ) 
	{ 
		Reset(); 
	}

    GridCircle
	( 
		GridPoint  const & gpCenter, 
		GRID_COORD const   radius 
	) :
		m_gpCenter( gpCenter ),
		m_radius( radius )
	{};

	virtual void Apply2Circle( GridPointFuncShort const &, short const = 0) const;

    bool const operator== ( GridCircle const &a ) const { return ( a.m_gpCenter == m_gpCenter ) && ( a.m_radius == m_radius ); };
    bool const operator!= ( GridCircle const &a ) const { return ( a.m_gpCenter != m_gpCenter ) || ( a.m_radius != m_radius ); };

	virtual void Reset( ) { *this = GRID_CIRCLE_EMPTY; }

    GridPoint  const GetCenter( ) const { return m_gpCenter; }
    GRID_COORD const GetRadius( ) const { return m_radius; }

    static GridCircle const GRID_CIRCLE_EMPTY;

private:
    GridPoint  m_gpCenter;
    GRID_COORD m_radius;
};

std::wostream & operator << ( std::wostream &, GridCircle const & );
