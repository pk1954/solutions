// ScanMatrix.ixx
//
// NNetScan

module;

#include <memory>
#include <vector>
#include <Windows.h>

export module ScanMatrix;

import Types;
import ObserverInterface;
import SaveCast;
import ScanPixel;
import NNetModel;
import Vector2D;

export import Raster;

using std::unique_ptr;
using std::vector;

export class ScanMatrix: public ObserverInterface
{
public:

    ScanMatrix();
    ScanMatrix(RasterPoint const&);

    void SetModelInterface(NNetModelReaderInterface* const);

    void Notify(bool const) final { m_bDirty = true; }

    void Prepare();
    void Clear();

    RasterPoint Size  () const { return m_scanPixels.GetSize(); }
    RasterIndex Width () const { return m_scanPixels.Width(); }
    RasterIndex Height() const { return m_scanPixels.Height(); }

    mV Scan(RasterPoint const&);

    bool IsValid(RasterPoint const& pnt) const { return m_scanPixels.IsValid(pnt); }

    ScanPixel const &GetScanPixel(RasterPoint const&) const;

    size_t NrOfDataPntsInPixel(RasterPoint const&) const;
    size_t NrOfDataPntsInMatrix()                  const;
    size_t MaxNrOfDataPoints()                     const { return m_maxNrOfDataPnts; }
    float  AverageDataPointsPerPixel()             const;
    float  DivideByArea(size_t const)              const;
    float  DataPointVariance();

    void Apply2AllScanPixels(auto const& func)
    {
        m_scanPixels.Apply2AllPixels(func);
    }

    void Apply2AllScanPixelsC(auto const& func) const
    {
        m_scanPixels.Apply2AllPixelsC(func);
    }

private:
    void add2list(Pipe const&);
    void findMaxNrOfDataPoints();

	NNetModelReaderInterface const* m_pNMRI { nullptr };

    bool                m_bDirty;
    size_t              m_maxNrOfDataPnts;
    Vector2D<ScanPixel> m_scanPixels;
    CRITICAL_SECTION    m_cs;
};
