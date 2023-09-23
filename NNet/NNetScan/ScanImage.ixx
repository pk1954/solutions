// ScanImage.ixx
//
// NNetScan

module;

#include <memory>
#include <vector>

export module NNetScan:ScanImage;

import Raster;
import :ScanDataPoint;
import :ScanLine;

using std::vector;
using std::unique_ptr;
using std::make_unique;

export using ImageLine = vector<mV>;

export class ScanImage
{
public:

    ScanImage(RasterPoint const& size)
    {
        m_imageLines.reserve(size.m_y);
        for (int i = 0; i < size.m_y; ++i)
            m_imageLines.push_back(make_unique<ImageLine>(size.m_x));
    }

    ImageLine& GetLine(RasterIndex const ry)
    {
        return *m_imageLines.at(ry).get();
    }

    ImageLine const& GetLine(RasterIndex const ry) const
    {
        return *m_imageLines.at(ry).get();
    }

    void Set(RasterPoint const& rp, mV voltage)
    {
        GetLine(rp.m_y).at(rp.m_x) = voltage;
    }

    mV Get(RasterPoint const& rp) const
    {
        return GetLine(rp.m_y).at(rp.m_x);
    }

    ScanImage& operator*= (float const factor)
    {
        Apply2AllPixels([factor](mV &voltage) { voltage *= factor; });
        return *this;
    }

    void Apply2AllPixels(auto const& func)
    {
        for (auto &upLine : m_imageLines)
            for (mV & voltage : *upLine.get())
                func(voltage);
    }

    void Apply2AllPixelsC(auto const& func) const
    {
        for (auto const& upLine : m_imageLines)
            for (mV const& voltage : *upLine.get())
                func(voltage);
    }

    mV GetMaxVoltage() const
    {
        mV mVmax { 0.0_mV };
        Apply2AllPixelsC
        (
            [&mVmax](mV const mv)
            {
                if (mv > mVmax)
                    mVmax = mv;
            }
        );
        return mVmax;
    }

private:
    vector<unique_ptr<ImageLine>> m_imageLines;
};
