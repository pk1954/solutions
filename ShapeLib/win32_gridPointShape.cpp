// win32_gridPointShape.cpp
//

#include "stdafx.h"
#include "config.h"
#include "win32_gridPointShape.h"

// Can be displayed, if at least IndividualShape has space
// If possible, display also CoordShape

PIXEL GridPointShape::GetIndShapeSize( ) // returns half of side length
{
	PIXEL const pixFieldSize = m_shape.GetFieldSize();
	return ( pixFieldSize <   8_PIXEL ) ?                           1_PIXEL   :
		   ( pixFieldSize <= 16_PIXEL ) ? ((pixFieldSize * 3) / 8 - 1_PIXEL ) : 
								          ((pixFieldSize * 3) / 8    );
}

void GridPointShape::RefreshLayout( )
{
	static const PIXEL MARGIN = 3_PIXEL;

	PixelRectSize const minCoord     = m_coordShape.MinimalSize( );
	PixelRectSize const minIndiv     = m_indivShape.MinimalSize( );
	PixelRectSize const minSize      = m_shape.SetMinSize( minIndiv );     
	PIXEL         const pixFieldSize = m_shape.GetFieldSize();

	if ( m_shape.SetShapeRect( PixelPoint( 0_PIXEL ), PixelRectSize( pixFieldSize ) ) )
	{
		PIXEL pixSizeInd   = GetIndShapeSize( ) * 2;
		PIXEL pixSizeFrame = pixFieldSize - pixSizeInd;
		PIXEL pixHalfFrame = pixSizeFrame / 2;

		m_coordShape.PrepareShape
		( 
			PixelPoint   ( PIXEL_X(pixHalfFrame), PIXEL_Y(0_PIXEL) ), 
			PixelRectSize( PIXEL_X(pixSizeInd),   PIXEL_Y(pixHalfFrame) ) 
		);
		m_indivShape.PrepareShape
		( 
			PixelPoint   ( pixSizeFrame ) / 2 + MARGIN, 
			PixelRectSize( pixSizeInd   ) - MARGIN * 2
		);
	}
}

void GridPointShape::Draw( GridPoint const gp, PixelPoint const ppGridpointOffset )
{
	if ( m_shape.IsNotEmpty () )
	{
		m_coordShape.Draw( gp, ppGridpointOffset );
		m_indivShape.Draw( gp, ppGridpointOffset );
	}
}

Shape const * GridPointShape::FindShape
( 
	PixelPoint const pnt, 
	GridPoint  const gp
) const
{
 	Shape const * pShapeRes = m_coordShape.FindShape( pnt, gp );
	if ( pShapeRes != nullptr )
		return pShapeRes;

	pShapeRes = m_indivShape.FindShape( pnt, gp );
	if ( pShapeRes != nullptr )
		return pShapeRes;

	return m_shape.FindShape( pnt, gp );
}
