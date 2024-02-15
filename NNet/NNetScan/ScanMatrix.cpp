// ScanMatrix.cpp
//
// NNetScan

module;

#include <cassert>
#include <vector>
#include <optional>
#include <Windows.h>

module ScanMatrix;

import Types;
import Raster;
import SaveCast;
import ScanPixel;
import NNetModel;

using std::vector;
using std::optional;

ScanMatrix::ScanMatrix()
{
    InitializeCriticalSection(&m_cs);
}

ScanMatrix::ScanMatrix(RasterPoint const& rpSize)
    :m_scanPixels(rpSize)
{
    InitializeCriticalSection(&m_cs);
}

void ScanMatrix::SetModelInterface(NNetModelReaderInterface* const p)
{
    m_pNMRI = p;
    Notify(false);
}

void ScanMatrix::Prepare()
{
    EnterCriticalSection(&m_cs);
    if (m_bDirty)
    {
        RasterPoint rpSize { m_pNMRI->GetScanAreaSize() };
        if (rpSize != Size())
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
