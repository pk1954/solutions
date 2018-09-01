// gridCircle.h : 
//

#pragma once

#include <iostream>
#include "EvolutionTypes.h"
#include "gridShape.h"
#include "gridPoint.h"

class GridCircle : public Shape
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

	virtual void Apply2Shape( GridPointFuncShort const &, short const = 0) const;

    bool const operator== ( GridCircle const &a ) const { return ( a.m_gpCenter == m_gpCenter ) && ( a.m_radius == m_radius ); };
    bool const operator!= ( GridCircle const &a ) const { return ( a.m_gpCenter != m_gpCenter ) || ( a.m_radius != m_radius ); };

	virtual void Reset( ) { *this = GRID_CIRCLE_EMPTY; }

    virtual GridPoint  const GetCenter( ) const { return m_gpCenter; }
    virtual GRID_COORD const GetRadius( ) const { return m_radius; }

	virtual bool const IsEmpty( )    const { return ( *this == GRID_CIRCLE_EMPTY ); }
    virtual bool const IsNotEmpty( ) const { return ( *this != GRID_CIRCLE_EMPTY ); }

    static GridCircle const GRID_CIRCLE_EMPTY;

private:
    GridPoint  m_gpCenter;
    GRID_COORD m_radius;
};

std::wostream & operator << ( std::wostream &, GridCircle const & );
