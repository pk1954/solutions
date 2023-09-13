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
        MicroMeterPnt const umPos     { umOff * (1.0f / m_resolution.GetValue()) };
        RasterPoint   const rasterPos { round2RasterPoint(umPos) };
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
    adjust2Raster();
}

void Raster::SetRasterRect(MicroMeterRect const& umRect)
{
    m_rect = umRect;
    adjust2Raster();
}

RasterPoint Raster::Size() const
{
    return round2RasterPoint(MicroMeterPnt(m_rect.GetWidth(), m_rect.GetHeight()));
}

RasterPoint Raster::round2RasterPoint(MicroMeterPnt const& pntParam) const
{
    MicroMeterPnt pnt { pntParam.DivRound(m_resolution) };
    return RasterPoint(Cast2Int(pnt.GetXvalue()), Cast2Int(pnt.GetYvalue()));
}

void Raster::adjust2Raster()
{
    m_rect.SetSize(m_rect.GetSize().Round(m_resolution));
}
