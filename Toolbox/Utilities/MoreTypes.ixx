// MoreTypes.ixx
//
// Toolbox\Utilities

module;

#include <math.h>
#include <iostream>
#include <iomanip>
#include <vector>

export module Types:MoreTypes;

import IoConstants;
import :BasicTypes;
import :CircleType;
import :EllipseType;
import :PointType;
import :RectType;
import :LineType;

using std::vector;
using std::wostream;
using std::setprecision;

export using MicroMeterRect     = RectType   <MicroMeter>;
export using MicroMeterRectSize = SizeType   <MicroMeter>;
export using MicroMeterLine     = LineType   <MicroMeter>;
export using MicroMeterEllipse  = EllipseType<MicroMeter>;
export using MicroMeterCircle   = CircleType <MicroMeter>;
export using MicroMeterPnt      = PosType    <MicroMeter>;

export inline const MicroMeterPnt NP_NULL(MicroMeterPnt::NULL_VAL());   // compiler generates call!
export inline const MicroMeterPnt NP_ZERO(MicroMeterPnt::ZERO_VAL());   // compiler generates call!

export wostream& operator<< (wostream& out, MicroMeterPnt const& pnt)
{
	out << L' '
		<< OPEN_BRACKET
		<< setprecision(10)
		<< pnt.GetX()
		<< SEPARATOR
		<< pnt.GetY()
		<< CLOSE_BRACKET;
	return out;
}

export wostream & operator<< (wostream & out, MicroMeterRect const & rect)
{
	out << rect.GetStartPoint() << rect.GetEndPoint();
	return out;
}

export wostream & operator<< (wostream & out, MicroMeterCircle const & circle)
{
    out << OPEN_BRACKET 
        << circle.GetPos()
        << ID_SEPARATOR 
        << circle.GetRadius()
        << CLOSE_BRACKET;
    return out;
}

export MicroMeterPnt Radian2Vector(Radian const r)
{
	return MicroMeterPnt(Cos(r), Sin(r));
}

export Radian Vector2Radian(MicroMeterPnt const& umPnt)
{
	return Radian(atan2f(umPnt.GetYvalue(), umPnt.GetXvalue()));
}

export MicroMeterPnt Degrees2Vector(Degrees const r)
{
	return Radian2Vector(Degrees2Radian(r));
}

export Degrees Vector2Degrees(MicroMeterPnt const& umPnt)
{
	return Radian2Degrees(Vector2Radian(umPnt));
}

export template <typename T>
inline MicroMeterLine CalcMaxDistLine(vector<T*> const& list) // find two nobs with maximum distance
{
	MicroMeter     maxDist { 0.0_MicroMeter };
	MicroMeterLine lineMax { MicroMeterLine::ZERO_VAL() };
	for (auto it1 : list)
		for (auto it2 : list)    //TODO: optimize
		{
			auto const line { MicroMeterLine(it1->GetPos(), it2->GetPos()) };
			auto const dist { line.Length() };
			if (dist > maxDist)
			{
				maxDist = dist;
				lineMax = line;
			}
		}
	if (lineMax.GetStartPoint().GetX() > lineMax.GetEndPoint().GetX())
		lineMax.Normalize();
	return lineMax;
}
