// Raster.ixx 
//
// ToolBox\Utilities

module;

#include <optional>

export module Raster;

import Types;

using std::optional;

export using RasterIndex = int;
export using RasterPoint = PosType<RasterIndex>;
export using RasterSize  = SizeType<RasterIndex>;

export class Raster
{
public:

Raster
(
    MicroMeter    resolution,
    MicroMeterPnt center,
    RasterSize    size
)
 :  m_resolution(resolution),
    m_center(center),
    m_size(size)
{}

void SetResolution(MicroMeter const logUnitRes)
{
    m_resolution = logUnitRes;
}

MicroMeterPnt Origin() const
{
    return m_center - logSizeHalf();
}

optional<RasterPoint> FindRasterPos(MicroMeterPnt const) const;
MicroMeterRect        GetRasterRect() const;

void SetRasterRect(CardPoint const, MicroMeterPnt const&);
void SetRasterRect(MicroMeterRect const&);

RasterSize const& Size()       const { return m_size; }
MicroMeter        Resolution() const { return m_resolution; }

private:

    MicroMeter    m_resolution { 100.0_MicroMeter };
    MicroMeterPnt m_center     { NP_ZERO };
    RasterSize    m_size       { 1, 1 };

    MicroMeterPnt logSizeHalf() const
    {
        MicroMeterPnt res(MicroMeter(Cast2Float(m_size.GetX())), MicroMeter(Cast2Float(m_size.GetY())));
        res *= m_resolution.GetValue() * 0.5f;
        return res;
    }
};
