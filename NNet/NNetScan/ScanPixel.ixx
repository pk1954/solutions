// ScanPixel.ixx
//
// NNetScan

module;

#include <vector>

export module NNetScan:ScanPixel;

import Raster;
import NNetModel;
import :ScanDataPoint;

using std::vector;

export class ScanPixel
{
public:
    mV Scan() const
    {
        mV mVsum = 0.0_mV;
        for (ScanDataPoint const& pnt : m_dataPoints)
            mVsum += pnt.GetSignalValue();
        return mVsum;
    }

    //mV GetVoltage() const
    //{
    //    return m_mVsum;
    //}

    void Add(ScanDataPoint const& dataPoint)
    {
        m_dataPoints.push_back(dataPoint);
    }

    size_t GetNrOfDataPoints() const 
    { 
        return m_dataPoints.size(); 
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
    //mV                    m_mVsum;
};
