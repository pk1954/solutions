// gridShape.h
//

#pragma once

#include "gridPoint.h"

class Shape
{
public:
	Shape()	
	{ 
		Reset(); 
	}

	Shape( GridPoint const & gpCenter, GRID_COORD const iSize ) : 
		m_gpCenter(gpCenter), 
		m_gpExtension(GridPoint(iSize, iSize))
	{}

	virtual ~Shape() {};

	virtual void Apply2Shape( GridPointFuncShort const &, short const = 0 ) const = 0;

	void SetCenter( GridPoint  const & gp ) { m_gpCenter    = gp; };
	void SetSize  ( GridPoint  const & gp ) { m_gpExtension = gp; };
	void SetSize  ( GRID_COORD const    s ) { m_gpExtension = GridPoint( s, s ); };
	void Move     ( GridPoint  const & gp ) { m_gpCenter += gp; }

	void Reset( ) 
	{
		m_gpCenter    = GridPoint::GP_NULL;
		m_gpExtension = GridPoint::GP_NULL;
	}

    bool const operator== ( Shape const &a ) const { return ( a.m_gpCenter == m_gpCenter ) && ( a.m_gpExtension == m_gpExtension ); };
    bool const operator!= ( Shape const &a ) const { return ( a.m_gpCenter != m_gpCenter ) || ( a.m_gpExtension != m_gpExtension ); };

    bool const IsEmpty( )    const { return ( m_gpExtension == GridPoint::GP_NULL ); }
    bool const IsNotEmpty( ) const { return ( m_gpExtension != GridPoint::GP_NULL ); }

    GridPoint const GetSize  ( ) const { return m_gpExtension; }
	GridPoint const GetCenter( ) const { return m_gpCenter; }

protected:

	GridPoint m_gpCenter;
	GridPoint m_gpExtension;    
};
