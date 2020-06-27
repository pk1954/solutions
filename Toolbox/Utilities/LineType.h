// LineType.h
//
// Utilities
//
// Line is defined by two PointType values

#pragma once

#include <algorithm>  // min/max/abs templates
#include "util.h"
#include "NamedType.h"
#include "PointType.h"

template <typename BASE_TYPE> 
class LineType
{
    using POS_TYPE  = PosType<BASE_TYPE>;

public:
    LineType( POS_TYPE const & p1, POS_TYPE const & p2 )
      : p1(p1),
        p2(p2)
    {}

    friend bool const Intersect( LineType const & l1, LineType const & l2 )
    {
        BASE_TYPE q (
                        (l1.p1.GetY() - l2.p1.GetY()).GetValue() * (l2.p2.GetX() - l2.p1.GetX()).GetValue() 
                      - (l1.p1.GetX() - l2.p1.GetX()).GetValue() * (l2.p2.GetY() - l2.p1.GetY()).GetValue()
                    );
        BASE_TYPE d (
                        (l1.p2.GetX() - l1.p1.GetX()).GetValue() * (l2.p2.GetY() - l2.p1.GetY()).GetValue() 
                      - (l1.p2.GetY() - l1.p1.GetY()).GetValue() * (l2.p2.GetX() - l2.p1.GetX()).GetValue()
                    );

        if ( IsCloseToZero( d ) )
        {
            return false;
        }

        BASE_TYPE r { q.GetValue() / d.GetValue() };

        q = (l1.p1.GetY() - l2.p1.GetY()) * (l1.p2.GetX() - l1.p1.GetX()).GetValue() 
          - (l1.p1.GetX() - l2.p1.GetX()) * (l1.p2.GetY() - l1.p1.GetY()).GetValue();

        BASE_TYPE s { q.GetValue() / d.GetValue() };

        if ( r < BASE_TYPE(0.0) || r > BASE_TYPE(1.0) || s < BASE_TYPE(0.0) || s > BASE_TYPE(1.0) )
        {
            return false;
        }

        return true;
    }

private:
    POS_TYPE p1;
    POS_TYPE p2;
};
