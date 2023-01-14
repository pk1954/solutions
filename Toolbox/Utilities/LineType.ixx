// LineType.ixx
//
// Utilities
//
// Line is defined by two PointType values

module;

#include <algorithm>
#include <math.h> 

export module Types:LineType;

import :PointType;
import Geometry;

using std::swap;

export template <typename BASE_TYPE>
BASE_TYPE PointToLine(PosType<BASE_TYPE> const& l1, PosType<BASE_TYPE> const& l2, PosType<BASE_TYPE> const& p0)
{
    PosType<BASE_TYPE> const p01 { p0 - l1 };
    PosType<BASE_TYPE> const p12 { l1 - l2 };
    BASE_TYPE          const res { (p01.GetX() * p12.GetYvalue() - p01.GetY() * p12.GetXvalue()) / Distance(l1, l2).GetValue() };
    if (isnan(res.GetValue()))
    {
        int x = 42;
    }
    return res;
}

export template <typename BASE_TYPE> 
class LineType
{
    using POS_TYPE = PosType<BASE_TYPE>;

public:
    LineType(POS_TYPE const & p1, POS_TYPE const & p2)
        : m_p1(p1),
        m_p2(p2)
    {}

    LineType(LineType const & l)
        : m_p1(l.m_p1),
        m_p2(l.m_p2)
    {}

    LineType() = default;

    bool operator==(LineType const & a) const { return (m_p1 == a.m_p1) && (m_p2 == a.m_p2); }
    bool operator!=(LineType const & a) const { return (m_p1 != a.m_p1) || (m_p2 != a.m_p2); }

    LineType operator+= (POS_TYPE const a) { m_p1 += a; m_p2 += a; return * this; }
    LineType operator-= (POS_TYPE const a) { m_p1 -= a; m_p2 -= a; return * this; }

    friend LineType operator+ (LineType const l, POS_TYPE const p) 
    { 
        LineType res { l }; 
        res += p; 
        return res; 
    };

    friend LineType operator- (LineType const l, POS_TYPE const p) 
    { 
        LineType res { l }; 
        res -= p; 
        return res; 
    };

    void SetPoints(POS_TYPE const & p1, POS_TYPE const & p2) 
    { 
        m_p1 = p1; 
        m_p2 = p2; 
    }

    BASE_TYPE Length() const
    {
        return Distance(m_p1, m_p2);
    }

    POS_TYPE GetVector() const 
    {
        return m_p2 - m_p1;
    }

    POS_TYPE GetStartPoint() const { return m_p1; }
    POS_TYPE GetEndPoint  () const { return m_p2; }
    POS_TYPE GetCenter    () const { return (m_p1 + m_p2) * 0.5f; }

    POS_TYPE OrthoVector() const
    {
        return GetVector().OrthoVector().ScaledTo(1.0_MicroMeter);
    }

    LineType OrthoLine() const
    {
        return LineType(m_p1, m_p1 + OrthoVector());
    }

    friend BASE_TYPE PointToLine(LineType const& l, POS_TYPE const& p0)
    {
        return PointToLine(l.m_p1, l.m_p2, p0);
    }

   static LineType const & NULL_VAL()
    { 
        static const POS_TYPE posNull { POS_TYPE(BASE_TYPE::NULL_VAL(), BASE_TYPE::NULL_VAL()) }; 
        static const LineType res     { LineType(posNull, posNull) }; 
        return res;
    };

    static LineType const & ZERO_VAL() 
    { 
        static const POS_TYPE posZero { POS_TYPE(BASE_TYPE::ZERO_VAL(), BASE_TYPE::ZERO_VAL()) }; 
        static const LineType res     { LineType(posZero, posZero) }; 
        return res;
    };

    void Set2Zero() { * this = ZERO_VAL(); }
    void Set2Null() { * this = NULL_VAL(); }

    bool IsNull   () const { return * this == NULL_VAL(); };
    bool IsNotNull() const { return * this != NULL_VAL(); };
    bool IsZero   () const { return * this == ZERO_VAL(); };
    bool IsNotZero() const { return * this != ZERO_VAL(); };

    void Swap() { swap(m_p1, m_p2); }
    void Normalize() 
    { 
        POS_TYPE vector { GetVector() };
        if (vector.GetX() >  BASE_TYPE::ZERO_VAL())
            return;
        if ((vector.GetX() == BASE_TYPE::ZERO_VAL()) && (vector.GetY() > BASE_TYPE::ZERO_VAL()))
            return;
        swap(m_p1, m_p2); 
    }

private:
    POS_TYPE m_p1;
    POS_TYPE m_p2;
};

//friend bool const Intersect(LineType const & l1, LineType const & l2)
//{
//    BASE_TYPE q 
//    (
//        (l1.p1.GetY() - l2.p1.GetY()).GetValue() * (l2.p2.GetX() - l2.p1.GetX()).GetValue() -
//        (l1.p1.GetX() - l2.p1.GetX()).GetValue() * (l2.p2.GetY() - l2.p1.GetY()).GetValue()
//   );
//    BASE_TYPE d 
//    (
//        (l1.p2.GetX() - l1.p1.GetX()).GetValue() * (l2.p2.GetY() - l2.p1.GetY()).GetValue() -
//        (l1.p2.GetY() - l1.p1.GetY()).GetValue() * (l2.p2.GetX() - l2.p1.GetX()).GetValue()
//   );

//    if (IsCloseToZero(d))
//    {
//        return false;
//    }

//    BASE_TYPE r { q.GetValue() / d.GetValue() };

//    q = (l1.p1.GetY() - l2.p1.GetY()) * (l1.p2.GetX() - l1.p1.GetX()).GetValue() -
//        (l1.p1.GetX() - l2.p1.GetX()) * (l1.p2.GetY() - l1.p1.GetY()).GetValue();

//    BASE_TYPE s { q.GetValue() / d.GetValue() };

//    if (r < BASE_TYPE(0.0) || r > BASE_TYPE(1.0) || s < BASE_TYPE(0.0) || s > BASE_TYPE(1.0))
//    {
//        return false;
//    }

//    return true;
//}
