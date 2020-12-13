// CircleType.h
//
// Utilities

#pragma once

#include "NamedType.h"
#include "PointType.h"

template <typename BASE_TYPE> 
class CircleType
{
    using POS_TYPE = PosType<BASE_TYPE>;

public:
    CircleType( ) 
    { 
        * this = ZERO_VAL(); 
    }

    CircleType( POS_TYPE const & pos, BASE_TYPE const radius ) 
        : m_position( pos ),
          m_radius( radius )
    {}

    auto operator <=> (const CircleType &) const = default;

    CircleType const operator+= (POS_TYPE const a) { m_position += a.m_position; return * this; }
    CircleType const operator-= (POS_TYPE const a) { m_position -= a.m_position; return * this; }

    CircleType const operator*= (float const d) { m_radius *= d; return * this; }
    CircleType const operator/= (float const d) { m_radius /= d; return * this; }

    POS_TYPE  const & GetPosition() const { return m_position; }
    BASE_TYPE const & GetRadius  () const { return m_radius; }

    void SetPosition( POS_TYPE  const & newPos    ) { m_position = newPos; }
    void SetRadius  ( BASE_TYPE const   newRadius ) { m_radius   = newRadius; }

    bool Includes( POS_TYPE const pos ) const
    {
        return DistSquare(pos, m_position) <= m_radius.GetValue() * m_radius.GetValue();
    }

    static CircleType const & NULL_VAL() 
    { 
        static CircleType res { CircleType( POS_TYPE::NULL_VAL(), BASE_TYPE::NULL_VAL() ) }; 
        return res;
    };

    static CircleType const & ZERO_VAL() 
    { 
        static PointType res { CircleType( POS_TYPE::ZERO_VAL(), BASE_TYPE::ZERO_VAL() ) }; 
        return res;
    };

    void Set2Zero( ) { * this = ZERO_VAL(); }
    void Set2Null( ) { * this = NULL_VAL(); }

    bool IsNull   ( ) const { return * this == NULL_VAL(); };
    bool IsNotNull( ) const { return * this != NULL_VAL(); };
    bool IsZero   ( ) const { return * this == ZERO_VAL(); };
    bool IsNotZero( ) const { return * this != ZERO_VAL(); };

    friend CircleType const operator* (CircleType const a,  float const d) 
    { 
        CircleType res { a }; 
        res *= d; 
        return res; 
    };

    friend CircleType const operator/ (CircleType const a,  float const d) 
    { 
        CircleType res { a }; 
        res /= d; 
        return res; 
    };

    friend wostream & operator<< ( wostream & out, CircleType const & param )
    {
        out << param.m_position << param.m_radius.GetValue();
        return out;
    }

private:
    POS_TYPE  m_position;
    BASE_TYPE m_radius;
};
