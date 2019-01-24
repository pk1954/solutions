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
    m_pixFieldSize( fs ),
    m_smoothMove(  ),
	m_bMoving   ( false ),
	m_bHexagon  ( bHexagon )
{ }

bool PixelCoordinates::isValidFieldSize( PIXEL const pixNewFieldSize ) const 
{ 
    return (MINIMUM_FIELD_SIZE <= pixNewFieldSize) && (pixNewFieldSize <= MAXIMUM_FIELD_SIZE); 
};

void PixelCoordinates::MoveGrid( PixelPoint const pntDelta )
{
    m_pixOffset -= pntDelta;
}

PixelPoint PixelCoordinates::calcCenterOffset  // calculate new pixel offset,
(                                              // which moves gridRect to center of window.
	GridPoint  const gpCenter,                 // do not yet set m_pixOffset to this value!
	PixelPoint const pixCenter 
)  
{
	assert( gpCenter.IsInGrid());
	PixelPoint const pixPnt( Grid2PixelSize(gpCenter) + m_pixFieldSize / 2 );
	PixelPoint const pixOffset( pixPnt - pixCenter );
	return pixOffset;
}

bool PixelCoordinates::CenterPoi( PixelPoint const pixCenter, GridPoint const gpPoi ) // returns TRUE, if POI was already centered, or if no POI defined
{
    if ( gpPoi.IsNull( ) )
        return true;

    PixelPoint pixOffsetDesired( calcCenterOffset( gpPoi, pixCenter ) );
    bool       bCentered( m_pixOffset == pixOffsetDesired );

    if ( ! bCentered )
        m_pixOffset = m_smoothMove.Step( m_pixOffset, pixOffsetDesired );

    return bCentered;
}

void PixelCoordinates::CenterGrid
( 
	GridPoint     const gpCenter,   
	PixelRectSize const pntPixSize  // available size 
)
{
    m_pixOffset = calcCenterOffset( gpCenter, pntPixSize.ToPixelPoint() / 2 );
}

PIXEL PixelCoordinates::CalcMaximumFieldSize
( 
	GridPoint     const & gpGridRectSize,   // Grid size to fit into window
	PixelRectSize const   pntPixSize        // available pixel size 
)
{
	return min( 
				pntPixSize.GetWidth()  / gpGridRectSize.x.get(), 
				pntPixSize.GetHeight() / gpGridRectSize.y.get() 
	          );
}

bool PixelCoordinates::SetGridFieldSize( PIXEL const pixNewFieldSize )
{
    bool bValid = isValidFieldSize( pixNewFieldSize );
	if ( bValid )
	    m_pixFieldSize = pixNewFieldSize;
	return bValid;
}

PIXEL PixelCoordinates::ComputeNewFieldSize( bool const bZoomIn ) const
{
    PIXEL pixNewFieldSize( m_pixFieldSize );
    if ( bZoomIn )
    {
        PIXEL const pixDelta
		( 
			( m_pixFieldSize < PIXEL(16_PIXEL ) 
			? PIXEL(1_PIXEL) 
			: (
				( m_pixFieldSize < PIXEL(64_PIXEL) ) 
				? PIXEL(16_PIXEL) 
				: PIXEL(32_PIXEL)
			   ) 
			)
		);
        pixNewFieldSize += pixDelta;
    }
    else
    {
        PIXEL const pixDelta( ( m_pixFieldSize <= PIXEL(16_PIXEL) ) ? PIXEL(1_PIXEL) : (( m_pixFieldSize <= PIXEL(64_PIXEL) ) ? PIXEL(16_PIXEL) : PIXEL(32_PIXEL)) );
        pixNewFieldSize -= pixDelta;
    }
	return pixNewFieldSize;
}

PixelPoint PixelCoordinates::Pixel2PixelSize( PixelPoint const ptSizeIn, PixelCoordinates const & fTarget ) const 
{
    static long const FACTOR( 1024 ); // to avoid zero when dividing small ptSizeIn by m_pixFieldSize 

    return (((ptSizeIn * FACTOR) / m_pixFieldSize.get()) * fTarget.m_pixFieldSize.get() ) / FACTOR;
}

PixelPoint PixelCoordinates::Pixel2PixelPos( PixelPoint const ptPosIn, PixelCoordinates const & fTarget ) const 
{
    return Pixel2PixelSize( ptPosIn + m_pixOffset, fTarget ) - fTarget.m_pixOffset;
}

KGridPoint PixelCoordinates::Pixel2KGridPos( PixelPoint const pp ) const 
{ 
	return Pixel2KGridSize( pp + m_pixOffset, m_pixFieldSize ); 
}

