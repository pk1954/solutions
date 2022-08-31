// MoreTypes.ixx
//
// Toolbox\Utilities

module;

#include <math.h>

export module MoreTypes;

import BasicTypes;
import PointType;
import RectType;
import LineType;
import EllipseType;
import CircleType;

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
	return Radian(atan2(umPnt.GetYvalue(), umPnt.GetXvalue()));
}
