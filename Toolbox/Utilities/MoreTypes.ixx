// MoreTypes.ixx
//
// Toolbox\Utilities

module;

#include <ostream>
#include <iomanip>
#include <algorithm>
#include "BasicTypes.h"

export module MoreTypes;

import NamedType;
import PointType;
import RectType;
import LineType;
import EllipseType;
import CircleType;
import AngleTypes;

using std::min;
using std::max;
using std::setw;
using std::wostream;

export using MicroMeterPnt = PosType<MicroMeter>;

export inline const MicroMeterPnt NP_NULL(MicroMeterPnt::NULL_VAL());   // compiler generates call!
export inline const MicroMeterPnt NP_ZERO(MicroMeterPnt::ZERO_VAL());   // compiler generates call!

export MicroMeterPnt Radian2Vector(Radian const r)
{
	return MicroMeterPnt(Cos(r), Sin(r));
}

export Radian Vector2Radian(MicroMeterPnt const & umPnt)
{
	return Radian(atan2(umPnt.GetYvalue(),umPnt.GetXvalue()));
}

export using MicroMeterRect     = RectType<MicroMeter>;
export using MicroMeterRectSize = SizeType<MicroMeter>;
export using MicroMeterLine     = LineType<MicroMeter>;
export using MicroMeterEllipse  = EllipseType<MicroMeter>;
export using MicroMeterCircle   = CircleType <MicroMeter>;

export template <typename UNIT>
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
