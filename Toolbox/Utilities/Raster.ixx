// Raster.ixx 
//
// ToolBox\Utilities

module;

#include <cmath>

export module Raster;

import Types;

using std::round;

export using RasterIndex = int;
export using RasterPnt   = PosType<RasterIndex>;

export template <typename LOG_UNIT>
class Raster
{
public:

Raster
(
    LOG_UNIT          resolution,
    PosType<LOG_UNIT> origin,
    SizeType<int>     size
)
 :  m_resolution(resolution),
    m_origin(origin),
    m_size(size)
{}

RasterPnt FindRasterPos(MicroMeterPnt const umPnt) const
{
    MicroMeterPnt const umOff { umPnt - m_origin };
    MicroMeterPnt const umPos { umOff / m_resolution };
    RasterPnt     const pos   { round(umPos.GetXvalue()), round(umPos.GetYvalue()) };
    return 
    (
        (pos.GetX() < 0) || (pos.GetX() >= m_size.GetX()) ||
        (pos.GetY() < 0) || (pos.GetY() >= m_size.GetY())
    )
    ? RasterPnt::NULL_VAL()
    : pos;
}

private:

    LOG_UNIT          m_resolution { LOG_UNIT(1.0f) };
    PosType<LOG_UNIT> m_origin;
    SizeType<int>     m_size { 1, 1 };
};
