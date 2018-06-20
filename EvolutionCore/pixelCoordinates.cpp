// pixelCoordinates.cpp
//

#include "stdafx.h"
#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "pixelPoint.h"
#include "pixelCoordinates.h"

PixelCoordinates::PixelCoordinates
( 
    short const fs, 
	bool  const bHexagon
)
  : m_pixOffset ( ),
    m_sFieldSize( fs ),
    m_smoothMove(  ),
	m_bMoving   ( false ),
	m_bHexagon  ( bHexagon )
{ }

bool PixelCoordinates::isValidFieldSize( long const lNewFieldSize ) const 
{ 
    return (MINIMUM_FIELD_SIZE <= lNewFieldSize) && (lNewFieldSize <= MAXIMUM_FIELD_SIZE); 
};

void PixelCoordinates::MoveGrid( PixelPoint const & pntDelta )
{
    m_pixOffset -= pntDelta;
}

PixelPoint PixelCoordinates::getCenterOffset(GridRect const & gridRect, PixelPoint const pixCenter )  // Move gridRect to center of window
{
	assert(gridRect.GetStartPoint().IsInGrid());
	GridPoint  const gpStart(gridRect.GetStartPoint());
	GridPoint  const gpSum(gpStart + gpStart + gridRect.GetSize());
	PixelPoint const pixPnt(Grid2PixelSize(gpSum));
	PixelPoint const pixOffset(pixPnt / 2 - pixCenter);
	return pixOffset;
}

bool PixelCoordinates::CenterPoi( PixelPoint const pixCenter, GridPoint const gpPoi ) // returns TRUE, if POI was already centered, or if no POI defined
{
    if ( gpPoi.IsNull( ) )
        return true;

    PixelPoint pixCenterOffset = getCenterOffset( GridRect( gpPoi, gpPoi + 1 ), pixCenter );
    bool       bCentered       = ( m_pixOffset == pixCenterOffset );

    if ( ! bCentered )
        m_pixOffset = m_smoothMove.Step( m_pixOffset, pixCenterOffset );

    return bCentered;
}

bool PixelCoordinates::FitToRect( GridRect const & gridRect, PixelRectSize const pntPixSize )
{
    GridPoint gp( pntPixSize.GetWidth(), pntPixSize.GetHeight() );
    gp /= gridRect.GetSize() + 1;

    short const sNewFieldSize = std::min( gp.x, gp.y );
    
    if ( !isValidFieldSize( sNewFieldSize ) )
        return false;

    PixelPoint pixCenter( pntPixSize.GetWidth() / 2, pntPixSize.GetHeight() / 2 );
    m_sFieldSize = sNewFieldSize;
    m_pixOffset  = getCenterOffset( gridRect, pixCenter );

    return true;
}

bool PixelCoordinates::SetFieldSize( short const sNewFieldSize, PixelPoint const pntCenter )
{
    if ( !isValidFieldSize( sNewFieldSize ) )
        return false;
 
    m_pixOffset  = ((m_pixOffset + pntCenter) * sNewFieldSize) / m_sFieldSize - pntCenter;
    m_sFieldSize = sNewFieldSize;

    return true;
}

bool PixelCoordinates::Zoom( bool const bZoomIn, PixelPoint const pntCenter )
{
    short sNewFieldSize = m_sFieldSize;
    if ( bZoomIn )
    {
        short const sDelta = ( m_sFieldSize < 16 ) ? 1 : (( m_sFieldSize < 64 ) ? 16 : 32);
        sNewFieldSize += sDelta;
    }
    else
    {
        short const sDelta = ( m_sFieldSize <= 16 ) ? 1 : (( m_sFieldSize <= 64 ) ? 16 : 32);
        sNewFieldSize -= sDelta;
    }

    return SetFieldSize( sNewFieldSize, pntCenter );
}

PixelPoint PixelCoordinates::Pixel2PixelSize( PixelPoint const & ptSizeIn, PixelCoordinates const & fTarget ) const 
{
    static long const FACTOR = 1024; // to avoid zero when dividing small ptSizeIn by m_sFieldSize 

    return (((ptSizeIn * FACTOR) / m_sFieldSize) * fTarget.m_sFieldSize ) / FACTOR;
}

PixelPoint PixelCoordinates::Pixel2PixelPos( PixelPoint const & ptPosIn, PixelCoordinates const & fTarget ) const 
{
    return Pixel2PixelSize( ptPosIn + m_pixOffset, fTarget ) - fTarget.m_pixOffset;
}

GridCircle PixelCoordinates::Pixel2GridCircle( PixelPoint const & pntCenter, short const sRadius ) const 
{ 
	return GridCircle( Pixel2GridPos( pntCenter ), sRadius / m_sFieldSize ); 
}

KGridPoint PixelCoordinates::Pixel2KGridPos( PixelPoint const & pp ) const 
{ 
	return Pixel2KGridSize( pp + m_pixOffset, m_sFieldSize ); 
}

