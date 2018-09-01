// gridRect.h : 
//

#pragma once

#include <iostream>
#include <functional>
#include "gridShape.h"

class GridRect;

void Apply2Grid( GridPointFunc const & );

class GridRect : public Shape
{
public:
	GridRect( ) 
	{ 
		Reset(); 
	}

	GridRect
	( 
		GRID_COORD const lLeft, 
		GRID_COORD const lTop, 
		GRID_COORD const lRight, 
		GRID_COORD const lBottom 
	) : 
		m_lLeft  (lLeft), 
		m_lTop   (lTop), 
		m_lRight (lRight), 
		m_lBottom(lBottom) 
	{};

    GridRect
	( 
		GridPoint const & gpStart, 
		GridPoint const & gpEnd 
	) : 
		m_lLeft  (gpStart.x), 
		m_lTop   (gpStart.y), 
		m_lRight (gpEnd.x), 
		m_lBottom(gpEnd.y) 
	{};

    GridRect
	( 
		GridPoint const & gpCenter, 
		GRID_COORD const iSize 
	) : 
		m_lLeft  (gpCenter.x - iSize), 
		m_lTop   (gpCenter.y - iSize), 
		m_lRight (gpCenter.x + iSize), 
		m_lBottom(gpCenter.y + iSize) 
	{};

    ~GridRect() { };

	virtual void Reset( ) { *this = GRID_RECT_EMPTY; }

	virtual void Apply2Shape( GridPointFuncShort const &, short const = 0 ) const;
	
	void Apply2Rect( GridPointFunc const & ) const;

    bool const operator== ( GridRect const &a ) const { return ( a.m_lLeft == m_lLeft ) && ( a.m_lTop == m_lTop ) && ( a.m_lRight == m_lRight ) && ( a.m_lBottom == m_lBottom ); };
    bool const operator!= ( GridRect const &a ) const { return ( a.m_lLeft != m_lLeft ) || ( a.m_lTop != m_lTop ) || ( a.m_lRight != m_lRight ) || ( a.m_lBottom != m_lBottom ); };

	GRID_COORD const GetLeft  () const { return m_lLeft;   };
    GRID_COORD const GetTop   () const { return m_lTop;    };
    GRID_COORD const GetRight () const { return m_lRight;  };
    GRID_COORD const GetBottom() const { return m_lBottom; };

    virtual GridPoint const GetStartPoint( ) const { return GridPoint( GetLeft(),  GetTop()    ); }
    virtual GridPoint const GetEndPoint  ( ) const { return GridPoint( GetRight(), GetBottom() ); }
    virtual GridPoint const GetCenter    ( ) const { return GridPoint( (m_lLeft + m_lRight) / 2, (m_lTop + m_lBottom) / 2 ); }
    virtual GridPoint const GetSize      ( ) const { return GridPoint( m_lRight - m_lLeft, m_lBottom - m_lTop ); }
	
	GridRect  const ClipToGrid   ( ) const { return GridRect( clipStartPoint( ), clipEndPoint( ) );	}
 
    virtual bool const IsEmpty( )    const { return ( *this == GRID_RECT_EMPTY ); }
    virtual bool const IsNotEmpty( ) const { return ( *this != GRID_RECT_EMPTY ); }

    static GridRect const GRID_RECT_FULL;
    static GridRect const GRID_RECT_EMPTY;

private:
	GridPoint clipStartPoint( ) const;
	GridPoint clipEndPoint  ( ) const;

	GRID_COORD m_lLeft;
    GRID_COORD m_lTop;
    GRID_COORD m_lRight;
    GRID_COORD m_lBottom;
};

std::wostream & operator << ( std::wostream &, GridRect const & );
