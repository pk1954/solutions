// NNetScan.ixx
//
// NNetScan

module;

#include <memory>

export module NNetScan;

import SaveCast;
import Raster;
import NNetModel;

export import :ScanImage;
export import :ScanMatrix;

import :ScanLine;
import :ScanPixel;
import :ScanDataPoint;

using std::unique_ptr;

export class NNetScan
{
public:
    unique_ptr<ScanMatrix> PrepareScan(NNetModelWriterInterface&);

    size_t GetNrOfSensorPoints(RasterPoint const&) const;
    
    ScanPixel const& GetScanPixel(RasterPoint const&) const;

    unique_ptr<ScanImage> Scan(NNetModelWriterInterface&, ScanMatrix&);

private:

    mV getMaxVoltage(ScanMatrix const&) const;

    void add2list(Pipe const&, ScanMatrix &);
};