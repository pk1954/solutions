// ScanMatrix.ixx
//
// NNetScan

module;

#include <memory>
#include <vector>

export module ScanMatrix;

import Types;
import SaveCast;
import ScanPixel;
import NNetModel;
import Vector2D;

export import Raster;

using std::unique_ptr;
using std::vector;

export class ScanMatrix
{
public:

    ScanMatrix() {};
    ScanMatrix(RasterPoint const&);

    void Clear();
    void Fill(NNetModelReaderInterface const&);
    void Resize(RasterPoint const&);

    RasterPoint Size  () const { return m_scanPixels.GetSize(); }
    RasterIndex Width () const { return m_scanPixels.Width(); }
    RasterIndex Height() const { return m_scanPixels.Height(); }

    void Add2list(Pipe const&, Raster const&);

    mV Scan(RasterPoint const&);

    bool IsValid(RasterPoint const& pnt) const { return m_scanPixels.IsValid(pnt); }

    ScanPixel const &GetScanPixel(RasterPoint const&) const;

    size_t GetNrOfDataPoints(RasterPoint const& rp) const
    {
        return GetScanPixel(rp).GetNrOfDataPoints();
    }
        
    size_t GetNrOfDataPoints()                     const;
    size_t MaxNrOfDataPoints()                     const;
    float  AverageDataPointsPerPixel()             const;
    float  AverageDataPointsPerPixel(size_t const) const;
    float  DataPointVariance();

    void Apply2AllScanPixels(auto const& func)
    {
        m_scanPixels.Apply2AllPixels(func);
    }

    void Apply2AllScanPixelsC(auto const& func) const
    {
        m_scanPixels.Apply2AllPixelsC(func);
    }

    ScanPixel const* GetMaxScanPixel() { return m_pScanPixelMax; }

private:
    void addScanDataPoint(Pipe const&, Pipe::SegNr const, RasterPoint const&);

    Vector2D<ScanPixel> m_scanPixels;
    ScanPixel const   * m_pScanPixelMax { nullptr };
};
