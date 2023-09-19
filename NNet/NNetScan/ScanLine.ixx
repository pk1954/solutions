// Scan.ixx
//
// NNetScan

module;

#include <memory>
#include <vector>

export module NNetScan:ScanLine;

import Raster;
import NNetModel;
import :ScanDataPoint;
import :ScanPixel;

using std::unique_ptr;
using std::vector;

class ScanLine
{
public:

    ScanLine(RasterIndex const nrCols)
    {
        m_scanPixels.resize(nrCols);
    }

    ScanPixel& GetScanPixel(RasterIndex const rx)
    {
        return m_scanPixels.at(rx);
    }

    void Apply2AllScanPointsC(auto const& func) const
    {
        for (ScanPixel const& scanPixel : m_scanPixels)
            scanPixel.Apply2AllScanPointsC(func);
    }

    void Apply2AllScanPoints(auto const& func)
    {
        for (ScanPixel& scanPixel : m_scanPixels)
            scanPixel.Apply2AllScanPoints(func);
    }

    void Apply2AllScanPixels(auto const& func)
    {
        for (ScanPixel &scanPixel : m_scanPixels)
            func(scanPixel);
    }

    void Apply2AllScanPixelsC(auto const& func) const
    {
        for (ScanPixel const& scanPixel : m_scanPixels)
            func(scanPixel);
    }

private:
    vector<ScanPixel> m_scanPixels;
};
