// MoreTypes.h
//
// Utilities

#pragma once

#include <iomanip>
#include <algorithm>
#include "BasicTypes.h"
#include "RectType.h"
#include "LineType.h"
#include "CircleType.h"
#include "EllipseType.h"

import NamedType;
import PointType;

using std::min;
using std::max;
using std::setw;

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

	friend wostream & operator<< (wostream & out, BASE_PEAK<UNIT> const & data)
	{
		out << setw(5) << data.base << L' ' << data.peak << L' ';
		return out;
	}
private:
	UNIT base;
	UNIT peak;
};
