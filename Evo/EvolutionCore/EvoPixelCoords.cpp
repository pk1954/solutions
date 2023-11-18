// EvoPixelCoords.cpp
//
// EvolutionCoreInterface

#include <algorithm>  // min/max templates
import GridDimensions;
#include "PixelTypes.h"
#include "EvoPixelCoords.h"

import GridPoint;

static double const SQRT3_DIV2 = sqrt(3.) / 2.;
static double const SQRT3_DIV3 = sqrt(3.) / 3.;

EvoPixelCoords::EvoPixelCoords()
  : m_pixOffset   (0_PIXEL),
	m_pixFieldSize(0_PIXEL),
	m_smoothMove  (),
	m_bMoving     (false),
	m_bHexagon    (false)
{ }

void EvoPixelCoords::Start
(
	PIXEL const fs, 
	bool  const bHexagon
)
{	
	m_pixOffset    = 0_PIXEL;
	m_pixFieldSize = fs;
	m_bMoving      = false;
	m_bHexagon     = bHexagon;
}

bool EvoPixelCoords::isValidFieldSize(PIXEL const pixNewFieldSize) const 
{ 
    return (MINIMUM_FIELD_SIZE <= pixNewFieldSize) && (pixNewFieldSize <= MAXIMUM_FIELD_SIZE); 
};

void EvoPixelCoords::MoveGrid(PixelPoint const pntDelta)
{
    m_pixOffset -= pntDelta;
}

PixelPoint EvoPixelCoords::calcCenterOffset    // calculate new pixel offset,
(                                             // which moves gridRect to center of window.
	GridPoint  const gpCenter,                 // do not yet set m_pixOffset to this value!
	PixelPoint const pixCenter 
)  
{
	assert(IsInGrid(gpCenter));
	PixelPoint const pixPnt(Grid2PixelSize(gpCenter) + m_pixFieldSize / 2);
	PixelPoint const pixOffset(pixPnt - pixCenter);
	return pixOffset;
}

bool EvoPixelCoords::CenterPoi(PixelPoint const pixCenter, GridPoint const gpPoi) // returns TRUE, if POI was already centered, or if no POI defined
{
    if (gpPoi.IsNull())
        return true;

    PixelPoint pixOffsetDesired(calcCenterOffset(gpPoi, pixCenter));
    bool       bCentered(m_pixOffset == pixOffsetDesired);

    if (! bCentered)
        m_pixOffset = m_smoothMove.Step(m_pixOffset, pixOffsetDesired);

    return bCentered;
}

void EvoPixelCoords::CenterGrid
(
	GridPoint     const gpCenter,   
	PixelRectSize const pntPixSize  // available size 
)
{
    m_pixOffset = calcCenterOffset(gpCenter, PixelPoint(pntPixSize.GetX(), pntPixSize.GetY()) / 2);
}

PIXEL EvoPixelCoords::CalcMaximumFieldSize
(
	GridRectSize  const & gpGridRectSize,   // Grid size to fit into window
	PixelRectSize const & pntPixSize        // available pixel size 
) const
{
	PIXEL xSize = pntPixSize.GetX() / gpGridRectSize.GetXvalue();
	PIXEL ySize = pntPixSize.GetY() / gpGridRectSize.GetYvalue(); 
	return PIXEL(std::min(xSize.GetValue(), ySize.GetValue()));
}

bool EvoPixelCoords::SetGridFieldSize(PIXEL const pixNewFieldSize)
{
    bool bValid = isValidFieldSize(pixNewFieldSize);
	if (bValid)
	    m_pixFieldSize = pixNewFieldSize;
	return bValid;
}

PIXEL EvoPixelCoords::ComputeNewFieldSize(bool const bZoomIn) const
{
	PIXEL pixNewFieldSize { m_pixFieldSize };
    if (bZoomIn)
    {
        PIXEL const pixDelta
		(
			(m_pixFieldSize < 16_PIXEL) 
			? 1_PIXEL
			: (
				(m_pixFieldSize < 64_PIXEL) 
				? 16_PIXEL
				: 32_PIXEL
			  ) 
		);
        pixNewFieldSize += pixDelta;
    }
    else
    {
        PIXEL const pixDelta((m_pixFieldSize <= 16_PIXEL) ? 1_PIXEL : ((m_pixFieldSize <= 64_PIXEL) ? 16_PIXEL : 32_PIXEL));
        pixNewFieldSize -= pixDelta;
    }
	return isValidFieldSize(pixNewFieldSize) ? pixNewFieldSize : m_pixFieldSize;
}

PixelPoint EvoPixelCoords::Grid2PixelSize(GridPoint const gp) const 
{ 
	PIXEL pixX { m_pixFieldSize * gp.GetXvalue() };
	PIXEL pixY { m_pixFieldSize * gp.GetYvalue() };

	if (m_bHexagon)
	{
		pixX = PIXEL(CastToLong(static_cast<double>(pixX.GetValue()) * SQRT3_DIV2 + 0.5));
	}

	return PixelPoint(PIXEL(pixX), PIXEL(pixY));
}