PixelPoint PixelCoordinates::KGrid2PixelPos( KGridPoint const & kp ) const 
{ 
	return KGrid2PixelSize( kp, m_sFieldSize ) - m_pixOffset; 
}
    
PixelPoint PixelCoordinates::Grid2PixelPosCenter( GridPoint  const & gp ) const 
{ 
	return Grid2PixelPos( gp ) + m_sFieldSize / 2; 
}

GridPoint PixelCoordinates::Pixel2GridSize( PixelPoint const & pp ) const 
{ 
	if ( m_bHexagon )
	{
		double const fDistX = (2 / sqrt( 3 )) * (static_cast<double>( pp.x ) - 0.5) / static_cast<double>( m_sFieldSize );
		long   const lDistX = static_cast<long>( fDistX );

		return GridPoint( lDistX, pp.y / m_sFieldSize ); 
	}
	else
		return GridPoint( pp.x / m_sFieldSize, pp.y / m_sFieldSize ); 
}

PixelPoint PixelCoordinates::Grid2PixelSize( GridPoint  const & gp ) const 
{ 
	if ( m_bHexagon )
	{
		double const fDistX = static_cast<double>( m_sFieldSize ) * (sqrt(3) / 2) * static_cast<double>( gp.x );
		long   const lDistX = static_cast<long>( fDistX + 0.5 );

		return PixelPoint( lDistX, gp.y * m_sFieldSize ); 
	}
	else
		return PixelPoint( gp.x * m_sFieldSize, gp.y * m_sFieldSize );
}

GridPoint PixelCoordinates::Pixel2GridPos ( PixelPoint const & pp ) const 
{ 
	PixelPoint const pixPoint = pp +  m_pixOffset;
	if ( m_bHexagon ) // https://web.archive.org/web/20161024224848/http://gdreflections.com/2011/02/hexagonal-grid-math.html
	{
		double dFieldSize = static_cast<double>(m_sFieldSize);
		double dRadius    = (sqrt(3) / 3) * dFieldSize;
		double dSide      = (sqrt(3) / 2) * dFieldSize;
		double dPixPointx = static_cast<double>(pixPoint.x);

		int    ci = static_cast<int>(floor(dPixPointx/dSide));
		double cx = dPixPointx - dSide * ci;

		double  ty = static_cast<double>(pixPoint.y - (ci % 2) * m_sFieldSize / 2);
		int     cj = static_cast<int>(floor(ty/dFieldSize));
		double  cy = ty - static_cast<double>(m_sFieldSize * cj);

		GridPoint gpResult( ci, cj);
		if (cx <= abs(dRadius / 2 - dRadius * cy / dFieldSize)) 
		{
			gpResult.x -= 1;
			gpResult.y += (ci % 2);
			if (cy < dFieldSize / 2)
				--gpResult.y;
		}
		return gpResult;
	}
	else 
	{
		return Pixel2GridSize( pixPoint ); 
	}
}

PixelPoint PixelCoordinates::Grid2PixelPos ( GridPoint const & gp ) const 
{ 
	PixelPoint ppRes = Grid2PixelSize( gp ) - m_pixOffset;
	if ( m_bHexagon )
	{
		if ( gp.IsOddCol( ) )
			ppRes.y -= m_sFieldSize / 2 ;
	}
	return ppRes;
}

KGridRect PixelCoordinates::Pixel2KGridRect( PixelRect const & rect ) const 
{
    PixelPoint pntStart( rect.left,  rect.top    );
    PixelPoint pntEnd  ( rect.right, rect.bottom );

    return KGridRect 
    (
        Pixel2KGridPos ( pntStart ),
        Pixel2KGridSize( pntEnd - pntStart, m_sFieldSize )
    );
}

PixelRect PixelCoordinates::KGrid2PixelRect( KGridRect const & kgrIn ) const 
{
    PixelPoint const ptPos  = KGrid2PixelPos ( kgrIn.GetPos()  );
    PixelPoint const ptSize = KGrid2PixelSize( kgrIn.GetSiz(), m_sFieldSize );
    return PixelRect( ptPos, ptPos + ptSize );
}

GridRect PixelCoordinates::Pixel2GridRect( PixelRect const & rect ) const 
{
    return GridRect
    ( 
        Pixel2GridPos( PixelPoint( rect.left,  rect.top    ) ), 
        Pixel2GridPos( PixelPoint( rect.right, rect.bottom ) ) 
    );
}

PixelRect PixelCoordinates::Grid2PixelRect( GridRect const & rcGrid ) const 
{
    return PixelRect
    ( 
        Grid2PixelPos( rcGrid.GetStartPoint( ) ),
        Grid2PixelPos( rcGrid.GetEndPoint  ( ) ) + (m_sFieldSize - 1) 
    );
}
