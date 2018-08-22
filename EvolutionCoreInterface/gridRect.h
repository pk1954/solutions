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
	GridRect() : Shape() {}

    GridRect( GridPoint const & gpStart, GridPoint const & gpEnd )
	{
		m_gpCenter    = (gpEnd + gpStart) / 2;
		m_gpExtension = gpEnd - gpStart;

		if ( (m_gpExtension.x < 0) || (m_gpExtension.y < 0) )
			Reset();
	}
    
	GridRect( GridPoint const & gpCenter, GRID_COORD const iSize ) 
		: Shape( gpCenter, iSize )
	{ }

	virtual void Apply2Shape( GridPointFuncShort const &, short const = 0 ) const;
	
	void Apply2Rect( GridPointFunc const & ) const;

    GRID_COORD const GetLeft     ( ) const { return m_gpCenter.x - m_gpExtension.x / 2; };
    GRID_COORD const GetTop      ( ) const { return m_gpCenter.y - m_gpExtension.y / 2; };
    GRID_COORD const GetRight    ( ) const { return m_gpCenter.x + (m_gpExtension.x + 1) / 2 - 1; };
    GRID_COORD const GetBottom   ( ) const { return m_gpCenter.y + (m_gpExtension.y + 1) / 2 - 1; };

    GridPoint const GetStartPoint( ) const { return GridPoint( GetLeft(),  GetTop()    ); }
    GridPoint const GetEndPoint  ( ) const { return GridPoint( GetRight(), GetBottom() ); }

	GridRect  const ClipToGrid   ( ) const { return GridRect( clipStartPoint( ), clipEndPoint( ) );	}
 
    static GridRect const GRID_RECT_FULL;

private:
	GridPoint clipStartPoint( ) const;
	GridPoint clipEndPoint  ( ) const;
};

std::wostream & operator << ( std::wostream &, GridRect const & );
