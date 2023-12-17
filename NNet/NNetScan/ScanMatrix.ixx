// ScanMatrix.ixx
//
// NNetScan

module;

#include <memory>
#include <vector>

export module ScanMatrix;

import Types;
import Raster;
import SaveCast;
import ScanPixel;
import NNetModel;

import Vector2D;

using std::unique_ptr;
using std::vector;

export class ScanMatrix
{
public:

    ScanMatrix(RasterPoint const&);

    void Clear();
    void Fill(NNetModelReaderInterface const&);

    RasterPoint Size() const;

    mV Scan(RasterPoint const&);

    ScanPixel const &GetScanPixel(RasterPoint const&) const;

    void   Add2list(Pipe const&, Raster const&);

    size_t GetNrOfSensorPoints()       const;
    float  AverageDataPointsPerPixel() const;
    float  DataPointVariance();

    void Apply2AllScanPixels(auto const& func)
    {
        m_scanLines.Apply2AllPixels(func);
    }

    void Apply2AllScanPixelsC(auto const& func) const
    {
        m_scanLines.Apply2AllPixelsC(func);
    }

    ScanPixel const* GetMaxScanPixel() { return m_pScanPixelMax; }

private:
    void addScanDataPoint(Pipe const&, Pipe::SegNr const, RasterPoint const&);

    Vector2D<ScanPixel> m_scanLines;
    ScanPixel const* m_pScanPixelMax { nullptr };
};
