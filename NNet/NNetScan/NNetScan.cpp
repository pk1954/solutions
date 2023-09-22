// Scan.cpp
//
// NNetScan

module;

#include <memory>
#include <vector>
#include <algorithm>
#include <optional>

module NNetScan;

import Types;
import Raster;
import Signals;
import NNetModel;
import :ScanDataPoint;
import :ScanMatrix;

using std::min;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::optional;

unique_ptr<ScanMatrix> NNetScan::PrepareScan(NNetModelWriterInterface& nmwi)
{
    Raster          const  raster       { nmwi.GetScanRaster() };
    RasterPoint     const  rasterSize   { raster.Size() };
    unique_ptr<ScanMatrix> upScanMatrix { make_unique<ScanMatrix>(rasterSize) };
    nmwi.Apply2AllC<Pipe>
    (
        [&upScanMatrix, &raster](Pipe const& pipe)
        {
            upScanMatrix->Add2list(pipe, raster);
        }
    );
    return upScanMatrix;
}

unique_ptr<Image> NNetScan::Scan
(
    NNetModelWriterInterface &nmwi,
    ScanMatrix               &scanMatrix
)
{
    RasterPoint const scanRasterSize { nmwi.GetScanAreaSize() };
    fMicroSecs        usScanTime     { SimulationTime::Get() };
    RasterPoint       imageSize      { scanMatrix.Size() };
    unique_ptr<Image> upImage        { make_unique<Image>(imageSize) };
    RasterPoint       rpRun;
    for (rpRun.m_y = 0; rpRun.m_y < imageSize.m_y; ++rpRun.m_y)
    {
        ScanLine const& scanLine  { scanMatrix.GetScanLineC(rpRun.m_y) };
        ImageLine     & imageLine { upImage->GetLine(rpRun.m_y) };
        for (rpRun.m_x = 0; rpRun.m_x < imageSize.m_x; ++rpRun.m_x)
        {
            usScanTime += nmwi.PixelScanTime();
            while (SimulationTime::Get() < usScanTime)
                nmwi.Compute();
            imageLine.at(rpRun.m_x) = scanLine.Scan(rpRun.m_x);
        }
    }
    return upImage;
}
