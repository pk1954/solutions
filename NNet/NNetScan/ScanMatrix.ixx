// ScanMatrix.ixx
//
// NNetScan

module;

#include <memory>
#include <vector>
#include <optional>
#include <Windows.h>

export module ScanMatrix;

import Types;
import ObserverInterface;
import SaveCast;
import ScanPixel;
import NNetModel;
import Vector2D;
import Uniform2D;
import ColorLUT;
import DrawContext;
import NNetPreferences;

export import Raster;

using std::unique_ptr;
using std::vector;
using std::optional;
using std::nullopt;

export class ScanMatrix: public ObserverInterface
{
public:

    ScanMatrix();

    void SetModelInterface(NNetModelReaderInterface* const);
    void PrepareScanMatrix();
    void Clear();
    mV   Scan(RasterPoint const&);
    bool SetScanAreaHandle(DrawContext const&, MicroMeterPnt const&);
    void DrawScanAreaBackground(DrawContext const&) const;
    void DrawScanArea(DrawContext const&, bool const);

    void Notify(bool const) final { m_bDirty = true; }

    RasterPoint Size  ()                        const { return m_scanPixels.GetSize(); }
    RasterIndex Width ()                        const { return m_scanPixels.Width(); }
    RasterIndex Height()                        const { return m_scanPixels.Height(); }
    bool        IsValid(RasterPoint const& pnt) const { return m_scanPixels.IsValid(pnt); }

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

    optional<CardPoint> GetHandleSelected() { return m_scanAreaHandleSelected; }

private:
    void       add2list(Pipe const&);
    void       findMaxNrOfDataPoints();
    ColorLUT   sensorDensityLUT() const;
    MicroMeter getScanAreaHandleSize(Uniform2D<MicroMeter> const &);
    void       drawSensorDensityMap(DrawContext const&);
    void       drawScanImage(DrawContext const&) const;
    void       drawScanAreaHandles(DrawContext const&);
    void       drawScanRaster(DrawContext const&);

	NNetModelReaderInterface const* m_pNMRI  { nullptr };
    bool                            m_bDirty { true };
    size_t                          m_maxNrOfDataPnts;
    Vector2D<ScanPixel>             m_scanPixels;
    CRITICAL_SECTION                m_cs;
	ColorLUT                        m_lut;
	optional<CardPoint>             m_scanAreaHandleSelected { nullopt };
};
