// NNetScan.ixx
//
// NNetScan

module;

#include <memory>

export module NNetScan;

import SaveCast;
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

    size_t GetNrOfSensorPoints() const
    {
        size_t nr { 0 };
        Apply2AllScanPixelsC([&nr](ScanPixel const& p) { nr += p.GetNrOfDataPoints(); });
        return nr;
    }

    float DataPointsPerPixel() const
    {
        size_t nrOfPoints { GetNrOfSensorPoints() };
        size_t nrOfPixels { m_pNMWI->GetScanRaster().NrOfPoints() };
        return Cast2Float(nrOfPoints) / Cast2Float(nrOfPixels);
    }

    float DataPointVariance()
    {
        float fCenter   { DataPointsPerPixel() };
        float fVariance { 0.0f };
        Apply2AllScanPixelsC
        (
            [fCenter, &fVariance](ScanPixel const& p)
            { 
                float const fDiff { Cast2Float(p.GetNrOfDataPoints()) - fCenter };
                fVariance += fDiff * fDiff;
            }
        );
        return fVariance;
    }

private:

    mV getMaxVoltage() const;

    void add2list(Pipe const&, Pipe::SegNr const, RasterPoint const&);
    void add2list(Pipe const&, Raster const&);

    unique_ptr<Image>         m_upImage;
    NNetModelWriterInterface* m_pNMWI { nullptr };
};