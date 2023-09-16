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

struct SigDataPoint
{
    Pipe        const* m_pPipe { nullptr };
    Pipe::SegNr const  m_segNr;

    mV GetSignalValue() const
    {
        return m_pPipe->GetVoltage(m_segNr);
    }
};

class ScanPoint
{
public:
    void scan()
    {
        mVsum = 0.0_mV;
        for (SigDataPoint const& pnt : list)
            mVsum += pnt.GetSignalValue();
    }

    void add(SigDataPoint const& dataPoint)
    {
        list.push_back(dataPoint);
    }

private:
    vector<SigDataPoint> list {};
    mV                   mVsum;
};

struct ScanLine
{
    ScanLine(RasterIndex const nrCols)
    {
        m_scanPoints.resize(nrCols);
    }

    vector<ScanPoint> m_scanPoints;
}; 

struct Image
{
    Image(RasterPoint const& size)
    {
        m_scanLines.reserve(size.GetY());
        for (int i = 0; i < size.GetY(); ++i)
        {
            m_scanLines.push_back(ScanLine(size.GetX()));
        }
    }

    vector<ScanLine> m_scanLines;
};

export class Scan
{
public:
    void PrepareScan(NNetModelWriterInterface&);
    void StartScan  (NNetModelWriterInterface&);

private:

    void add2list(Pipe const&, Pipe::SegNr const, RasterPoint const&);
    void add2list(Pipe const&, Raster const&);

    unique_ptr<Image> m_upImage;
};