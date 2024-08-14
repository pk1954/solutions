// PointType.ixx
//
// Toolbox\Types

module;

#include <algorithm>  // min/max/abs templates
#include <cmath>

export module Types:PointType;

import Debug;
import Util;
import :BasicTypes;

using std::abs;
using std::max;
using std::hypot;

export template <typename BASE_TYPE, typename Parameter> 
class PointType
{
public:

	PointType() : m_x(BASE_TYPE::NULL_VAL()), m_y(BASE_TYPE::NULL_VAL()) { }
	PointType(BASE_TYPE const _x, BASE_TYPE const _y) : m_x(_x), m_y(_y) {}

	explicit PointType(BASE_TYPE const _b) : m_x(_b), m_y(_b) {}

	bool      operator==(PointType const & a) const { return (m_x == a.m_x) && (m_y == a.m_y); }
	bool      operator!=(PointType const & a) const { return (m_x != a.m_x) || (m_y != a.m_y); }

	PointType operator+= (PointType const a) { m_x += a.m_x; m_y += a.m_y; return * this; }
	PointType operator-= (PointType const a) { m_x -= a.m_x; m_y -= a.m_y; return * this; }
	PointType operator%= (PointType const a) { m_x %= a.m_x; m_y %= a.m_y; return * this; }

	PointType operator*= (int       const i) { m_x *= i; m_y *= i; return * this; }
	PointType operator/= (int       const i) { m_x /= i; m_y /= i; return * this; }

	PointType operator*= (float     const d) { m_x *= d; m_y *= d; return * this; }
	PointType operator/= (float     const d) { m_x /= d; m_y /= d; return * this; }

	PointType operator- () const { return PointType { -m_x, -m_y }; };
	PointType operator+ () const { return PointType { +m_x, +m_y }; };

	void MoveHorz(BASE_TYPE const offset) {	m_x += offset; }
	void MoveVert(BASE_TYPE const offset) {	m_y += offset; }

	BASE_TYPE GetX() const { return m_x; }
	BASE_TYPE GetY() const { return m_y; }

	void SetX(BASE_TYPE const b) { m_x = b; }
	void SetY(BASE_TYPE const b) { m_y = b; }

	auto GetXvalue() const { return GetX().GetValue(); }
	auto GetYvalue() const { return GetY().GetValue(); }

	static PointType const & NULL_VAL() 
	{ 
		static PointType res { PointType(BASE_TYPE::NULL_VAL()) }; 
		return res;
	};

	static PointType const & ZERO_VAL() 
	{ 
		static PointType res { PointType(BASE_TYPE::ZERO_VAL()) }; 
		return res;
	};

	void Set2Zero() { * this = ZERO_VAL(); }
	void Set2Null() { * this = NULL_VAL(); }

	bool IsNull   () const { return * this == NULL_VAL(); };
	bool IsNotNull() const { return * this != NULL_VAL(); };
	bool IsZero   () const { return * this == ZERO_VAL(); };
	bool IsNotZero() const { return * this != ZERO_VAL(); };

	float AspectRatio() const { return m_y / m_x; }

	friend BASE_TYPE Distance(PointType const & npA, PointType const & npB)
	{
		PointType diff { npA - npB };
		return (diff.IsCloseToZero()) ? BASE_TYPE::ZERO_VAL() : Hypot(diff);
	}

	friend float DistSquare(PointType const pntA, PointType const pntB)
	{
		PointType delta { pntA - pntB };
		return delta.GetXvalue() * delta.GetXvalue() + delta.GetYvalue() * delta.GetYvalue();
	}

	friend float CrossProduct(PointType const pntA, PointType const pntB)
	{
		return pntA.GetXvalue() * pntB.GetYvalue() - pntA.GetYvalue() * pntB.GetXvalue();
	}

	bool IsCloseToZero() const
	{
		return ::IsCloseToZero(GetXvalue()) && ::IsCloseToZero(GetYvalue());
	}

	bool IsCloseTo(PointType const & pt) const
	{
		return (*this - pt).IsCloseToZero();
	}

	friend BASE_TYPE Hypot(PointType const pt) 
	{ 
		return BASE_TYPE(hypot(pt.GetXvalue(), pt.GetYvalue()));
	};

	friend PointType Normalize(PointType const & pt) 
	{ 
		BASE_TYPE fHypot { Hypot(pt) };
		Assert(! ::IsCloseToZero(fHypot.GetValue()));
		return pt / fHypot.GetValue();
	};