PixelPoint EvoPixelCoords::Grid2PixelPos(GridPoint const gp) const 
{ 
	PixelPoint ppRes { Grid2PixelSize(gp) - m_pixOffset };

	if (m_bHexagon && IsOddColumn(gp))
		ppRes -= PixelPoint(0_PIXEL, PIXEL(m_pixFieldSize / 2));

	return ppRes;
}

PixelPoint EvoPixelCoords::Grid2PixelPosCenter(GridPoint const gp) const 
{ 
	PixelPoint ppRes { Grid2PixelPos(gp) };
	if (m_bHexagon)
	{
		ppRes += PixelPoint(
			                  PIXEL(static_cast<long>(SQRT3_DIV3 * m_pixFieldSize.GetValue())),
		                      PIXEL(m_pixFieldSize / 2) 
			              );
	}
	else
	{
		ppRes += m_pixFieldSize / 2; 
	}
	return ppRes;
}

GridPoint EvoPixelCoords::Pixel2GridPos(PixelPoint const pp) const 
{ 
	PixelPoint pixPoint(pp + m_pixOffset);

	if (m_bHexagon) // adapted from http://blog.ruslans.com/2011/02/hexagonal-grid-math.html
	{
		double const dFieldSize = static_cast<double>(m_pixFieldSize.GetValue());
		double const dRadius    = dFieldSize * SQRT3_DIV3;
		double const dSide      = dFieldSize * SQRT3_DIV2;

		double const dPixPointX = static_cast<double>(pixPoint.GetXvalue());
		double const dCi        = floor(dPixPointX/dSide);
		double const dCx        = dPixPointX - dSide * dCi;

		GridCoord const gCi    = GridCoord(CastToShort(dCi));
		bool       const bOdd   = IsOdd(gCi);

		double const dPixPointY = static_cast<double>(pixPoint.GetYvalue());
		double const dTy        = dPixPointY + (bOdd ? (dFieldSize * 0.5) : 0);
		double const dCj        = floor(dTy/dFieldSize);
		double const dCy        = dTy - dFieldSize * dCj;
		double const dCrit      = 0.5 - dCy / dFieldSize;

		GridPoint gpResult(GridCoord(gCi), GridCoord(CastToShort(dCj)));

		if (dCx <= dRadius * abs(dCrit))
		{
			gpResult -= GridPoint(1_GRID_COORD, 0_GRID_COORD);
			if (bOdd)
				gpResult -= GridPoint(0_GRID_COORD, 1_GRID_COORD);
			if (dCrit < 0)
				gpResult += GridPoint(0_GRID_COORD, 1_GRID_COORD);
		}

		return gpResult;
	}
	else 
	{
		if (pixPoint.GetX() < 0_PIXEL) 
		{
			pixPoint -= PixelPoint(m_pixFieldSize - 1_PIXEL, 0_PIXEL);
		}
		if (pixPoint.GetY() < 0_PIXEL) 
		{
			pixPoint -= PixelPoint(0_PIXEL, m_pixFieldSize - 1_PIXEL);
		}

		GridPoint gp = GridPoint
		(
			GridCoord(CastToShort(pixPoint.GetXvalue() / m_pixFieldSize.GetValue())), 
			GridCoord(CastToShort(pixPoint.GetYvalue() / m_pixFieldSize.GetValue())) 
		); 

		return gp;
	}
}

GridRect EvoPixelCoords::Pixel2GridRect(PixelRect const & rect) const 
{
    GridRect gridRect
    (
        ClipToGrid(Pixel2GridPos(rect.GetStartPoint())), 
        ClipToGrid(Pixel2GridPos(rect.GetEndPoint  ()))
   );
	return gridRect;
}

PixelRect EvoPixelCoords::Grid2PixelRect(GridRect const & rcGrid) const 
{
    return PixelRect
    (
        Grid2PixelPos(rcGrid.GetStartPoint()),
        Grid2PixelPos(rcGrid.GetEndPoint  ()) + (m_pixFieldSize - 1_PIXEL) 
   );
}

PixelPoint EvoPixel2PixelSize(PixelPoint const ptSizeIn, EvoPixelCoords const * pSrc, EvoPixelCoords const * pDst)
{
    return (ptSizeIn * pDst->GetFieldSize().GetValue()) / pSrc->GetFieldSize().GetValue();
}

PixelPoint EvoPixel2PixelPos(PixelPoint const ptPosIn, EvoPixelCoords const * pSrc, EvoPixelCoords const * pDst) 
{
    return EvoPixel2PixelSize(ptPosIn + pSrc->GetPixelOffset(), pSrc, pDst) - pDst->GetPixelOffset();
}

PixelRect EvoPixel2PixelRect(PixelRect const & rect, EvoPixelCoords const * pSrc, EvoPixelCoords const * pDst)
{
	return PixelRect
	(
		EvoPixel2PixelPos(rect.GetStartPoint(), pSrc, pDst),
		EvoPixel2PixelPos(rect.GetEndPoint(),   pSrc, pDst)
	);
}

