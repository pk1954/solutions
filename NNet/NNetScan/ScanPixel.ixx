// ScanPixel.ixx
//
// NNetScan

module;

#include <vector>

export module ScanPixel;

import Types;
import Raster;
import NNetModel;

using std::vector;

export class ScanPixel
{
public:

    void Clear()
    {
        m_dataPoints.clear();
    }

    void Add(ScanDataPoint const& dataPoint)
    {
        m_dataPoints.push_back(dataPoint);
    }

    mV Scan()
    {
        mV mVsum = 0.0_mV;
        Apply2AllScanPoints([&mVsum](ScanDataPoint const& p) { mVsum += p.GetSignalValue(); });
        return mVsum;
    }

    size_t NrOfDataPnts() const 
    { 
        return m_dataPoints.size(); 
    }

    ScanDataPoint const& GetScanDataPoint(size_t const i) const
    {
        return m_dataPoints.at(i);
    }

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
};
