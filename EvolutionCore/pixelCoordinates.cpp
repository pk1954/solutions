// pixelCoordinates.cpp
//

#include "stdafx.h"
#include <algorithm>  // min/max templates
#include "gridPoint.h"
#include "pixelPoint.h"
#include "pixelRect.h"
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

void PixelCoordinates::MoveGrid( PixelPoint const pntDelta )
{
    m_pixOffset -= pntDelta;
	CheckPixelPoint( m_pixOffset );
}

PixelPoint PixelCoordinates::calcCenterOffset  // calculate new pixel offset,
(                                              // which moves gridRect to center of window.
	GridPoint  const gpCenter,                 // do not yet set m_pixOffset to this value!
	PixelPoint const pixCenter 
)  
{
	assert( gpCenter.IsInGrid());
	PixelPoint const pixPnt( Grid2PixelSize(gpCenter) + m_sFieldSize / 2 );
	PixelPoint const pixOffset( pixPnt - pixCenter );
	return pixOffset;
}

bool PixelCoordinates::CenterPoi( PixelPoint const pixCenter, GridPoint const gpPoi ) // returns TRUE, if POI was already centered, or if no POI defined
{
    if ( gpPoi.IsNull( ) )
        return true;

    PixelPoint pixOffsetDesired( calcCenterOffset( gpPoi, pixCenter ) );
    bool       bCentered( m_pixOffset == pixOffsetDesired );
	CheckPixelPoint( pixOffsetDesired );

    if ( ! bCentered )
	{
        m_pixOffset = m_smoothMove.Step( m_pixOffset, pixOffsetDesired );
		CheckPixelPoint( m_pixOffset );
	}

    return bCentered;
}

void PixelCoordinates::CenterGrid
( 
	GridPoint     const gpCenter,   
	PixelRectSize const pntPixSize  // available size 
)
{
    m_pixOffset = calcCenterOffset( gpCenter, pntPixSize.ToPixelPoint() / 2 );
	CheckPixelPoint( m_pixOffset );
}

short PixelCoordinates::CalcMaximumFieldSize
( 
	GridPoint     const & gpGridRectSize,   // Grid size to fit into window
	PixelRectSize const   pntPixSize        // available pixel size 
)
{
	PixelRectSize pixRectSize = pntPixSize / PixelRectSize{ gpGridRectSize.x, gpGridRectSize.y };
	return min( pixRectSize.GetWidth(), pixRectSize.GetHeight() );
}

bool PixelCoordinates::SetGridFieldSize( short const sNewFieldSize )
{
    bool bValid = isValidFieldSize( sNewFieldSize );
	if ( bValid )
	    m_sFieldSize = sNewFieldSize;
	return bValid;
}

short PixelCoordinates::ComputeNewFieldSize( bool const bZoomIn ) const
{
    short sNewFieldSize( m_sFieldSize );
    if ( bZoomIn )
    {
        short const sDelta( ( m_sFieldSize < 16 ) ? 1 : (( m_sFieldSize < 64 ) ? 16 : 32) );
        sNewFieldSize += sDelta;
    }
    else
    {
        short const sDelta( ( m_sFieldSize <= 16 ) ? 1 : (( m_sFieldSize <= 64 ) ? 16 : 32) );
        sNewFieldSize -= sDelta;
    }
	return sNewFieldSize;
}

PixelPoint PixelCoordinates::Pixel2PixelSize( PixelPoint const ptSizeIn, PixelCoordinates const & fTarget ) const 
{
    static long const FACTOR( 1024 ); // to avoid zero when dividing small ptSizeIn by m_sFieldSize 

    return (((ptSizeIn * FACTOR) / m_sFieldSize) * fTarget.m_sFieldSize ) / FACTOR;
}

PixelPoint PixelCoordinates::Pixel2PixelPos( PixelPoint const ptPosIn, PixelCoordinates const & fTarget ) const 
{
    return Pixel2PixelSize( ptPosIn + m_pixOffset, fTarget ) - fTarget.m_pixOffset;
}

KGridPoint PixelCoordinates::Pixel2KGridPos( PixelPoint const pp ) const 
{ 
	return Pixel2KGridSize( pp + m_pixOffset, m_sFieldSize ); 
}

PixelPoint PixelCoordinates::KGrid2PixelPos( KGridPoint const kp ) const 
{ 
	return KGrid2PixelSize( kp, m_sFieldSize ) - m_pixOffset; 
}
    
PixelPoint PixelCoordinates::Grid2PixelSize( GridPoint const gp ) const 
{ 
	PixelPoint ppRes( gp.x * m_sFieldSize, gp.y * m_sFieldSize );
	
	if ( m_bHexagon )
		ppRes.x = static_cast<long>( ppRes.x * SQRT3_DIV2 + 0.5 );

	return ppRes;
}

PixelPoint PixelCoordinates::Grid2PixelPos( GridPoint const gp ) const 
{ 
	PixelPoint ppRes( Grid2PixelSize( gp ) - m_pixOffset );

	if ( m_bHexagon && gp.IsOddCol( ) )
		ppRes.y -= m_sFieldSize / 2 ;

	return ppRes;
}

PixelPoint PixelCoordinates::Grid2PixelPosCenter( GridPoint const gp ) const 
{ 
	if (m_bHexagon)
	{
		PixelPoint pxResult( Grid2PixelPos( gp ) );
		pxResult.x += static_cast<long>(SQRT3_DIV3 * m_sFieldSize);
		pxResult.y += m_sFieldSize / 2;
		return pxResult;
	}
	else
		return Grid2PixelPos( gp ) + m_sFieldSize / 2; 
}
PixelRect PixelCoordinates::GridPoint2PixelRect( GridPoint const gp ) const
{
	return PixelRect
	(
		Grid2PixelPos( gp ),  // PixelPoint 
		PixelRectSize( m_sFieldSize, m_sFieldSize ) 
	);
}

GridPoint PixelCoordinates::Pixel2GridPos( PixelPoint const pp ) const 
{ 
	PixelPoint pixPoint( pp + m_pixOffset );

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
    return KGridRect 
    (
        Pixel2KGridPos ( rect.GetStartPoint() ),
        Pixel2KGridSize( rect.GetSize().ToPixelPoint(), m_sFieldSize )
    );
}

PixelRect PixelCoordinates::KGrid2PixelRect( KGridRect const & kgrIn ) const 
{
    PixelPoint const ptPos ( KGrid2PixelPos ( kgrIn.GetPos ()  ) );
    PixelPoint const ptSize( KGrid2PixelSize( kgrIn.GetSize(), m_sFieldSize ) );
    return PixelRect( ptPos, ptPos + ptSize );
}

GridRect PixelCoordinates::Pixel2GridRect( PixelRect const & rect ) const 
{
    GridRect gridRect
    ( 
        Pixel2GridPos( rect.GetStartPoint() ), 
        Pixel2GridPos( rect.GetEndPoint  () ) 
    );
	GridRect gridRectRes = gridRect.ClipToGrid( );
	return gridRectRes;
}

PixelRect PixelCoordinates::Grid2PixelRect( GridRect const & rcGrid ) const 
{
    return PixelRect
    ( 
        Grid2PixelPos( rcGrid.GetStartPoint( ) ),
        Grid2PixelPos( rcGrid.GetEndPoint  ( ) ) + (m_sFieldSize - 1) 
    );
}
