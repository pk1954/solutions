// Raster.cpp
//
// ToolBox\Utilities

module Raster;

import std;
import SaveCast;
import Types;

using std::optional;

optional<RasterPoint> Raster::FindRasterPos(MicroMeterPnt const umPnt) const
{
    MicroMeterPnt const umOrigin  { m_rect.GetStartPoint() };
    MicroMeterPnt const umOff     { umPnt - umOrigin };
    RasterPoint   const rasterPos { round2RasterPoint(umOff) };
    if (rasterPos.m_x < 0)
        return {};
    if (rasterPos.m_y < 0)
        return {};
    if (rasterPos.m_x >= RasterWidth())
        return {};
    if (rasterPos.m_y >= RasterHeight())
        return {};
    return rasterPos;
}

MicroMeterRect Raster::GetPointRect(RasterPoint const& rPnt) const 
{ 
    MicroMeter const umLeft { m_rect.GetLeft() + m_resolution * Cast2Float(rPnt.m_x) };
    MicroMeter const umTop  { m_rect.GetTop () + m_resolution * Cast2Float(rPnt.m_y) };
    MicroMeterRect const rect(umLeft, umTop, umLeft + m_resolution, umTop + m_resolution);
    return rect;
}

MicroMeterRect Raster::GetRasterRect() const 
{ 
    MicroMeter const umRight  { m_rect.GetLeft() + m_resolution * Cast2Float(RasterWidth()) };
    MicroMeter const umBottom { m_rect.GetTop () + m_resolution * Cast2Float(RasterHeight()) };
    MicroMeterRect const rect(m_rect.GetLeft(), m_rect.GetTop (), umRight, umBottom);
    return rect;
}

void Raster::SetScanArea
(
    CardPoint     const  cardPnt,
    MicroMeterPnt const& pos
)
{
    m_rect.Manipulate(cardPnt, pos);
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
