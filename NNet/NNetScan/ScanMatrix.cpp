// ScanMatrix.cpp
//
// NNetScan

module;

#include <memory>
#include <cassert>
#include <vector>
#include <iostream>
#include <optional>
#include <Windows.h>

module ScanMatrix;

import Types;
import Raster;
import SaveCast;
import ColorLUT;
import ScanPixel;
import NNetModel;

using std::unique_ptr;
using std::make_unique;
using std::vector;
using std::optional;
using std::nullopt;

void ScanMatrix::Initialize(ColorLUT const& colLUT)
{
    InitializeCriticalSection(&m_cs);
	m_pLut = &colLUT;
}

void ScanMatrix::SetModelInterface(NNetModelReaderInterface* const p)
{
    m_pNMRI = p;
    Notify(false);
}

void ScanMatrix::PrepareScanMatrix()
{
    EnterCriticalSection(&m_cs);
    if (m_bDirty)
    {
        if (m_pNMRI->GetScanAreaSize() != Size())
            m_scanPixels.Resize(m_pNMRI->GetScanAreaSize());
        else 
            Clear();
        m_pNMRI->Apply2AllC<Pipe>([this](Pipe const& p){ add2list(p); });
        findMaxNrOfDataPoints();
        m_bDirty = false;
    }
    LeaveCriticalSection(&m_cs);
}

mV ScanMatrix::Scan(RasterPoint const& rp)
{
    return m_scanPixels.GetPtr(rp)->Scan();
}

size_t ScanMatrix::NrOfDataPntsInPixel(RasterPoint const& rp) const
{
    return GetScanPixel(rp).NrOfDataPnts();
}

ScanPixel const& ScanMatrix::GetScanPixel(RasterPoint const& rp) const
{
    ScanPixel const * pPixel{ m_scanPixels.GetConstPtr(rp) };
    assert(pPixel);
    return *pPixel;
}

void ScanMatrix::add2list(Pipe const& pipe)
{
    pipe.Apply2AllSensorPoints
    (
        m_pNMRI->GetScanResolution(),
        [this](Pipe const& pipe, MicroMeterPnt const& umPnt, Pipe::SegNr const segNr)
        {
            if (optional<RasterPoint> const rPntOpt = m_pNMRI->FindRasterPos(umPnt))
            {
                ScanPixel* pScanPoint{ m_scanPixels.GetPtr(rPntOpt.value()) };
                pScanPoint->Add(ScanDataPoint(pipe, segNr));
            }
        }
    );
}

void ScanMatrix::Clear()
{
    Apply2AllScanPixels([](auto &p) { p.Clear(); });
}

void ScanMatrix::DensityCorrection(ScanImageRaw &image) const
{
    assert(image.Size() == m_scanPixels.Size());
	image.Divide([this](RasterPoint const& pnt){ return NrOfDataPntsInPixel(pnt); });
}

size_t ScanMatrix::NrOfDataPntsInMatrix() const
{
    size_t nr { 0 };
    Apply2AllScanPixelsC([&nr](ScanPixel const& p) { nr += p.NrOfDataPnts(); });
    return nr;
}

float ScanMatrix::AverageDataPointsPerPixel() const
{
    return DivideByArea(NrOfDataPntsInMatrix());
}

float ScanMatrix::DivideByArea(size_t const nrOfPoints) const
{
    size_t const nrOfPixels { m_scanPixels.NrOfPoints() };
    return (nrOfPixels > 0)
        ? Cast2Float(nrOfPoints) / Cast2Float(nrOfPixels)
        : 0.0f;
}

void ScanMatrix::findMaxNrOfDataPoints()
{
    m_maxNrOfDataPnts = 0;
    Apply2AllScanPixelsC
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
    Apply2AllScanPixelsC
    (
        [this, fCenter, &fVariance](ScanPixel const& p)
        {
            float const fDiff { Cast2Float(p.NrOfDataPnts()) - fCenter };
            fVariance += fDiff * fDiff;
        }
    );
    fVariance /= Cast2Float(m_scanPixels.NrOfPoints());
    return fVariance;
}

