// pixelCoordinates.cpp
//

#include "stdafx.h"
#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "pixelPoint.h"
#include "pixelCoordinates.h"

static double const SQRT3_DIV2 = sqrt(3.) / 2.;
static double const SQRT3_DIV3 = sqrt(3.) / 3.;

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

bool PixelCoordinates::FitGridToRect( GridRect const & gridRect, PixelRectSize const pntPixSize )
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

bool PixelCoordinates::SetGridFieldSize( short const sNewFieldSize, PixelPoint const pntCenter )
{
    if ( !isValidFieldSize( sNewFieldSize ) )
        return false;
 
    m_pixOffset  = ((m_pixOffset + pntCenter) * sNewFieldSize) / m_sFieldSize - pntCenter;
    m_sFieldSize = sNewFieldSize;

    return true;
}

short PixelCoordinates::ComputeNewFieldSize( bool const bZoomIn ) const
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
	return sNewFieldSize;
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

KGridPoint PixelCoordinates::Pixel2KGridPos( PixelPoint const & pp ) const 
{ 
	return Pixel2KGridSize( pp + m_pixOffset, m_sFieldSize ); 
}

PixelPoint PixelCoordinates::KGrid2PixelPos( KGridPoint const & kp ) const 
{ 
	return KGrid2PixelSize( kp, m_sFieldSize ) - m_pixOffset; 
}
    
PixelPoint PixelCoordinates::Grid2PixelSize( GridPoint  const & gp ) const 
{ 
	PixelPoint ppRes( gp.x * m_sFieldSize, gp.y * m_sFieldSize );
	
	if ( m_bHexagon )
		ppRes.x = static_cast<long>( ppRes.x * SQRT3_DIV2 + 0.5 );

	return ppRes;
}

PixelPoint PixelCoordinates::Grid2PixelPos ( GridPoint const & gp ) const 
{ 
	PixelPoint ppRes = Grid2PixelSize( gp ) - m_pixOffset;

	if ( m_bHexagon && gp.IsOddCol( ) )
		ppRes.y -= m_sFieldSize / 2 ;

	return ppRes;
}

PixelPoint PixelCoordinates::Grid2PixelPosCenter( GridPoint  const & gp ) const 
{ 
	if (m_bHexagon)
	{
		PixelPoint pxResult = Grid2PixelPos( gp );
		pxResult.x += static_cast<long>(SQRT3_DIV3 * m_sFieldSize);
		pxResult.y += m_sFieldSize / 2;
		return pxResult;
	}
	else
		return Grid2PixelPos( gp ) + m_sFieldSize / 2; 
}

GridPoint PixelCoordinates::Pixel2GridPos( PixelPoint const & pp ) const 
{ 
	PixelPoint pixPoint = pp + m_pixOffset;

	if ( m_bHexagon ) // adapted from http://blog.ruslans.com/2011/02/hexagonal-grid-math.html
	{
		double const dFieldSize = static_cast<double>(m_sFieldSize);
		double const dRadius    = dFieldSize * SQRT3_DIV3;
		double const dSide      = dFieldSize * SQRT3_DIV2;

		double const dPixPointX = static_cast<double>(pixPoint.x);
		double const dCi        = floor(dPixPointX/dSide);
		double const dCx        = dPixPointX - dSide * dCi;

		GRID_COORD const gCi    = static_cast<GRID_COORD>(dCi);
		bool       const bOdd   = ((gCi % 2) != 0);

		double const dPixPointY = static_cast<double>(pixPoint.y);
		double const dTy        = dPixPointY + (bOdd ? (dFieldSize * 0.5) : 0);
		double const dCj        = floor(dTy/dFieldSize);
		double const dCy        = dTy - dFieldSize * dCj;
		double const dCrit      = 0.5 - dCy / dFieldSize;

		GRID_COORD const gCj    = static_cast<GRID_COORD>(dCj);

		GridPoint gpResult;

		if (dCx > dRadius * abs(dCrit))
		{
			gpResult.x = gCi;
			gpResult.y = gCj;
		}
		else
		{
			gpResult.x = gCi - 1;
			gpResult.y = gCj;
			if (bOdd)
				--gpResult.y;
			if (dCrit < 0)
				++gpResult.y;
		}

		return gpResult;
	}
	else 
	{
		if (pixPoint.x < 0) 
		{
			pixPoint.x -= m_sFieldSize - 1;
		}
		if (pixPoint.y < 0) 
		{
			pixPoint.y -= m_sFieldSize - 1;
		}

		GridPoint gp = GridPoint( pixPoint.x / m_sFieldSize, pixPoint.y / m_sFieldSize ); 

		return gp;
	}
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
    ).ClipToGrid( );
}

PixelRect PixelCoordinates::Grid2PixelRect( GridRect const & rcGrid ) const 
{
    return PixelRect
    ( 
        Grid2PixelPos( rcGrid.GetStartPoint( ) ),
        Grid2PixelPos( rcGrid.GetEndPoint  ( ) ) + (m_sFieldSize - 1) 
    );
}

PixelRect PixelCoordinates::GetTextRect( GridPoint const & gp ) const
{
    long  const lHalfSizeInd = (5 * m_sFieldSize) / 16;
    PixelPoint ptCenter   = Grid2PixelPosCenter( gp );
//				Util::UpsideDown( m_hwnd, & ptCenter ); 
    return PixelRect( ptCenter - lHalfSizeInd, ptCenter + lHalfSizeInd );
}
