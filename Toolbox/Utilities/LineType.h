// LineType.h
//
// Utilities
//
// Line is defined by two PointType values

#pragma once

#include <math.h> 
#include "NamedType.h"
#include "PointType.h"

template <typename BASE_TYPE> 
class LineType
{
    using POS_TYPE  = PosType<BASE_TYPE>;

public:
    LineType( POS_TYPE const & p1, POS_TYPE const & p2 )
      : m_p1(p1),
        m_p2(p2)
    {}

    BASE_TYPE const Length( ) const
    {
        return Distance( m_p1, m_p2 );
    }

    POS_TYPE const GetVector( ) const 
    {
        return m_p2 - m_p1;
    }

    POS_TYPE const OrthoVector( ) const
    {
        return GetVector().OrthoVector( 1.0_MicroMeter );
    }

    friend BASE_TYPE const PointToLine( LineType const & l, POS_TYPE const & p0 )
    {
        POS_TYPE p01 { p0     - l.m_p1 };
        POS_TYPE p12 { l.m_p1 - l.m_p2 };
        BASE_TYPE res =  p01.GetX() * p12.GetYvalue() - p01.GetY() * p12.GetXvalue() / l.Length();
        return res;
    }

private:
    POS_TYPE m_p1;
    POS_TYPE m_p2;
};

//friend bool const Intersect( LineType const & l1, LineType const & l2 )
//{
//    BASE_TYPE q 
//    (
//        (l1.p1.GetY() - l2.p1.GetY()).GetValue() * (l2.p2.GetX() - l2.p1.GetX()).GetValue() -
//        (l1.p1.GetX() - l2.p1.GetX()).GetValue() * (l2.p2.GetY() - l2.p1.GetY()).GetValue()
//    );
//    BASE_TYPE d 
//    (
//        (l1.p2.GetX() - l1.p1.GetX()).GetValue() * (l2.p2.GetY() - l2.p1.GetY()).GetValue() -
//        (l1.p2.GetY() - l1.p1.GetY()).GetValue() * (l2.p2.GetX() - l2.p1.GetX()).GetValue()
//    );

//    if ( IsCloseToZero( d ) )
//    {
//        return false;
//    }

//    BASE_TYPE r { q.GetValue() / d.GetValue() };

//    q = (l1.p1.GetY() - l2.p1.GetY()) * (l1.p2.GetX() - l1.p1.GetX()).GetValue() -
//        (l1.p1.GetX() - l2.p1.GetX()) * (l1.p2.GetY() - l1.p1.GetY()).GetValue();

//    BASE_TYPE s { q.GetValue() / d.GetValue() };

//    if ( r < BASE_TYPE(0.0) || r > BASE_TYPE(1.0) || s < BASE_TYPE(0.0) || s > BASE_TYPE(1.0) )
//    {
//        return false;
//    }

//    return true;
//}
