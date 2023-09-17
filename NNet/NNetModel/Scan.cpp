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
    ScanLine  &scanLine  { m_upImage->GetScanLine(rPnt.GetY()) };
    ScanPoint &scanPoint { scanLine.GetScanPoint(rPnt.GetX()) };
    scanPoint.Add(SigDataPoint(pipe, segNr));
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
    m_pNMWI = &nmwi;
    Raster const &raster { m_pNMWI->GetScanRaster() };
    m_upImage = make_unique<Image>(raster.Size());
    m_pNMWI->Apply2AllC<Pipe>
    (
        [this, &raster](Pipe const& pipe)
        {
            add2list(pipe, raster);
        }
    );
}

size_t Scan::GetNrOfSensorPoints(RasterPoint const& rp) const
{
    return m_upImage->GetScanLine(rp.GetY()).GetScanPoint(rp.GetX()).GetNrOfSignalPoints();
}

void Scan::StartScan(NNetModelWriterInterface &nmwi)
{
    RasterPoint const scanRasterSize  { m_pNMWI->GetScanAreaSize() };
    fMicroSecs  const usPixelScanTime { m_pNMWI->PixelScanTime() };
    fMicroSecs        usScanTime      { 0.0_MicroSecs };
    Apply2AllScanPoints
    (
        [this, &usScanTime, usPixelScanTime](ScanPoint& scanPoint)
        {
            usScanTime += usPixelScanTime;
            while (SimulationTime::Get() < usScanTime)
                m_pNMWI->Compute();
            scanPoint.Scan();
        }
    );
}

Image::Image(RasterPoint const& size)
{
    m_scanLines.reserve(size.GetY());
    for (int i = 0; i < size.GetY(); ++i)
        m_scanLines.push_back(ScanLine(size.GetX()));
}

ScanLine& Image::GetScanLine(RasterIndex const ry)
{
    return m_scanLines.at(ry);
};

ScanLine::ScanLine(RasterIndex const nrCols)
{
    m_scanPoints.resize(nrCols);
}

ScanPoint& ScanLine::GetScanPoint(RasterIndex const rx)
{
    return m_scanPoints.at(rx);
}

void ScanPoint::Scan()
{
    mVsum = 0.0_mV;
    for (SigDataPoint const& pnt : list)
        mVsum += pnt.GetSignalValue();
}

void ScanPoint::Add(SigDataPoint const& dataPoint)
{
    list.push_back(dataPoint);
}

SigDataPoint::SigDataPoint(Pipe const& pipe, Pipe::SegNr const segNr)
  : m_pPipe(&pipe),
    m_segNr(segNr)
{}

mV SigDataPoint::GetSignalValue() const
{
    return m_pPipe->GetVoltage(m_segNr);
}
