// ScanMatrix.ixx
//
// NNetScan

module;

#include <cassert>
#include <vector>
#include <optional>

export module NNetScan:ScanMatrix;

import Types;
import Raster;
import SaveCast;
import :ScanLine;

using std::vector;
using std::optional;

export class ScanMatrix
{
public:

    ScanMatrix(RasterPoint const& size)
    {
        m_scanLines.reserve(size.m_x);
        for (int i = 0; i < size.m_y; ++i)
            m_scanLines.push_back(ScanLine(size.m_x));
    }

    RasterPoint Size() const
    {
        return RasterPoint(m_scanLines.at(0).Size(), Cast2Int(m_scanLines.size()));
    }

    ScanLine const& GetScanLineC(RasterIndex const ry) const
    {
        return m_scanLines.at(ry);
    }

    ScanLine& GetScanLine(RasterIndex const ry)
    {
        return m_scanLines.at(ry);
    }

    ScanPixel const &GetScanPixel(RasterPoint const &rp) const
    {
        return GetScanLineC(rp.m_y).GetScanPixelC(rp.m_x);
    }

    void Add2list
    (
        Pipe   const &pipe,
        Raster const &raster
    )
    {
        pipe.Apply2AllSensorPoints
        (
            raster.Resolution(),
            [this, &raster](Pipe const& pipe, MicroMeterPnt const& umPnt, Pipe::SegNr const segNr)
            {
                if (optional<RasterPoint> const rPntOpt = raster.FindRasterPos(umPnt))
                    addScanDataPoint(pipe, segNr, rPntOpt.value());
            }
        );
    }

    void Apply2AllScanPixels(auto const& func)
    {
        for (ScanLine& scanLine : m_scanLines)
            scanLine.Apply2AllScanPixels(func);
    }

    void Apply2AllScanPixelsC(auto const& func) const
    {
        for (ScanLine const& scanLine : m_scanLines)
            scanLine.Apply2AllScanPixelsC(func);
    }

    size_t GetNrOfSensorPoints() const
    {
        size_t nr { 0 };
        Apply2AllScanPixelsC([&nr](ScanPixel const& p) { nr += p.GetNrOfDataPoints(); });
        return nr;
    }

    float AverageDataPointsPerPixel() const
    {
        assert(!m_scanLines.empty());
        size_t nrOfPoints { GetNrOfSensorPoints() };
        size_t nrOfPixels { m_scanLines.size() * m_scanLines[0].Size() };
        return Cast2Float(nrOfPoints) / Cast2Float(nrOfPixels);
    }

    float DataPointVariance()
    {
        float fCenter   { AverageDataPointsPerPixel() };
        float fVariance { 0.0f };
        Apply2AllScanPixelsC
        (
            [fCenter, &fVariance](ScanPixel const& p)
            {
                float const fDiff { Cast2Float(p.GetNrOfDataPoints()) - fCenter };
                fVariance += fDiff * fDiff;
            }
        );
        return fVariance;
    }

private:
    void addScanDataPoint
    (
        Pipe        const& pipe,
        Pipe::SegNr const  segNr,
        RasterPoint const& rPnt
    )
    {
        ScanLine & scanLine  { m_scanLines.at(rPnt.m_y) };
        ScanPixel& scanPoint { scanLine.GetScanPixel(rPnt.m_x) };
        scanPoint.Add(ScanDataPoint(pipe, segNr));
    }

    vector<ScanLine> m_scanLines;
};
