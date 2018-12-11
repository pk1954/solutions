// win32_gridPointShape.cpp
//

#include "stdafx.h"
#include "config.h"
#include "win32_gridPointShape.h"

void GridPointShape::PrepareShape( GridPoint const gp )
{
    short const sFieldSize = m_textDisplay.GetFieldSize();
	if ( sFieldSize >= ZOOM_LEVEL_1 )
	{
		PixelRectSize const rectSize = GetShapeSize();
		long                lYpos    = 0;
		long                lHeight  = rectSize.GetHeight() / 10;

		m_coordShape.SetShapeRect
		( 
			PixelPoint   (                   0,   lYpos ),
			PixelRectSize( rectSize.GetWidth(), lHeight )
		);
		m_coordShape.PrepareShape( gp );
		lYpos += lHeight;

		m_indivShape.SetShapeRect
		( 
			rectSize.ToPixelPoint() *  3 / 16,    // 3 left + 3 right margin
			rectSize                * 10 / 16     // rest for iIdividualShape
		);
		m_indivShape.PrepareShape( gp );
	}
}

void GridPointShape::Draw( GridPoint const gp )
{
    short const sFieldSize = m_textDisplay.GetFieldSize();
	if ( sFieldSize >= ZOOM_LEVEL_1 )
	{
		if ( Config::GetConfigValueBoolOp( Config::tId::showGridPointCoords ) == tBoolOp::opTrue )
			m_coordShape.Draw( gp );
		m_indivShape.Draw( gp );
	}
}

Shape const * GridPointShape::FindShape
( 
	PixelPoint const pnt, 
	GridPoint  const gp
) const
{
	Shape const * pShapeRes;
    short const   sFieldSize = m_textDisplay.GetFieldSize();
	if ( sFieldSize >= ZOOM_LEVEL_1 )
	{
		pShapeRes = m_coordShape.FindShape( pnt, gp );
		if ( pShapeRes != nullptr )
			return pShapeRes;

		pShapeRes = m_indivShape.FindShape( pnt, gp );
		if ( pShapeRes != nullptr )
			return pShapeRes;
	}

	return PointInShape( pnt ) ? this : nullptr;
}
