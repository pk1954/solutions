// ScanMatrix.ixx
//
// NNetScan

module;

#include <vector>

export module ScanMatrix;

import Types;
import Raster;
import SaveCast;
import ScanLine;
import ScanPixel;
import NNetModel;

using std::vector;

export class ScanMatrix
{
public:

    ScanMatrix(RasterPoint const&);

    RasterPoint Size() const;

    mV Scan(RasterPoint const&);

    ScanLine        &GetScanLine (RasterIndex const );
    ScanLine  const &GetScanLineC(RasterIndex const ) const;
    ScanPixel const &GetScanPixel(RasterPoint const&) const;

    void   Add2list(Pipe const&, Raster const&);

    size_t GetNrOfSensorPoints()       const;
    float  AverageDataPointsPerPixel() const;
    float  DataPointVariance()         const;

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

private:
    void addScanDataPoint(Pipe const&, Pipe::SegNr const, RasterPoint const&);

    vector<ScanLine> m_scanLines;
};
