// ScanPixel.ixx
//
// NNetScan

module;

#include <memory>
#include <vector>

export module NNetScan:ScanPixel;

import Raster;
import NNetModel;
import :ScanDataPoint;

using std::unique_ptr;
using std::vector;

export class ScanPixel
{
public:
    void Scan()
    {
        m_mVsum = 0.0_mV;
        for (ScanDataPoint const& pnt : m_dataPoints)
            m_mVsum += pnt.GetSignalValue();
    }

    void Add(ScanDataPoint const& dataPoint)
    {
        m_dataPoints.push_back(dataPoint);
    }

    ScanPixel& operator*= (float const f)
    {
        m_mVsum *= f;
        return *this;
    }

    size_t GetNrOfDataPoints() const { return m_dataPoints.size(); }
    mV     GetVoltage()        const { return m_mVsum; }

    void Apply2AllScanPoints(auto const& func)
    {
        for (ScanDataPoint &dataPoint : m_dataPoints)
            func(dataPoint);
    }

    void Apply2AllScanPointsC(auto const& func) const
    {
        for (ScanDataPoint const &dataPoint : m_dataPoints)
            func(dataPoint);
    }

private:
    vector<ScanDataPoint> m_dataPoints {};
    mV                    m_mVsum;
};
