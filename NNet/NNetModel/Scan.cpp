// Scan.cpp
//
// NNetModel

module;

#include <memory>
#include <vector>
#include <algorithm>
#include <optional>

module NNetModel:Scan;

using std::min;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::optional;

void Scan::add2list
(
    Pipe        const& pipe,
    Pipe::SegNr const  segNr,
    RasterPoint const& rPnt
)
{
    SigDataPoint const  dataPoint(&pipe, segNr);
    RasterIndex  const  rx { rPnt.GetX() };
    RasterIndex  const  ry { rPnt.GetY() };
    ScanLine  &scanLine  { m_upImage->m_scanLines.at(ry) };
    ScanPoint &scanPoint { scanLine.m_scanPoints.at(rx) };
    scanPoint.add(dataPoint);
}

void Scan::add2list
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

void Scan::PrepareScan(NNetModelWriterInterface& nmwi)
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

void Scan::StartScan(NNetModelWriterInterface &nmwi)
{
    RasterPoint const scanRasterSize { nmwi.GetScanAreaSize() };
    fMicroSecs        usScanTime     { 0.0_MicroSecs };
    for (RasterIndex ry = 0; ry < scanRasterSize.GetY(); ++ry)
    {
        ScanLine &scanLine { m_upImage->m_scanLines.at(ry) };
        for (RasterIndex rx = 0; rx < scanRasterSize.GetX(); ++rx)
        {
            ScanPoint &scanPoint { scanLine.m_scanPoints.at(rx) };
            usScanTime += nmwi.PixelScanTime();
            while (SimulationTime::Get() < usScanTime)
                nmwi.Compute();
            scanPoint.scan();
        }
    }
}
