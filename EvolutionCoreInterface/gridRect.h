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

    explicit GridRect
	( 
		GridPoint const gpStart, 
		GridPoint const gpEnd 
	) : 
		m_Left  (gpStart.GetX()), 
		m_Top   (gpStart.GetY()), 
		m_Right (gpEnd.GetX()), 
		m_Bottom(gpEnd.GetY())
	{
		assert( m_Top <= m_Bottom );
	};

	~GridRect() { };

	void Reset( ) { * this = GRID_RECT_EMPTY(); }

	void Apply2Rect( GridPointFunc const & func ) const
	{
		::Apply2Rect( func, clipStartPoint( ), clipEndPoint( ) );
	}

    bool const operator== ( GridRect const &a ) const { return ( a.m_Left == m_Left ) && ( a.m_Top == m_Top ) && ( a.m_Right == m_Right ) && ( a.m_Bottom == m_Bottom ); };
    bool const operator!= ( GridRect const &a ) const { return ( a.m_Left != m_Left ) || ( a.m_Top != m_Top ) || ( a.m_Right != m_Right ) || ( a.m_Bottom != m_Bottom ); };

    GRID_COORD const GetWidth ( ) const { return m_Right - m_Left + GRID_COORD(1_GRID_COORD); }
	GRID_COORD const GetHeight( ) const { return m_Bottom - m_Top + GRID_COORD(1_GRID_COORD); }
	
	GRID_COORD const GetLeft  () const { return m_Left;   };
    GRID_COORD const GetTop   () const { return m_Top;    };
    GRID_COORD const GetRight () const { return m_Right;  };
    GRID_COORD const GetBottom() const { return m_Bottom; };

    GridPoint const GetStartPoint( ) const { return GridPoint( GetLeft(),  GetTop()    ); }
    GridPoint const GetEndPoint  ( ) const { return GridPoint( GetRight(), GetBottom() ); }
    GridPoint const GetCenter    ( ) const { return ( GetStartPoint() + GetEndPoint() ) / 2; }
    GridPoint const GetSize      ( ) const { return GridPoint( GetWidth(), GetHeight() ); }
	
	GridRect  const ClipToGrid   ( ) const { return GridRect( clipStartPoint( ), clipEndPoint( ) );	}
 
    bool const IsEmpty( )    const { return ( *this == GRID_RECT_EMPTY() ); }
    bool const IsNotEmpty( ) const { return ( *this != GRID_RECT_EMPTY() ); }

	bool Includes( GridPoint const gp ) const
	{
		return (m_Left <= gp.GetX()) && (gp.GetX() <= m_Right) && (m_Top <= gp.GetY()) && (gp.GetY() <= m_Bottom);
	}

	inline static GridRect const & GRID_RECT_EMPTY() 
	{ 
		static GridRect res = GridRect( GRID_ORIGIN(), GRID_ORIGIN() ); 
		return res;
	};

	inline static GridRect const & GRID_RECT_FULL() 
	{ 
		static GridRect res = GridRect( GRID_ORIGIN(), GRID_SIZE() - GRID_COORD(1_GRID_COORD)); 
		return res;
	};

private:
	GridPoint clipStartPoint( ) const;
	GridPoint clipEndPoint  ( ) const;

	GRID_COORD m_Left;
    GRID_COORD m_Top;
    GRID_COORD m_Right;
    GRID_COORD m_Bottom;
};

std::wostream & operator << ( std::wostream &, GridRect const & );
