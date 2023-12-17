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

ScanMatrix::ScanMatrix(RasterPoint const& size)
    :m_scanLines(size)
{}

RasterPoint ScanMatrix::Size() const
{
    return m_scanLines.GetSize();
}

mV ScanMatrix::Scan(RasterPoint const& rp)
{
    return m_scanLines.GetRef(rp).Scan();
}

ScanPixel const& ScanMatrix::GetScanPixel(RasterPoint const& rp) const
{
    return m_scanLines.GetConstRef(rp);
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
    nmri.Apply2AllC<Pipe>
    (
        [this, nmri](Pipe const& p)
        {
            Add2list(p, nmri.GetScanRaster());
        }
    );
}

size_t ScanMatrix::GetNrOfSensorPoints() const
{
    size_t nr { 0 };
    Apply2AllScanPixelsC([&nr](ScanPixel const& p) { nr += p.GetNrOfDataPoints(); });
    return nr;
}

float ScanMatrix::AverageDataPointsPerPixel() const
{
    size_t nrOfPoints { GetNrOfSensorPoints() };
    size_t nrOfPixels { m_scanLines.NrOfPoints() };
    assert(nrOfPixels > 0);
    return Cast2Float(nrOfPoints) / Cast2Float(nrOfPixels);
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
    ScanPixel& scanPoint { m_scanLines.GetRef(rPnt) };
    scanPoint.Add(ScanDataPoint(pipe, segNr));
}
