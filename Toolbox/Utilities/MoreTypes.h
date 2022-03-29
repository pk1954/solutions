// MoreTypes.h
//
// Utilities

#pragma once

#include "NamedType.h"
#include "BasicTypes.h"
#include "PointType.h"
#include "RectType.h"
#include "LineType.h"
#include "CircleType.h"
#include "EllipseType.h"

using MicroMeterPnt = PosType<MicroMeter>;

inline static const MicroMeterPnt NP_NULL(MicroMeterPnt::NULL_VAL());   // compiler generates call!
inline static const MicroMeterPnt NP_ZERO(MicroMeterPnt::ZERO_VAL());   // compiler generates call!

static MicroMeterPnt Radian2Vector(Radian const r)
{
	return MicroMeterPnt(Cos(r), Sin(r));
}

static Radian Vector2Radian(MicroMeterPnt const & umPnt)
{
	return Radian(atan2(umPnt.GetYvalue(),umPnt.GetXvalue()));
}

using MicroMeterRect     = RectType<MicroMeter>;
using MicroMeterRectSize = SizeType<MicroMeter>;
using MicroMeterLine     = LineType<MicroMeter>;
using MicroMeterCircle   = CircleType <MicroMeter>;
using MicroMeterEllipse  = EllipseType<MicroMeter>;

template <typename UNIT>
class BASE_PEAK
{
public:
	BASE_PEAK(UNIT b, UNIT p)
	  : base(b),
		peak(p)
	{}

	bool operator==(BASE_PEAK const & rhs) const = default;

	UNIT Base()  const { return base; }
	UNIT Peak()  const { return peak; }
	UNIT Boost() const { return peak - base; }

	void SetBase(UNIT v)
	{
		base = min(v, peak);
	}

	void SetPeak(UNIT v)
	{
		peak = max(v, base);
	}

private:
	UNIT base;
	UNIT peak;
};
