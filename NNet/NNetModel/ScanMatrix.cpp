// ScanMatrix.cpp
//
// NNetModel

module;

#include <Windows.h>

module NNetModel:ScanMatrix;

import std;
import Debug;
import Types;
import Raster;
import SaveCast;
import ColorLUT;
import D2D_ColorLUT;
import :ScanPixel;
import :NNetColors;
import :UPNobList;

using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::optional;
using std::nullopt;

void ScanMatrix::Prepare
(
	Raster    const& raster,
	UPNobList const& nobList
)
{
    if (raster.Size() != Size())
        m_scanPixels.Resize(raster.Size());
    Clear();
    nobList.Apply2AllC<Pipe>([this, &raster](Pipe const& p){ add2list(p, raster); });
    findMaxNrOfDataPoints();
}

void ScanMatrix::add2list(Pipe const& pipe, Raster const& raster) 
{
    pipe.Apply2AllSensorPoints
    (
        raster.Resolution(),
        [this, &raster](Pipe const& pipe, MicroMeterPnt const& umPnt, Pipe::SegNr const segNr)
        {
            if (optional<RasterPoint> const rPntOpt = raster.FindRasterPos(umPnt))
            {
                ScanPixel* pScanPoint{ m_scanPixels.GetPtr(rPntOpt.value()) };
                pScanPoint->Add(ScanDataPoint(pipe, segNr));
            }
        }
    );
}

void ScanMatrix::DensityCorrection(RawImage &image) const
{
    Assert(image.Size() == m_scanPixels.Size());
	image.Divide([this](RasterPoint const& pnt){ return NrOfDataPntsInPixel(pnt); });
}

size_t ScanMatrix::NrOfDataPntsInMatrix() const
{
    size_t nr { 0 };
    m_scanPixels.Apply2AllPixelsC([&nr](ScanPixel const& p) { nr += p.NrOfDataPnts(); });
    return nr;
}

float ScanMatrix::DivideByArea(size_t const nrOfPoints) const
{
    size_t const nrOfPixels { m_scanPixels.NrOfRasterPoints() };
    return (nrOfPixels > 0)
        ? Cast2Float(nrOfPoints) / Cast2Float(nrOfPixels)
        : 0.0f;
}

void ScanMatrix::findMaxNrOfDataPoints()
{
    m_maxNrOfDataPnts = 0;
    m_scanPixels.Apply2AllPixelsC
    (
        [this](ScanPixel const& p)
        {
            if (p.NrOfDataPnts() > m_maxNrOfDataPnts)
                m_maxNrOfDataPnts = p.NrOfDataPnts();
        }
    );
}

float ScanMatrix::DataPointVariance()
{
    float  fCenter   { AverageDataPointsPerPixel() };
    float  fVariance { 0.0f };
    m_scanPixels.Apply2AllPixelsC
    (
        [this, fCenter, &fVariance](ScanPixel const& p)
        {
            float const fDiff { Cast2Float(p.NrOfDataPnts()) - fCenter };
            fVariance += fDiff * fDiff;
        }
    );
    fVariance /= Cast2Float(m_scanPixels.NrOfRasterPoints());
    return fVariance;
}

void ScanMatrix::DrawScanRaster(DrawContext const& context, Raster const& raster) const
{
	MicroMeter const umRes { raster.Resolution() };

	if (context.GetCoordC().Transform2fPixel(umRes) >= 8.0_fPixel)
	{
		Color          const colLine { NNetColors::SCAN_AREA_HANDLE };
		MicroMeterRect const umRect  { raster.GetScanArea() };
		RasterPoint    const rSize   { raster.Size() };
		MicroMeter     const umYend  { umRect.GetTop()  + umRes * Cast2Float(rSize.m_y) };
		MicroMeter     const umXend  { umRect.GetLeft() + umRes * Cast2Float(rSize.m_x) };
		for (int x = 0; x <= rSize.m_x; ++x)
		{
			MicroMeter const umX { umRect.GetLeft() + umRes * Cast2Float(x) };
			context.DrawLine
			(
				MicroMeterPnt(umX, umRect.GetTop()),
				MicroMeterPnt(umX, umYend),
				0.0_MicroMeter,
				colLine
			);
		}
		for (int y = 0; y <= rSize.m_y; ++y)
		{
			MicroMeter const umY { umRect.GetTop() + umRes * Cast2Float(y) };
			context.DrawLine
			(
				MicroMeterPnt(umRect.GetLeft(), umY),
				MicroMeterPnt(umXend, umY),
				0.0_MicroMeter,
				colLine
			);
		}
	}
}

void ScanMatrix::DrawScanImage
(
	DrawContext  const& context,
	Raster       const& raster, 
	RawImage     const& image, 
	mV           const  mVmax,
	D2D_ColorLUT const& lut
) const
{
	float const fFactor { 255.0f / mVmax };
	context.FillRectangle(raster.GetScanArea(), D2D1::ColorF::Black);
	raster.DrawRasterPoints
	(
		context, 
		[&image, &lut, fFactor](auto const &rp) -> ID2D1Brush*
		{
			mV       const voltage { image.Get(rp) };
			ColIndex const index   { Cast2Byte(voltage.GetValue() * fFactor) };
			return lut.GetBrush(index);
		}
	);
}

