// MoreTypes.ixx
//
// Toolbox\Utilities

module;

#include <math.h>

export module Types:MoreTypes;

import :BasicTypes;
import :CircleType;
import :EllipseType;
import :PointType;
import :RectType;
import :LineType;

export using MicroMeterRect     = RectType<MicroMeter>;
export using MicroMeterRectSize = SizeType<MicroMeter>;
export using MicroMeterLine     = LineType<MicroMeter>;
export using MicroMeterEllipse  = EllipseType<MicroMeter>;
export using MicroMeterCircle   = CircleType <MicroMeter>;
export using MicroMeterPnt      = PosType<MicroMeter>;

export inline const MicroMeterPnt NP_NULL(MicroMeterPnt::NULL_VAL());   // compiler generates call!
export inline const MicroMeterPnt NP_ZERO(MicroMeterPnt::ZERO_VAL());   // compiler generates call!

export MicroMeterPnt Radian2Vector(Radian const r)
{
	return MicroMeterPnt(Cos(r), Sin(r));
}

export Radian Vector2Radian(MicroMeterPnt const& umPnt)
{
	return Radian(atan2f(umPnt.GetYvalue(), umPnt.GetXvalue()));
}
