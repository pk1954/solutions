// win32_gridPointShape.cpp
//

#pragma once

#include <sstream> 
#include "d3d_buffer.h"
#include "GridPoint.h"
#include "EvolutionCore.h"
#include "win32_textDisplay.h"
#include "win32_rectShape.h"

class GridPointShape : public RectShape
{
public:
	GridPointShape
	( 
		Shape * const pParent,
		TextDisplay & textDisplay 
	) :
		RectShape( pParent ),
		m_textDisplay( textDisplay )
	{ }

	GridPointShape const * FindShape
	( 
		PixelPoint const pnt,             
		GridPoint  const gp
	) const
	{
		return PointInShape( pnt ) ? this : nullptr;
	}

	virtual GridPoint GetReferencedGridPoint( GridPoint const gp ) const 
	{ 
		return GridPoint::GP_NULL; 
	}

	void Draw( GridPoint const gp )
	{
		m_textDisplay.Clear();
		FillBuffer( gp );
		m_textDisplay.DrawText( GetRect( ) );
	}

	virtual void PrepareShape( GridPoint const ) {};

protected:

	virtual void FillBuffer( GridPoint const ) = 0;

    TextDisplay & m_textDisplay;
};   

