// NNetScan.ixx
//
// NNetScan

module;

#include <memory>

export module NNetScan;

import Raster;
import NNetModel;

import :Image;
import :ScanLine;
import :ScanDataPoint;
import :ScanPixel;

using std::unique_ptr;

export class NNetScan
{
public:
    void PrepareScan(NNetModelWriterInterface&);

    size_t GetNrOfSensorPoints(RasterPoint const&) const;
    
    ScanPixel const& GetScanPixel(RasterPoint const&) const;

    void StartScan(NNetModelWriterInterface&);

    void Apply2AllScanPointsC(auto const& func) const
    {
        m_upImage->Apply2AllScanPointsC(func);
    }

    void Apply2AllScanPoints(auto const& func)
    {
        m_upImage->Apply2AllScanPoints(func);
    }

    void Apply2AllScanPixelsC(auto const& func) const
    {
        m_upImage->Apply2AllScanPixelsC(func);
    }

    size_t GetNrOfSensorPoints()
    {
        size_t nr { 0 };
        Apply2AllScanPixelsC([&nr](ScanPixel const& p) { nr += p.GetNrOfDataPoints(); });
        return nr;
    }

private:

    mV getMaxVoltage() const;

    void add2list(Pipe const&, Pipe::SegNr const, RasterPoint const&);
    void add2list(Pipe const&, Raster const&);

    unique_ptr<Image>         m_upImage;
    NNetModelWriterInterface* m_pNMWI { nullptr };
};