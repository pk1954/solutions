// Raster.cpp
//
// ToolBox\Utilities

module;

#include <cmath>
#include <optional>

module Raster;

import SaveCast;
import Types;

using std::round;
using std::optional;

optional<RasterPoint> Raster::FindRasterPos(MicroMeterPnt const umPnt) const
{
    MicroMeterPnt const umOrigin { Origin() };
    MicroMeterPnt const umOff    { umPnt - umOrigin };
    MicroMeterPnt const umPos    { umOff * (1.0f / m_resolution.GetValue()) };
    RasterPoint   const pos      { Cast2Int(round(umPos.GetXvalue())), Cast2Int(round(umPos.GetYvalue())) };
    if (
        (pos.GetX() < 0) || (pos.GetX() >= m_size.GetX()) ||
        (pos.GetY() < 0) || (pos.GetY() >= m_size.GetY())
       )
        return {};
    return pos;
}

MicroMeterRect Raster::GetRasterRect() const
{
    MicroMeterPnt const umSizeHalf { logSizeHalf() };
    MicroMeterRect rect(m_center - umSizeHalf, m_center + umSizeHalf);
    return rect;
}

void Raster::SetRasterRect
(
    CardPoint     const  cardPnt,
    MicroMeterPnt const& pos
)
{
    MicroMeterRect umRect { GetRasterRect() };
    umRect.Manipulate(cardPnt, pos);
    SetRasterRect(umRect);
}

void Raster::SetRasterRect(MicroMeterRect const& umRect)
{
    m_center = umRect.GetCenter();
    m_size = RasterSize
    (
        Cast2Int((umRect.GetWidth () + m_resolution * 0.5f) / m_resolution),
        Cast2Int((umRect.GetHeight() + m_resolution * 0.5f) / m_resolution)
    );
}