PixelPoint PixelCoordinates::KGrid2PixelPos( KGridPoint const kp ) const 
{ 
	return KGrid2PixelSize( kp, m_pixFieldSize ) - m_pixOffset; 
}
    
PixelPoint PixelCoordinates::Grid2PixelSize( GridPoint const gp ) const 
{ 
	PixelPoint ppRes{ m_pixFieldSize * gp.x.get(), m_pixFieldSize * gp.y.get() };
	
	if ( m_bHexagon )
		ppRes.x = PIXEL( static_cast<long>( ppRes.x.get() * SQRT3_DIV2 + 0.5 ) );

	return ppRes;
}

PixelPoint PixelCoordinates::Grid2PixelPos( GridPoint const gp ) const 
{ 
	PixelPoint ppRes{ Grid2PixelSize( gp ) - m_pixOffset };

	if ( m_bHexagon && gp.IsOddColumn( ) )
		ppRes.y -= m_pixFieldSize / 2 ;

	return ppRes;
}

PixelPoint PixelCoordinates::Grid2PixelPosCenter( GridPoint const gp ) const 
{ 
	if (m_bHexagon)
	{
		PixelPoint pxResult( Grid2PixelPos( gp ) );
		pxResult.x += PIXEL(static_cast<long>(SQRT3_DIV3 * m_pixFieldSize.get()));
		pxResult.y += m_pixFieldSize / 2;
		return pxResult;
	}
	else
		return Grid2PixelPos( gp ) + m_pixFieldSize / 2; 
}
PixelRect PixelCoordinates::GridPoint2PixelRect( GridPoint const gp ) const
{
	return PixelRect
	(
		Grid2PixelPos( gp ),  // PixelPoint 
		PixelRectSize( m_pixFieldSize, m_pixFieldSize ) 
	);
}

GridPoint PixelCoordinates::Pixel2GridPos( PixelPoint const pp ) const 
{ 
	PixelPoint pixPoint( pp + m_pixOffset );

	if ( m_bHexagon ) // adapted from http://blog.ruslans.com/2011/02/hexagonal-grid-math.html
	{
		double const dFieldSize = static_cast<double>(m_pixFieldSize.get());
		double const dRadius    = dFieldSize * SQRT3_DIV3;
		double const dSide      = dFieldSize * SQRT3_DIV2;

		double const dPixPointX = static_cast<double>(pixPoint.x.get());
		double const dCi        = floor(dPixPointX/dSide);
		double const dCx        = dPixPointX - dSide * dCi;

		GRID_COORD const gCi( CastToShort(dCi) );  //TODO: check if ok
		bool       const bOdd   = ((gCi.get() % 2) != 0);

		double const dPixPointY = static_cast<double>(pixPoint.y.get());
		double const dTy        = dPixPointY + (bOdd ? (dFieldSize * 0.5) : 0);
		double const dCj        = floor(dTy/dFieldSize);
		double const dCy        = dTy - dFieldSize * dCj;
		double const dCrit      = 0.5 - dCy / dFieldSize;

		GRID_COORD const gCj( CastToShort(dCj) );

		GridPoint gpResult;

		if (dCx > dRadius * abs(dCrit))
		{
			gpResult.x = gCi;
			gpResult.y = gCj;
		}
		else
		{
			gpResult.x = gCi - GRID_COORD(1_GRID_COORD);
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
		if (pixPoint.x < PIXEL(0_PIXEL)) 
		{
			pixPoint.x -= m_pixFieldSize - PIXEL(1_PIXEL);
		}
		if (pixPoint.y < PIXEL(0_PIXEL)) 
		{
			pixPoint.y -= m_pixFieldSize - PIXEL(1_PIXEL);
		}

		GridPoint gp = GridPoint
		( 
			GRID_COORD( CastToShort(pixPoint.x / m_pixFieldSize) ), 
			GRID_COORD( CastToShort(pixPoint.y / m_pixFieldSize) ) 
		); 

		return gp;
	}
}

KGridRect PixelCoordinates::Pixel2KGridRect( PixelRect const & rect ) const 
{
    return KGridRect 
    (
        Pixel2KGridPos ( rect.GetStartPoint() ),
        Pixel2KGridSize( rect.GetSize().ToPixelPoint(), m_pixFieldSize )
    );
}

PixelRect PixelCoordinates::KGrid2PixelRect( KGridRect const & kgrIn ) const 
{
    PixelPoint const ptPos ( KGrid2PixelPos ( kgrIn.GetPos ()  ) );
    PixelPoint const ptSize( KGrid2PixelSize( kgrIn.GetSize(), m_pixFieldSize ) );
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
        Grid2PixelPos( rcGrid.GetEndPoint  ( ) ) + (m_pixFieldSize - PIXEL(1_PIXEL)) 
    );
}
