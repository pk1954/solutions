// Scan.ixx
//
// NNetModel

module;

#include <memory>
#include <vector>
#include <algorithm>
#include <optional>

export module NNetModel:Scan;

import Types;
import Raster;
import Signals;
import :NNetModelWriterInterface;
import :Model;

using std::min;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::optional;

struct SigDataPoint
{
    Pipe        const* m_pPipe;
    Pipe::SegNr const   m_segNr;

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

    void add(SigDataPoint const &dataPoint)
    {
        list.push_back(dataPoint);
    }

private:
    vector<SigDataPoint> list;
    mV                   mVsum;
};

using ScanLine = vector<unique_ptr<ScanPoint>>;
using Image = vector<unique_ptr<ScanLine>>;

export class Scan
{
public:
    void StartScan
    (
        NNetModelWriterInterface & nmwi,
        Raster              const& raster
    )
    {
        prepareScan(nmwi, raster);
        fMicroSecs usScanTime { 0.0_MicroSecs };
        Image    & image      { *m_upImage.get() };
        for (RasterIndex ry = 0; ry < raster.Size().GetY(); ++ry)
        {
            ScanLine& scanLine { *image[ry].get() };
            for (RasterIndex rx = 0; rx < raster.Size().GetX(); ++rx)
            {
                ScanPoint& scanPoint { *scanLine[rx] };
                usScanTime += m_usPerPixel;
                while (SimulationTime::Get() < usScanTime)
                    nmwi.Compute();
                scanPoint.scan();
            }
        }
    }

private:

    fMicroSecs m_usPerPixel;
    fMicroSecs m_usTotalScan;

    void add2list
    (
        Pipe   const &pipe,
        Raster const &raster
    )
    {
        float const  DATA_PNTS { 10.0f };
        float const  fIncCalc  { raster.Resolution() / (pipe.GetLength() * DATA_PNTS) };
        float const  fInc      { min(1.0f, fIncCalc) };
        Image const &image     { *m_upImage.get() };
        for (float fRun = 0.0f; fRun <= 1.0f; fRun += fInc)
        {
            MicroMeterPnt         const umpRun  { pipe.GetVector(fRun) };
            optional<RasterPoint> const rPntOpt { raster.FindRasterPos(umpRun) };
            if (rPntOpt)
            {
                SigDataPoint const  dataPoint(&pipe, pipe.GetSegNr(fRun));
                RasterPoint  const &rPnt      { rPntOpt.value() };
                RasterIndex  const  rx        { rPnt.GetX() };
                RasterIndex  const  ry        { rPnt.GetY() };
                ScanLine     const &scanLine  { *(image   [ry].get()) };
                ScanPoint          &scanPoint { *(scanLine[rx]) };
                scanPoint.add(dataPoint);
            }
        }
    }

    void prepareScan
    (
        NNetModelWriterInterface &nmwi,
        Raster             const &raster
    )
    {
        int iNrOfRows { raster.Size().GetY() };
        int iNrOfCols { raster.Size().GetX() };
        m_upImage = make_unique<Image>();
        m_upImage->reserve(iNrOfRows);
        for (int i = 0; i < iNrOfRows; ++i)
        {
            unique_ptr<ScanLine> upScanLine { make_unique<ScanLine>(iNrOfCols) };
            m_upImage->push_back(move(upScanLine));
        }
        nmwi.Apply2AllC<Pipe>
        (
            [this, &raster](Pipe const& pipe)
            { 
                add2list(pipe, raster);
            }
        );
    }

    unique_ptr<Image> m_upImage;
};