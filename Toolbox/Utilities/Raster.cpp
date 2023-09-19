// Raster.cpp
//
// ToolBox\Utilities

module;

#include <cmath>
#include <cassert>
#include <optional>

module Raster;

import SaveCast;
import Types;

using std::roundf;
using std::optional;

optional<RasterPoint> Raster::FindRasterPos(MicroMeterPnt const umPnt) const
{
    MicroMeterPnt const umOrigin  { m_rect.GetStartPoint() };
    MicroMeterPnt const umOff     { umPnt - umOrigin };
    RasterPoint   const rasterPos { round2RasterPoint(umOff) };
    if (rasterPos.GetX() < 0)
        return {};
    if (rasterPos.GetY() < 0)
        return {};
    if (rasterPos.GetX() >= round2Raster(m_rect.GetWidth()))
        return {};
    if (rasterPos.GetY() >= round2Raster(m_rect.GetHeight()))
        return {};
    return rasterPos;
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
    RasterPoint rpnt 
    { 
        round2Raster(m_rect.GetWidth()), 
        round2Raster(m_rect.GetHeight()) 
    };
    return rpnt;
}

RasterIndex Raster::round2Raster(MicroMeter umVal) const
{
    umVal /= m_resolution.GetValue();
    RasterIndex rIndex { Cast2Int(umVal.GetValue()) };
    return rIndex;
}

RasterPoint Raster::round2RasterPoint(MicroMeterPnt const& pntParam) const
{
    RasterPoint rpnt { round2Raster(pntParam.GetX()), round2Raster(pntParam.GetY()) };
    return rpnt;
}
