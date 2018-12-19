// win32_gridPointShape.cpp
//

#include "stdafx.h"
#include "config.h"
#include "win32_gridPointShape.h"

// Can be displayed, if at least IndividualShape has space
// If possible, display also CoordShape

long GridPointShape::GetIndShapeSize( ) // returns half of side length
{
	short const sFieldSize = m_shape.GetFieldSize();
	return ( sFieldSize <    8 ) ?                         1  :
		   ( sFieldSize <=  16 ) ? ((3 * sFieldSize) / 8 - 1) : 
								   ((3 * sFieldSize) / 8    );
}

void GridPointShape::RefreshLayout( )
{
	PixelRectSize const minCoord   = m_coordShape.MinimalSize( );
	PixelRectSize const minIndiv   = m_indivShape.MinimalSize( );
	PixelRectSize const minSize    = m_shape.SetMinSize( minIndiv );     
	short         const sFieldSize = m_shape.GetFieldSize();

	if ( m_shape.SetShapeRect( PixelPoint(), PixelRectSize( sFieldSize ) ) )
	{
		long lSizeInd   = 2 * GetIndShapeSize( );
		long lSizeFrame = sFieldSize - lSizeInd;

		m_coordShape.PrepareShape( PixelPoint( lSizeFrame / 2,              0 ), PixelRectSize( lSizeInd, lSizeFrame / 2 ) );
		m_indivShape.PrepareShape
		( 
			PixelPoint( lSizeFrame / 2 + 3, lSizeFrame / 2 + 3 ), 
			PixelRectSize( lSizeInd - 6,       lSizeInd - 6 ) 
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
