// ScanMatrix.ixx
//
// NNetModel

export module NNetModel:ScanMatrix;

import std;
import Types;
import SaveCast;
import Vector2D;
import Uniform2D;
import ColorLUT;
import D2D_ColorLUT;
import DrawContext;
import :ScanPixel;
import :UPNobList;

export import Raster;

using std::unique_ptr;
using std::vector;
using std::optional;
using std::nullopt;

export using RawImage = Vector2D<mV>;

export class ScanMatrix
{
public:
    void Prepare(Raster const&, UPNobList const&);
    void DrawScanAreaBackground(DrawContext const&, Raster const&) const;
    void DrawScanImage         (DrawContext const&, Raster const&, RawImage const&, mV const, D2D_ColorLUT const&) const;
    void DrawScanAreaHandles   (DrawContext const&, Raster const&, optional<CardPoint> const) const;
    void DrawScanRaster        (DrawContext const&, Raster const&) const;
    void DrawSensorDensityMap  (DrawContext const&, Raster const&, UPNobList const&) const;
    void DrawScanProgress      (DrawContext const&,	Raster const&, RasterPoint const&) const;

    optional<CardPoint> SelectScanAreaHandle(DrawContext const&, Raster const&, MicroMeterPnt const&) const;

    void Clear() { m_scanPixels.Apply2AllPixels([](auto &p) { p.Clear(); }); }

    RasterPoint      Size  ()                                   const { return m_scanPixels.Size(); }
    RasterIndex      Width ()                                   const { return m_scanPixels.Width(); }
    RasterIndex      Height()                                   const { return m_scanPixels.Height(); }
    float            AspectRatio()                              const { return m_scanPixels.AspectRatio(); }
    bool             IsValid            (RasterPoint const& rp) const { return m_scanPixels.IsValid(rp); }
    ScanPixel const& GetScanPixel       (RasterPoint const& rp) const { return *m_scanPixels.GetConstPtr(rp); }
    mV               Scan               (RasterPoint const& rp) const { return GetScanPixel(rp).Scan(); }
    size_t           NrOfDataPntsInPixel(RasterPoint const& rp) const { return GetScanPixel(rp).NrOfDataPnts(); }

    size_t NrOfDataPntsInMatrix()      const;
    size_t MaxNrOfDataPoints()         const { return m_maxNrOfDataPnts; }
    float  AverageDataPointsPerPixel() const { return DivideByArea(NrOfDataPntsInMatrix()); }
    float  DivideByArea(size_t const)  const;
    float  DataPointVariance();

    void DensityCorrection(RawImage &) const;

private:
    void           add2list(Pipe const&, Raster const&);
    void           findMaxNrOfDataPoints();
    ColorLUT       sensorDensityLUT() const;
    Color          handleColor          (CardPoint const, optional<CardPoint> const) const;
    MicroMeter     getScanAreaHandleSize(Uniform2D<MicroMeter> const &) const;
    MicroMeterRect getRectHandle        (MicroMeter const, MicroMeterRect const, CardPoint const) const;

    size_t              m_maxNrOfDataPnts;
    Vector2D<ScanPixel> m_scanPixels;
};
