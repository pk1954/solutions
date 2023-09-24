// NNetScan.ixx
//
// NNetScan

module;

#include <memory>

export module NNetScan;

import SaveCast;
import Raster;
import NNetModel;

export import :ScanMatrix;
export import :ScanLine;

import :ScanPixel;

using std::unique_ptr;

export class NNetScan
{
public:
    unique_ptr<ScanMatrix> PrepareScan(NNetModelWriterInterface&);

    size_t GetNrOfSensorPoints(RasterPoint const&) const;
    
    ScanPixel const& GetScanPixel(RasterPoint const&) const;

private:
};