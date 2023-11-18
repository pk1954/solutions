// Scan.ixx
//
// NNetScan

module;

#include <memory>
#include <vector>

export module ScanLine;

import Types;
import Raster;
import SaveCast;
import NNetModel;
import ScanPixel;

using std::unique_ptr;
using std::vector;

export class ScanLine
{
public:

    ScanLine(RasterIndex const nrCols)
    {
        m_scanPixels.resize(nrCols);
    }

    void Clear()
    {
        Apply2AllScanPixels([](auto& p) { p.Clear(); });
    }

    RasterIndex Size() const 
    { 
        return Cast2Int(m_scanPixels.size()); 
    }

    ScanPixel const& GetScanPixelC(RasterIndex const rx) const
    {
        return m_scanPixels.at(rx);
    }

    ScanPixel& GetScanPixel(RasterIndex const rx)
    {
        return m_scanPixels.at(rx);
    }

    mV Scan(RasterIndex const rpX)
    {
        return GetScanPixel(rpX).Scan();
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
