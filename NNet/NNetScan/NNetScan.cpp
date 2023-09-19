// Scan.cpp
//
// NNetScan

module;

#include <memory>
#include <vector>
#include <algorithm>
#include <optional>

module NNetScan;

import Signals;
import NNetModel;
import :ScanDataPoint;

using std::min;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::optional;

void NNetScan::add2list
(
    Pipe        const& pipe,
    Pipe::SegNr const  segNr,
    RasterPoint const& rPnt
)
{
    ScanLine  &scanLine  { m_upImage->GetScanLine(rPnt.GetY()) };
    ScanPixel &scanPoint { scanLine.GetScanPixel(rPnt.GetX()) };
    scanPoint.Add(ScanDataPoint(pipe, segNr));
}

void NNetScan::add2list
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
                add2list(pipe, segNr, rPntOpt.value());
        }
    );
}

void NNetScan::PrepareScan(NNetModelWriterInterface& nmwi)
{
    Raster const &raster { nmwi.GetScanRaster() };
    m_upImage = make_unique<Image>(raster.Size());
    nmwi.Apply2AllC<Pipe>
    (
        [this, &raster](Pipe const& pipe)
        {
            add2list(pipe, raster);
        }
    );
}

ScanPixel const& NNetScan::GetScanPixel(RasterPoint const& rp) const
{
    return m_upImage->GetScanLine(rp.GetY()).GetScanPixel(rp.GetX());
}

size_t NNetScan::GetNrOfSensorPoints(RasterPoint const& rp) const
{
    return GetScanPixel(rp).GetNrOfDataPoints();
}

void NNetScan::StartScan(NNetModelWriterInterface &nmwi)
{
    RasterPoint const scanRasterSize { nmwi.GetScanAreaSize() };
    fMicroSecs        usScanTime     { SimulationTime::Get() };
    int iNrOfScans = 0;
    int iNrOfComputations = 0;
    m_upImage->Apply2AllScanPixels
    (
        [this, &usScanTime, &nmwi, &iNrOfComputations, &iNrOfScans](ScanPixel& scanPixel)
        {
            usScanTime += nmwi.PixelScanTime();
            for (;;)
            {
                fMicroSecs usSimulationTime { SimulationTime::Get() };
                if (usSimulationTime >= usScanTime)
                    break;
                nmwi.Compute();
                ++iNrOfComputations;
            }
            scanPixel.Scan();
            ++iNrOfScans;
        }
    );
}

mV NNetScan::getMaxVoltage() const
{
    mV mVmax { 0.0_mV };
    m_upImage->Apply2AllScanPixelsC
    (
        [&mVmax](ScanPixel const& scanPoint)
        {
            mV mVscanPoint { scanPoint.GetVoltage() };
            if (mVscanPoint > mVmax)
                mVmax = mVscanPoint;
        }
    );
    return mVmax;
}
