// win32_individualShape.h
//

#pragma once

#include <sstream> 
#include "GridPoint.h"
#include "win32_gridPointShape.h"
#include "win32_leftColumn.h"
#include "win32_rightColumn.h"

class PixelCoordinates;
class EvolutionCore;
class D3dBuffer;
class LeftColumn;
class RightColumn;

class IndividualShape : public GridPointShape
{
public:
	IndividualShape( TextDisplay & textDisplay ) :
		GridPointShape( nullptr, textDisplay ),
		m_leftColumn  ( this, textDisplay ),
		m_rightColumn ( this, textDisplay ),
		m_gpCoordShape( this, textDisplay )
	{ }

	virtual void FillBuffer( GridPoint const gp ) { };  // all text in subshapes

	virtual void PrepareShape( GridPoint const gp )
	{
    	short const sFieldSize = m_textDisplay.GetFieldSize();
		if ( sFieldSize >= ZOOM_LEVEL_1 )
		{
			PixelRectSize const rectSize = GetSize();
			long          const lWidth   = rectSize.GetWidth();
			long          const lHeight  = rectSize.GetHeight();

			m_gpCoordShape.SetSize( PixelRectSize( lWidth, lHeight / 10 ) );
			m_gpCoordShape.SetShapeOffset( PixelPoint( 0, 11 * lHeight / 10 ) );
			m_gpCoordShape.PrepareShape( gp );

			m_leftColumn.SetSize( PixelRectSize( lWidth / 2, lHeight ) );
			m_leftColumn.SetShapeOffset( PixelPoint( 0, 0 ) );
			m_leftColumn.PrepareShape( gp );

			if ( sFieldSize >= ZOOM_LEVEL_2 )
			{
				m_rightColumn.SetSize( PixelRectSize( lWidth / 2, lHeight ) );
				m_rightColumn.SetShapeOffset( PixelPoint( lWidth / 2, 0 ) );
				m_rightColumn.PrepareShape( gp );
			}
		}
	}

	void Draw( GridPoint const gp )
	{
    	short const sFieldSize = m_textDisplay.GetFieldSize();
		if ( sFieldSize >= ZOOM_LEVEL_1 )
		{
			m_gpCoordShape.Draw( gp );
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

	GridPointShape const * FindShape
	( 
		PixelPoint const pnt, 
		GridPoint  const gp
	) const
	{
    	short const sFieldSize = m_textDisplay.GetFieldSize();
		if ( sFieldSize >= ZOOM_LEVEL_1 )
		{
			GridPointShape const * pShapeRes = m_leftColumn.FindShape( pnt, gp );
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

private:
	static short const ZOOM_LEVEL_1 =  96;
	static short const ZOOM_LEVEL_2 = 256;

	GridPointCoordShape m_gpCoordShape;
	LeftColumn          m_leftColumn;
	RightColumn         m_rightColumn;
};
