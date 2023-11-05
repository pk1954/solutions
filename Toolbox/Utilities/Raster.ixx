// Raster.ixx 
//
// ToolBox\Utilities

module;

#include <optional>

export module Raster;

import Types;
import SaveCast;

using std::optional;

export using RasterIndex = int;
export struct RasterPoint 
{
    RasterIndex m_x;
    RasterIndex m_y;
};

export class Raster
{
public:

void SetWidth      (RasterIndex    const  w)      { m_rect.SetWidth (m_resolution * Cast2Float(w)); }
void SetHeight     (RasterIndex    const  h)      { m_rect.SetHeight(m_resolution * Cast2Float(h)); }
void SetResolution (MicroMeter     const  res)    { m_resolution = res; }
void SetCenter     (MicroMeterPnt  const& pnt)    { m_rect.SetCenter(pnt); }
void MoveRasterRect(MicroMeterPnt  const& delta)  { m_rect.Move(delta); }
void SetRasterRect (MicroMeterRect const& umRect) { m_rect = umRect; }
void SetRasterRect (CardPoint const, MicroMeterPnt const&);

RasterIndex    RasterWidth () const { return round2Raster(m_rect.GetWidth ()); }
RasterIndex    RasterHeight() const { return round2Raster(m_rect.GetHeight()); }

RasterPoint    Size()          const { return RasterPoint(RasterWidth(), RasterHeight()); }
MicroMeter     Resolution()    const { return m_resolution; }
MicroMeterRect GetRasterRect() const { return m_rect; }
size_t         NrOfPoints()    const { return RasterWidth() * RasterHeight(); }

MicroMeterRect GetPointRect(RasterPoint const&) const;

optional<RasterPoint> FindRasterPos(MicroMeterPnt const) const;

private:

    MicroMeter     m_resolution { 100.0_MicroMeter };
    MicroMeterRect m_rect       { -1000._MicroMeter, -1000._MicroMeter, 1000._MicroMeter, 1000._MicroMeter, };

    RasterPoint round2RasterPoint(MicroMeterPnt const&) const;
    RasterIndex round2Raster     (MicroMeter    const ) const;
};