void ScanMatrix::DrawScanProgress
(
	DrawContext const& context,
	Raster      const& raster, 
	RasterPoint const& rpProgress
) const
{
	raster.DrawRasterPoints
	(
		context, 
		[&rpProgress](auto const &rp) -> Color
		{
			return (rp < rpProgress) ? D2D1::ColorF::Red : D2D1::ColorF::Black;
		}
	);
}

MicroMeter ScanMatrix::getScanAreaHandleSize(Uniform2D<MicroMeter> const &coord) const
{
	fPixel const HANDLE_SIZE { 8.0_fPixel };
	return coord.Transform2logUnit(HANDLE_SIZE);
}

MicroMeterRect ScanMatrix::getRectHandle
(
	MicroMeter     const umHandleSize,
	MicroMeterRect const umRect,
	CardPoint      const cardPnt
) const
{
	MicroMeterPnt  const pos          { umRect.CardPntPos(cardPnt) };
	MicroMeterRect const umRectHandle { RectFromCenterAndExtension<MicroMeter>(pos, umHandleSize) };
	return umRectHandle;
}

Color ScanMatrix::handleColor
(
	CardPoint           const cardPnt,
	optional<CardPoint> const cardPntSelected
) const
{ 
	return (cardPntSelected.has_value() && (cardPnt == cardPntSelected.value()))
		? NNetColors::INT_SELECTED
		: NNetColors::SCAN_AREA_HANDLE;
}

optional<CardPoint> ScanMatrix::SelectScanAreaHandle
(
	DrawContext   const& context,
	Raster        const& raster,
	MicroMeterPnt const& umCrsrPos
) const
{
	MicroMeter     const umHandleSize { getScanAreaHandleSize(context.GetCoordC()) };
	MicroMeterRect const umRect       { raster.GetScanArea() };
	optional<CardPoint>  cardPnt      { nullopt };
	Apply2CardPoints
	(
		[this, umHandleSize, &umRect, &umCrsrPos, &cardPnt](CardPoint const cp)
		{ 
			if (getRectHandle(umHandleSize, umRect, cp).Includes(umCrsrPos))
				cardPnt = cp;
		}
	);
	return cardPnt;
}

void ScanMatrix::DrawScanAreaHandles
(
	DrawContext         const& context,
	Raster              const& raster,
	optional<CardPoint> const  cardPntSelected
) const
{
	MicroMeter     const umHandleSize { getScanAreaHandleSize(context.GetCoordC()) };
	MicroMeterRect const umRect       { raster.GetScanArea() };
	Apply2CardPoints
	(
		[this, umHandleSize, &umRect, &context, cardPntSelected](CardPoint const cardPnt)
		{
			context.DrawTranspRect
			(
				getRectHandle(umHandleSize, umRect, cardPnt), 
				handleColor(cardPnt, cardPntSelected)
			);
		}
	);
}

ColorLUT ScanMatrix::sensorDensityLUT() const
{
	size_t const maxNrOfDataPnts  { MaxNrOfDataPoints() };
	float  const fAverageDataPnts { DivideByArea(maxNrOfDataPnts) };
	float        fLutIndexAverage { 255.0f * fAverageDataPnts / Cast2Float(maxNrOfDataPnts) };
	if (fLutIndexAverage < 1.0f)
		fLutIndexAverage = 1.0f;
	else if (fLutIndexAverage > 254.0f)
		fLutIndexAverage = 254.0f;
	int const indexAverage { Cast2Int(fLutIndexAverage) };
	ColorLUT lut;
    lut.AddBasePoint(           0, Color(D2D1::ColorF::Black));
	lut.AddBasePoint(indexAverage, Color(D2D1::ColorF::Green));
    lut.AddBasePoint(         255, Color(D2D1::ColorF::Yellow));
    lut.Construct();
	return lut;
}

void ScanMatrix::DrawSensorDensityMap
(
	DrawContext const& context,
	Raster      const& raster,
	UPNobList   const& nobList
) const
{
	if (MaxNrOfDataPoints() > 0.0f)
	{
		ColorLUT const lut     { sensorDensityLUT() };	
		float    const fFactor { 255.0f / MaxNrOfDataPoints() };

		raster.DrawRasterPoints
		(
			context, 
			[this, &lut, fFactor](auto const &rpRun) -> Color
			{
				size_t   const nrOfPnts { NrOfDataPntsInPixel(rpRun) };
				ColIndex const index    { Cast2Byte(NrOfDataPntsInPixel(rpRun) * fFactor) };
				return lut.GetColor(index);
			}
		);
	}
	else
	{
		DrawScanAreaBackground(context, raster);
	}
}

void ScanMatrix::DrawScanAreaBackground
(
	DrawContext const& context,
	Raster      const& raster
) const
{
	context.FillRectangle(raster.GetScanArea(), NNetColors::SCAN_AREA_RECT);
}
