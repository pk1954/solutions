// CircleType.ixx
//
// Utilities

module;

#include <ostream>

export module Types:CircleType;

import :PointType;
import IoConstants;

using std::wostream;

export template <typename BASE_TYPE> 
class CircleType
{
    using POS_TYPE = PosType<BASE_TYPE>;

public:
    CircleType() :
        m_position(),
        m_radius()
    { }

    CircleType(POS_TYPE const & pos, BASE_TYPE const radius)
        : m_position(pos),
        m_radius(radius)
    {}

    bool operator==(CircleType const & a) const 
    { 
        return (m_position == a.m_position) && (m_radius == a.m_radius); 
    }

    CircleType operator+= (POS_TYPE const a) { m_position += a; return * this; }
    CircleType operator-= (POS_TYPE const a) { m_position -= a; return * this; }

    CircleType operator*= (float const d) { m_radius *= d; return * this; }
    CircleType operator/= (float const d) { m_radius /= d; return * this; }

    POS_TYPE  const & GetPos   () const { return m_position; }
    BASE_TYPE const & GetRadius() const { return m_radius; }

    void SetPos   (POS_TYPE  const & newPos   ) { m_position = newPos; }
    void SetRadius(BASE_TYPE const   newRadius) { m_radius   = newRadius; }

    bool Includes(POS_TYPE const & pos) const
    {
        return DistSquare(pos, m_position) <= m_radius.GetValue() * m_radius.GetValue();  //TODO: better use hypot?
    }

    float DistFactor(POS_TYPE const &pos) const
    {
        float const fDsBorder   { m_radius.GetValue() * m_radius.GetValue() };
        float const fDistSquare { DistSquare(pos, m_position) };
        return (fDistSquare <= fDsBorder)  // pos in circle?
            ? (1.0f - fDistSquare / fDsBorder)
            : 0.0f;
    }

    static CircleType const & NULL_VAL() 
    { 
        static CircleType res { CircleType(POS_TYPE::NULL_VAL(), BASE_TYPE::NULL_VAL()) }; 
        return res;
    };

    static CircleType const & ZERO_VAL() 
    { 
        static CircleType res { CircleType(POS_TYPE::ZERO_VAL(), BASE_TYPE::ZERO_VAL()) }; 
        return res;
    };

    void Set2Zero() { * this = ZERO_VAL(); }
    void Set2Null() { * this = NULL_VAL(); }

    bool IsNull   () const { return * this == NULL_VAL(); };
    bool IsNotNull() const { return * this != NULL_VAL(); };
    bool IsZero   () const { return * this == ZERO_VAL(); };
    bool IsNotZero() const { return * this != ZERO_VAL(); };

    void Rotate(POS_TYPE const & pntPivot, Radian const radDelta)
    {
        m_position.Rotate(pntPivot, radDelta);
    }

    friend CircleType operator* (CircleType const a,  float const d) 
    { 
        CircleType res { a }; 
        res *= d; 
        return res; 
    };

    friend CircleType operator/ (CircleType const a,  float const d) 
    { 
        CircleType res { a }; 
        res /= d; 
        return res; 
    };

private:
    POS_TYPE  m_position;
    BASE_TYPE m_radius;
};