	PointType ScaledTo(BASE_TYPE const length) const
	{
		return Normalize(* this) * length.GetValue();
	}

	PointType OrthoVector() const
	{
		return PointType(GetY(), -GetX());
	}

	friend PointType operator+ (PointType const a, PointType const b) 
	{ 
		PointType res { a }; 
		res += b; 
		return res; 
	};

	friend PointType operator- (PointType const a, PointType const b) 
	{ 
		PointType res { a }; 
		res -= b; 
		return res; 
	};

	friend PointType operator% (PointType const a, PointType const b) 
	{ 
		PointType res { a }; 
		res %= b; 
		return res; 
	};

	friend PointType operator+ (PointType const a, int const i) 
	{ 
		PointType res { a }; 
		res += i; 
		return res; 
	};

	friend PointType operator+ (PointType const a, float const d) 
	{ 
		PointType res { a }; 
		res += d; 
		return res; 
	};

	friend PointType operator- (PointType const a, int const i) 
	{ 
		PointType res { a }; 
		res -= i; 
		return res; 
	};

	friend PointType operator- (PointType const a, float const d) 
	{ 
		PointType res { a }; 
		res -= d; 
		return res; 
	};

	friend PointType operator* (PointType const a,  float const d) 
	{ 
		PointType res { a }; 
		res *= d; 
		return res; 
	};

	friend PointType operator* (PointType const a,  int const i) 
	{ 
		PointType res { a }; 
		res *= i; 
		return res; 
	};

	friend PointType operator/ (PointType const a, float const d) 
	{ 
		PointType res { a }; 
		res /= d; 
		return res; 
	};

	void IncX()	{ ++m_x; }
	void IncY()	{ ++m_y; }

	void Move2Horz(BASE_TYPE const offset)
	{
		m_x += offset;
	}

	void Move2Vert(BASE_TYPE const offset)
	{
		m_y += offset;
	}

	PointType Horz(BASE_TYPE const offset) const
	{
		return PointType(m_x + offset, m_y);
	}

	PointType Vert(BASE_TYPE const offset) const
	{
		return PointType(m_x, m_y + offset);
	}

	PointType Round(BASE_TYPE const raster) const
	{
		return PointType
		(
			m_x.Round(raster),
			m_y.Round(raster)
		);
	}

	PointType Round() const
	{
		return PointType(Round(m_x), Round(m_y));
	}

	// makes problems! Not quite clear, but works without.
	//friend PointType const operator/ (PointType const a, int const i) 
	//{ 
	//	PointType res { a }; 
	//	res /= i; 
	//	return res; 
	//};

	friend BASE_TYPE MaxAbsDelta(PointType const pnt) 
	{
		BASE_TYPE xAbs { abs(pnt.GetXvalue()) };
		BASE_TYPE yAbs { abs(pnt.GetYvalue()) };
		return max(xAbs, yAbs);
	}

	void Rotate(PointType const & pntPivot, Radian const radDelta)
	{
		PointType const pntVectorOld    { *this - pntPivot };
		Radian    const radOld          { Vector2Radian(pntVectorOld) };
		Radian    const radNew          { radOld + radDelta };
		PointType const pntVectorNew    { Radian2Vector(radNew) };
		BASE_TYPE const distFromPivot   { Hypot(pntVectorOld) };
		PointType const pntVectorScaled { pntVectorNew.ScaledTo(distFromPivot) };
		PointType const pntPosNew       { pntPivot + pntVectorScaled };
		*this = pntPosNew;
	}

private:
	BASE_TYPE m_x;
	BASE_TYPE m_y;
};

// PosType: A specialized PointType used to denote the position of something

export template <typename BASE_TYPE> 
class PosType: public PointType<BASE_TYPE, struct pos_tag> 
{
public:
	using PointType<BASE_TYPE, struct pos_tag>::PointType;

	PosType(PointType<BASE_TYPE, struct pos_tag> const & p) 
		: PointType<BASE_TYPE, struct pos_tag>(p)	
	{}
};

// SizeType: A specialized PointType used to denote the size of something

export template <typename BASE_TYPE> 
class SizeType: public PointType<BASE_TYPE, struct siz_tag> 
{
public:
	using PointType<BASE_TYPE, struct siz_tag>::PointType;

	SizeType(PointType<BASE_TYPE, struct siz_tag> const & p) 
		: PointType<BASE_TYPE, struct siz_tag>(p) 
	{}
};
