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

void GridPointShape::RefreshLayout( EvolutionCore const * const pCore )
{
	static const PIXEL MARGIN = 3_PIXEL;

	PixelRectSize const minCoord     = m_coordShape.MinimalSize( pCore );
	PixelRectSize const minIndiv     = m_indivShape.MinimalSize( pCore );
	PixelRectSize const minSize      = m_shape.SetMinSize( minIndiv );     
	PIXEL         const pixFieldSize = m_shape.GetFieldSize();

	if ( m_shape.SetShapeRect( PixelPoint( 0_PIXEL ), PixelRectSize( pixFieldSize ) ) )
	{
		PIXEL pixSizeInd   = GetIndShapeSize( ) * 2;
		PIXEL pixSizeFrame = pixFieldSize - pixSizeInd;
		PIXEL pixHalfFrame = pixSizeFrame / 2;

		m_coordShape.PrepareShape
		( 
			PixelPoint   ( pixHalfFrame, 0_PIXEL ), 
			PixelRectSize( pixSizeInd,   pixHalfFrame ) 
		);
		m_indivShape.PrepareShape
		( 
			PixelPoint   ( pixSizeFrame ) / 2 + MARGIN, 
			PixelRectSize( pixSizeInd   ) - MARGIN * 2
		);
	}
}

void GridPointShape::Draw
( 
	EvolutionCore const * const pCore, 
	GridPoint             const gp
)
{
	PixelPoint const pntGridpointOffset = m_textDisplay.GetOffset( gp );
	if ( m_shape.IsNotEmpty () )
	{
		m_coordShape.Draw( pCore, gp, pntGridpointOffset );
		m_indivShape.Draw( pCore, gp, pntGridpointOffset );
	}
}

Shape const * GridPointShape::FindShape
( 
	EvolutionCore const * const pCore, 
	GridPoint             const gp,
	PixelPoint            const pnt // client window coordinates
) const
{
	PixelPoint pntShapeRelative = pnt - m_textDisplay.GetOffset( gp );

 	Shape const * pShapeRes = m_coordShape.FindShape( pntShapeRelative, gp );
	if ( pShapeRes != nullptr )
		return pShapeRes;

	pShapeRes = m_indivShape.FindShape( pCore, pntShapeRelative, gp );
	if ( pShapeRes != nullptr )
		return pShapeRes;

	return m_shape.FindShape( pntShapeRelative, gp );
}
