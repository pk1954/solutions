// ScanMatrix.cpp
//
// NNetScan

module;

#include <cassert>
#include <vector>
#include <optional>

module ScanMatrix;

import Types;
import Raster;
import SaveCast;
import ScanPixel;
import NNetModel;

using std::vector;
using std::optional;

ScanMatrix::ScanMatrix(RasterPoint const& rpSize)
    :m_scanPixels(rpSize)
{}

mV ScanMatrix::Scan(RasterPoint const& rp)
{
    return m_scanPixels.GetPtr(rp)->Scan();
}

void ScanMatrix::Resize(RasterPoint const& rpSize)
{
    m_scanPixels.Resize(rpSize);
    m_pScanPixelMax = nullptr;
}

ScanPixel const& ScanMatrix::GetScanPixel(RasterPoint const& rp) const
{
    ScanPixel const * pPixel{ m_scanPixels.GetConstPtr(rp) };
    assert(pPixel);
    return *pPixel;
}

void ScanMatrix::Add2list
(
    Pipe   const& pipe,
    Raster const& raster
)
{
    pipe.Apply2AllSensorPoints
    (
        raster.Resolution(),
        [this, &raster](Pipe const& pipe, MicroMeterPnt const& umPnt, Pipe::SegNr const segNr)
        {
            if (optional<RasterPoint> const rPntOpt = raster.FindRasterPos(umPnt))
                addScanDataPoint(pipe, segNr, rPntOpt.value());
        }
    );
}

void ScanMatrix::Clear()
{
    Apply2AllScanPixels([](auto &p) { p.Clear(); });
}

void ScanMatrix::Fill(NNetModelReaderInterface const& nmri)
{
    Raster const& raster = nmri.GetScanRaster();
    nmri.Apply2AllC<Pipe>
    (
        [this, raster](Pipe const& p)
        {
            Add2list(p, raster);
        }
    );
}

size_t ScanMatrix::GetNrOfDataPoints() const
{
    size_t nr { 0 };
    Apply2AllScanPixelsC([&nr](ScanPixel const& p) { nr += p.GetNrOfDataPoints(); });
    return nr;
}

float ScanMatrix::AverageDataPointsPerPixel() const
{
    return AverageDataPointsPerPixel(GetNrOfDataPoints());
}

float ScanMatrix::AverageDataPointsPerPixel(size_t const nrOfPoints) const
{
    size_t const nrOfPixels { m_scanPixels.NrOfPoints() };
    assert(nrOfPixels > 0);
    return Cast2Float(nrOfPoints) / Cast2Float(nrOfPixels);
}

size_t ScanMatrix::MaxNrOfDataPoints() const
{
    size_t maxNrOfDataPoints { 0 };
    Apply2AllScanPixelsC
    (
        [this, &maxNrOfDataPoints](ScanPixel const& p)
        {
            if (p.GetNrOfDataPoints() > maxNrOfDataPoints)
                maxNrOfDataPoints = p.GetNrOfDataPoints();
        }
    );
    return maxNrOfDataPoints;
}

float ScanMatrix::DataPointVariance()
{
    float  fCenter           { AverageDataPointsPerPixel() };
    float  fVariance         { 0.0f };
    size_t maxNrOfDataPoints { 0 };
    m_pScanPixelMax = nullptr;
    Apply2AllScanPixelsC
    (
        [this, fCenter, &fVariance, &maxNrOfDataPoints](ScanPixel const& p)
        {
            if (p.GetNrOfDataPoints() > maxNrOfDataPoints)
            {
                maxNrOfDataPoints = p.GetNrOfDataPoints();
                m_pScanPixelMax   = &p;
            }
            float const fDiff { Cast2Float(p.GetNrOfDataPoints()) - fCenter };
            fVariance += fDiff * fDiff;
        }
    );
    return fVariance;
}

void ScanMatrix::addScanDataPoint
(
    Pipe        const& pipe,
    Pipe::SegNr const  segNr,
    RasterPoint const& rPnt
)
{
    if (ScanPixel * pScanPoint { m_scanPixels.GetPtr(rPnt) })
        pScanPoint->Add(ScanDataPoint(pipe, segNr));
}
