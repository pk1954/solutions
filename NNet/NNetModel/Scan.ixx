// Scan.ixx
//
// NNetModel

module;

#include <memory>
#include <vector>

export module NNetModel:Scan;

import Raster;
import :NNetModelWriterInterface;
import :Pipe;

using std::unique_ptr;
using std::vector;

class SigDataPoint
{
public:
    SigDataPoint(Pipe const&, Pipe::SegNr const);
    mV GetSignalValue() const;

private:
    Pipe        const* m_pPipe { nullptr };
    Pipe::SegNr const  m_segNr;
};

export class ScanPoint
{
public:
    void   Scan();
    void   Add(SigDataPoint const&);
    void   Normalize(float const f) { mVsum *= f; }

    size_t GetNrOfSignalPoints() const { return list.size(); }
    mV     GetVoltage()          const { return mVsum; }

private:
    vector<SigDataPoint> list {};
    mV                   mVsum;
};

class ScanLine
{
public:
    ScanLine(RasterIndex const);

    ScanPoint& GetScanPoint(RasterIndex const);

    void Apply2AllScanPointsC(auto const& func) const
    {
        for (ScanPoint const& scanPoint : m_scanPoints)
            func(scanPoint);
    }

    void Apply2AllScanPoints(auto const& func)
    {
        for (ScanPoint & scanPoint : m_scanPoints)
            func(scanPoint);
    }

private:
    vector<ScanPoint> m_scanPoints;
}; 

class Image
{
public:
    Image(RasterPoint const&);

    ScanLine& GetScanLine(RasterIndex const);

    void Apply2AllScanPointsC(auto const& func) const
    {
        for (ScanLine const& scanLine : m_scanLines)
            scanLine.Apply2AllScanPointsC(func);
    }

    void Apply2AllScanPoints(auto const& func)
    {
        for (ScanLine & scanLine : m_scanLines)
            scanLine.Apply2AllScanPoints(func);
    }

private:
    vector<ScanLine> m_scanLines;
};

export class Scan
{
public:
    void PrepareScan(NNetModelWriterInterface&);

    size_t GetNrOfSensorPoints(RasterPoint const&) const;

    void StartScan(NNetModelWriterInterface&);

    void Apply2AllScanPointsC(auto const& func) const
    {
        m_upImage->Apply2AllScanPointsC(func);
    }

    void Apply2AllScanPoints(auto const& func)
    {
        m_upImage->Apply2AllScanPoints(func);
    }

private:

    void add2list(Pipe const&, Pipe::SegNr const, RasterPoint const&);
    void add2list(Pipe const&, Raster const&);

    unique_ptr<Image>         m_upImage;
    NNetModelWriterInterface* m_pNMWI { nullptr };
};