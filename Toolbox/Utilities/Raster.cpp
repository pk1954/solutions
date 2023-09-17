// Raster.cpp
//
// ToolBox\Utilities

module;

#include <cmath>
#include <optional>

module Raster;

import SaveCast;
import Types;

using std::roundf;
using std::optional;

optional<RasterPoint> Raster::FindRasterPos(MicroMeterPnt const umPnt) const
{
    if (m_rect.Includes(umPnt))
    {
        MicroMeterPnt const umOrigin  { m_rect.GetStartPoint() };
        MicroMeterPnt const umOff     { umPnt - umOrigin };
        RasterPoint   const rasterPos { round2RasterPoint(umOff) };
        return rasterPos;
    }
    return {};
}

void Raster::SetRasterRect
(
    CardPoint     const  cardPnt,
    MicroMeterPnt const& pos
)
{
    m_rect.Manipulate(cardPnt, pos);
}

RasterPoint Raster::Size() const
{
    return round2RasterPoint(MicroMeterPnt(m_rect.GetWidth(), m_rect.GetHeight()));
}

RasterPoint Raster::round2RasterPoint(MicroMeterPnt const& pntParam) const
{
    MicroMeterPnt pnt { pntParam };
    pnt /= m_resolution.GetValue();
    return RasterPoint(Cast2Int(pnt.GetXvalue()), Cast2Int(pnt.GetYvalue()));
}
