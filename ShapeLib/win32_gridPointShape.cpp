// win32_gridPointShape.cpp
//

#include "stdafx.h"
#include "config.h"
#include "win32_gridPointShape.h"

// Can be displayed, if at least IndividualShape has space
// If possible, display also CoordShape

PixelRectSize GridPointShape::MinimalSize( )  
{
	PixelRectSize minCoord = m_coordShape.MinimalSize( );
	PixelRectSize minIndiv = m_indivShape.MinimalSize( );

	return setMinSize( minIndiv );     
}                                     

long GridPointShape::GetIndShapeSize( ) // returns half of side length
{
	short const sFieldSize = m_textDisplay.GetFieldSize();
	return ( sFieldSize <    8 ) ?                         1  :
		   ( sFieldSize <=  16 ) ? ((3 * sFieldSize) / 8 - 1) : 
								   ((3 * sFieldSize) / 8    );
}

void GridPointShape::RefreshLayout( )
{
	MinimalSize( );

	short const sFieldSize = m_textDisplay.GetFieldSize();
	if ( setShapeRect( PixelPoint(), PixelRectSize( sFieldSize ) ) )
	{
		long lSizeInd   = 2 * GetIndShapeSize( );
		long lSizeFrame = sFieldSize - lSizeInd;

		PixelPoint pixPosSubShape = getShapePos( );

		pixPosSubShape += PixelPoint( lSizeFrame / 2, 0 );
		m_coordShape.PrepareShape( pixPosSubShape, PixelRectSize( lSizeInd, lSizeFrame / 2 ) );
		pixPosSubShape += PixelPoint( 0, lSizeFrame / 2);
		m_indivShape.PrepareShape( pixPosSubShape, PixelRectSize( lSizeInd, lSizeInd ) );
	}
}

void GridPointShape::Draw( GridPoint const gp, PixelPoint const ppGridpointOffset )
{
	if ( isNotEmpty () )
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

	return Shape::FindShape( pnt, gp );
}
