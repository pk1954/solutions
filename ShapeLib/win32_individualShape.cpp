// win32_individualShape.cpp
//

#include "stdafx.h"
#include "win32_individualShape.h"

void IndividualShape::PrepareShape( GridPoint const gp )
{
    short const sFieldSize = m_textDisplay.GetFieldSize();
	if ( sFieldSize >= ZOOM_LEVEL_1 )
	{
		PixelRect rect    = GetShapeRect();
		long      lWidth  = rect.GetWidth();
		long      lHeight = rect.GetHeight();

		if ( sFieldSize >= ZOOM_LEVEL_2 )
		{
			m_leftColumn.SetShapeRect
			( 
				PixelPoint   (          0,       0 ),
				PixelRectSize( lWidth / 2, lHeight )
			);
			m_leftColumn.PrepareShape( gp );

			m_rightColumn.SetShapeRect
			( 
				PixelPoint   ( lWidth / 2,       0 ),
				PixelRectSize( lWidth / 2, lHeight )
			);
			m_rightColumn.PrepareShape( gp );
		}
		else
		{
			m_leftColumn.SetShapeRect
			( 
				PixelPoint   (      0,       0 ),
				PixelRectSize( lWidth, lHeight )
			);
			m_leftColumn.PrepareShape( gp );
		}
	}
}

void IndividualShape::Draw( GridPoint const gp )
{
    short const sFieldSize = m_textDisplay.GetFieldSize();
	if ( sFieldSize >= ZOOM_LEVEL_1 )
	{
		m_leftColumn.Draw( gp );

		if ( sFieldSize >= ZOOM_LEVEL_2 )
		{
			if ( m_textDisplay.GetStrategyId( gp ) == tStrategyId::tit4tat )
			{
				m_rightColumn.Draw( gp );
			}
		}
	}
}

Shape const * IndividualShape::FindShape
( 
	PixelPoint const pnt, 
	GridPoint  const gp
) const
{
    short const sFieldSize = m_textDisplay.GetFieldSize();
	if ( sFieldSize >= ZOOM_LEVEL_1 )
	{
		Shape const * pShapeRes = m_leftColumn.FindShape( pnt, gp );
		if ( pShapeRes != nullptr )
			return pShapeRes;

		if ( sFieldSize >= ZOOM_LEVEL_2 )
		{
			if ( m_textDisplay.GetStrategyId( gp ) == tStrategyId::tit4tat )
			{
				pShapeRes = m_rightColumn.FindShape( pnt, gp );
				if ( pShapeRes != nullptr )
					return pShapeRes;
			}
		}
	}

	return PointInShape( pnt ) ? this : nullptr;
}
