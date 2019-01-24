// gridRect.h : 
//

#pragma once

#include <iostream>
#include "gridPoint.h"

class GridRect;

void Apply2Rect( GridPointFunc const &, GridPoint const, GridPoint const, bool const = false );
void Apply2Grid( GridPointFunc const &,                                   bool const = false );

class GridRect
{
public:
	GridRect( ) 
	{ 
		Reset(); 
	}

    GridRect
	( 
		GridPoint const gpStart, 
		GridPoint const gpEnd 
	) : 
		m_lLeft  (gpStart.x), 
		m_lTop   (gpStart.y), 
		m_lRight (gpEnd.x), 
		m_lBottom(gpEnd.y)
	{
		assert( m_lTop <= m_lBottom );
	};

	~GridRect() { };

	void Reset( ) { * this = GRID_RECT_EMPTY(); }

	void Apply2Rect( GridPointFunc const & func ) const
	{
		::Apply2Rect( func, clipStartPoint( ), clipEndPoint( ) );
	}

    bool const operator== ( GridRect const &a ) const { return ( a.m_lLeft == m_lLeft ) && ( a.m_lTop == m_lTop ) && ( a.m_lRight == m_lRight ) && ( a.m_lBottom == m_lBottom ); };
    bool const operator!= ( GridRect const &a ) const { return ( a.m_lLeft != m_lLeft ) || ( a.m_lTop != m_lTop ) || ( a.m_lRight != m_lRight ) || ( a.m_lBottom != m_lBottom ); };

    GRID_COORD const GetWidth ( ) const { return m_lRight  - m_lLeft + GRID_COORD(1_GRID_COORD); }
	GRID_COORD const GetHeight( ) const { return m_lBottom - m_lTop  + GRID_COORD(1_GRID_COORD); }
	
	GRID_COORD const GetLeft  () const { return m_lLeft;   };
    GRID_COORD const GetTop   () const { return m_lTop;    };
    GRID_COORD const GetRight () const { return m_lRight;  };
    GRID_COORD const GetBottom() const { return m_lBottom; };

    GridPoint const GetStartPoint( ) const { return GridPoint( GetLeft(),  GetTop()    ); }
    GridPoint const GetEndPoint  ( ) const { return GridPoint( GetRight(), GetBottom() ); }
    GridPoint const GetCenter    ( ) const { return ( GetStartPoint() + GetEndPoint() ) / 2; }
    GridPoint const GetSize      ( ) const { return GridPoint( GetWidth(), GetHeight() ); }
	
	GridRect  const ClipToGrid   ( ) const { return GridRect( clipStartPoint( ), clipEndPoint( ) );	}
 
    bool const IsEmpty( )    const { return ( *this == GRID_RECT_EMPTY() ); }
    bool const IsNotEmpty( ) const { return ( *this != GRID_RECT_EMPTY() ); }

	bool Includes( GridPoint const gp ) const
	{
		return (m_lLeft <= gp.x) && (gp.x <= m_lRight) && (m_lTop <= gp.y) && (gp.y <= m_lBottom);
	}

	inline static GridRect const & GRID_RECT_EMPTY() 
	{ 
		static GridRect res = GridRect( GridPoint::GRID_ORIGIN(), GridPoint::GRID_ORIGIN() ); 
		return res;
	};

	inline static GridRect const & GRID_RECT_FULL() 
	{ 
		static GridRect res = GridRect( GridPoint::GRID_ORIGIN(), GridPoint::GRID_SIZE() - GRID_COORD(1_GRID_COORD)); 
		return res;
	};

private:
	GridPoint clipStartPoint( ) const;
	GridPoint clipEndPoint  ( ) const;

	GRID_COORD m_lLeft;
    GRID_COORD m_lTop;
    GRID_COORD m_lRight;
    GRID_COORD m_lBottom;
};

std::wostream & operator << ( std::wostream &, GridRect const & );
