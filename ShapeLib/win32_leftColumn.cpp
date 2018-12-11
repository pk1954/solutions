// win32_leftColumn.cpp
//

#include "stdafx.h"
#include <iomanip>     
#include "win32_leftColumn.h"

void LeftColumn::PrepareShape( GridPoint const gp )
{
    short const sFieldSize = m_textDisplay.GetFieldSize();
	if ( sFieldSize >= ZOOM_LEVEL_1 )
	{
		PixelRectSize const rectSize     = GetShapeSize();
		long          const lShapeWidth  = rectSize.GetWidth();
		long          const lShapeHeight = rectSize.GetHeight();
		long                lYpos        = 0;
		long                lHeight      = lShapeHeight / 10;

		m_idShape.SetShapeRect
		( 
			PixelPoint   (           0, lYpos ),
			PixelRectSize( lShapeWidth, lHeight )
		);
		m_idShape.PrepareShape( gp );
		lYpos += lHeight;

		m_infoShape.SetShapeRect
		( 
			PixelPoint   (           0, lYpos ),
			PixelRectSize( lShapeWidth, lShapeHeight - lYpos)
		);
		m_infoShape.PrepareShape( gp );
	}
}

void LeftColumn::Draw( GridPoint const gp )
{
    short const sFieldSize = m_textDisplay.GetFieldSize();
	if ( sFieldSize >= ZOOM_LEVEL_1 )
	{
		m_idShape.Draw( gp );
		m_infoShape.Draw( gp );
	}
}

Shape const * LeftColumn::FindShape
( 
	PixelPoint const pnt, 
	GridPoint  const gp
) const
{
	Shape const * pShapeRes;
    short const   sFieldSize = m_textDisplay.GetFieldSize();
	if ( sFieldSize >= ZOOM_LEVEL_1 )
	{
		pShapeRes = m_idShape.FindShape( pnt, gp );
		if ( pShapeRes != nullptr )
			return pShapeRes;

		pShapeRes = m_infoShape.FindShape( pnt, gp );
		if ( pShapeRes != nullptr )
			return pShapeRes;
	}

	return PointInShape( pnt ) ? this : nullptr;
}
