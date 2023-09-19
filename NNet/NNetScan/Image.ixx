// Image.ixx
//
// NNetScan

module;

#include <memory>
#include <vector>

export module NNetScan:Image;

import Raster;
import :ScanDataPoint;
import :ScanLine;

using std::vector;

class Image
{
public:

    Image(RasterPoint const& size)
    {
        m_scanLines.reserve(size.GetY());
        for (int i = 0; i < size.GetY(); ++i)
            m_scanLines.push_back(ScanLine(size.GetX()));
    }

    Image& operator*= (float const factor)
    {
        Apply2AllScanPixels([factor](ScanPixel& sp) { sp *= factor; });
        return *this;
    }

    ScanLine& GetScanLine(RasterIndex const ry)
    {
        return m_scanLines.at(ry);
    };

    void Apply2AllScanPointsC(auto const& func) const
    {
        for (ScanLine const& scanLine : m_scanLines)
            scanLine.Apply2AllScanPointsC(func);
    }

    void Apply2AllScanPoints(auto const& func)
    {
        for (ScanLine& scanLine : m_scanLines)
            scanLine.Apply2AllScanPoints(func);
    }

    void Apply2AllScanPixels(auto const& func)
    {
        for (ScanLine &scanLine : m_scanLines)
            scanLine.Apply2AllScanPixels(func);
    }

    void Apply2AllScanPixelsC(auto const& func) const
    {
        for (ScanLine const& scanLine : m_scanLines)
            scanLine.Apply2AllScanPixelsC(func);
    }

private:
    vector<ScanLine> m_scanLines;
};