void ScanMatrix::drawScanRaster(DrawContext const& context)
{
	MicroMeter const umRes { m_pNMRI->GetScanResolution() };

	if (context.GetCoordC().Transform2fPixel(umRes) >= 8.0_fPixel)
	{
		Color          const colLine { NNetColors::SCAN_AREA_HANDLE };
		MicroMeterRect const umRect  { m_pNMRI->GetScanAreaRect() };
		RasterPoint    const rSize   { m_pNMRI->GetScanAreaSize() };
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

void ScanMatrix::drawScanImage(DrawContext const& context) const
{
	unique_ptr<ScanImageByte> upFiltered;
	ScanImageByte     const * pImage { m_pNMRI->GetScanImageC() };
	assert(pImage);

	if (NNetPreferences::ApplyFilter())
	{
		upFiltered = pImage->MeanFilter();
		upFiltered->Normalize(255.0f);
		pImage = upFiltered.get();
	}

	m_pNMRI->GetScanRaster().DrawRasterPoints
	(
		context, 
		[this, pImage](auto const &rp) -> Color
		{
			return m_pLut->GetColor(pImage->Get(rp));
		}
	);
}

MicroMeter ScanMatrix::getScanAreaHandleSize(Uniform2D<MicroMeter> const &coord)
{
	fPixel const HANDLE_SIZE { 8.0_fPixel };
	return coord.Transform2logUnit(HANDLE_SIZE);
}

bool ScanMatrix::SetScanAreaHandle
(
	DrawContext   const& context,
	MicroMeterPnt const& umCrsrPos
)
{
	MicroMeter    const umSize            { getScanAreaHandleSize(context.GetCoordC()) };
	optional<CardPoint> scanAreaHandleNew { nullopt };
	Apply2CardPoints
	(
		[this, umSize, &umCrsrPos, &scanAreaHandleNew](CardPoint const cardPnt)
		{ 
			MicroMeterPnt  const pos          { m_pNMRI->GetScanAreaRect().CardPntPos(cardPnt) };
			MicroMeterRect const umRectHandle { RectFromCenterAndExtension<MicroMeter>(pos, umSize) };
			if (umRectHandle.Includes(umCrsrPos))
				scanAreaHandleNew = cardPnt;
		}
	);
	if (scanAreaHandleNew != m_scanAreaHandleSelected)
	{
		m_scanAreaHandleSelected = scanAreaHandleNew;
		Notify(false);
	}
	return m_scanAreaHandleSelected.has_value();
}

void ScanMatrix::drawScanAreaHandles(DrawContext const& context)
{
	MicroMeter const umHandleSize { getScanAreaHandleSize(context.GetCoordC()) };
	Apply2CardPoints
	(
		[this, umHandleSize, &context](CardPoint const cp)
		{
			MicroMeterPnt  const pos    { m_pNMRI->GetScanAreaRect().CardPntPos(cp) };
			MicroMeterRect const umRect { RectFromCenterAndExtension<MicroMeter>(pos, umHandleSize) };
			Color          const col 
			{ 
				(m_scanAreaHandleSelected.has_value() && (cp == m_scanAreaHandleSelected.value()))
				? NNetColors::INT_SELECTED 
				: NNetColors::SCAN_AREA_HANDLE
			};
			context.DrawTranspRect(umRect, col);
		}
	);
}

ColorLUT ScanMatrix::sensorDensityLUT() const
{
	size_t const maxNrOfDataPnts { MaxNrOfDataPoints() };
	float const fAverageDataPnts { DivideByArea(maxNrOfDataPnts) };
	float       fLutIndexAverage { 255.0f * fAverageDataPnts / Cast2Float(maxNrOfDataPnts) };
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

void ScanMatrix::drawSensorDensityMap(DrawContext const& context)
{
	PrepareScanMatrix();
	if (MaxNrOfDataPoints() > 0.0f)
	{
		ColorLUT const lut { sensorDensityLUT() };
		m_pNMRI->GetScanRaster().DrawRasterPoints
		(
			context, 
			[this, &lut](auto const &rpRun) -> Color
			{
				size_t   const nrOfPnts { NrOfDataPntsInPixel(rpRun) };
				ColIndex const colIndex { Cast2Byte((255 * nrOfPnts) / MaxNrOfDataPoints()) };
				return lut.GetColor(colIndex);
			}
		);
	}
	else
	{
		DrawScanAreaBackground(context);
	}
}

void ScanMatrix::DrawScanAreaBackground(DrawContext const& context) const
{
	context.FillRectangle(m_pNMRI->GetScanAreaRect(), NNetColors::SCAN_AREA_RECT);
}

void ScanMatrix::DrawScanArea(DrawContext const& context)
{
	if (m_pNMRI->ModelLocked())
	{
		drawScanImage(context);
	}
	else
	{
		drawSensorDensityMap(context);
		drawScanAreaHandles(context);
	}
	drawScanRaster(context);
}
