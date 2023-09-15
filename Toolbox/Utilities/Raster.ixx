// Raster.ixx 
//
// ToolBox\Utilities

module;

#include <optional>

export module Raster;

import Types;

using std::optional;

export using RasterIndex = long;
export using RasterPoint = PosType<RasterIndex>;

export class Raster
{
public:

void SetResolution (MicroMeter     const  res)    { m_resolution = res; }
void SetCenter     (MicroMeterPnt  const& pnt)    { m_rect.SetCenter(pnt); }
void MoveRasterRect(MicroMeterPnt  const& delta)  { m_rect.Move(delta); }
void SetRasterRect (MicroMeterRect const& umRect) { m_rect = umRect; }
void SetRasterRect (CardPoint const, MicroMeterPnt const&);

RasterPoint    Size()          const;
MicroMeter     Resolution()    const { return m_resolution; }
MicroMeterRect GetRasterRect() const { return m_rect; }

optional<RasterPoint> FindRasterPos(MicroMeterPnt const) const;

private:

    MicroMeter     m_resolution { 100.0_MicroMeter };
    MicroMeterRect m_rect       { -1000._MicroMeter, -1000._MicroMeter, 1000._MicroMeter, 1000._MicroMeter, };

    RasterPoint round2RasterPoint(MicroMeterPnt const&) const;
};
