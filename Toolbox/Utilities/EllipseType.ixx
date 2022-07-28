// EllipseType.ixx
//
// Utilities

export module EllipseType;

import NamedType;
import PointType;
import CircleType;
import RectType;

export template <typename BASE_TYPE> 
class EllipseType
{
    using POS_TYPE = PosType<BASE_TYPE>;

public:
    EllipseType() 
    { 
        * this = ZERO_VAL(); 
    }

    EllipseType(POS_TYPE const & pos, BASE_TYPE const radiusX, BASE_TYPE const radiusY) 
        : m_position(pos),
        m_radiusX(radiusX),
        m_radiusY(radiusY)
    {}

    explicit EllipseType(CircleType<BASE_TYPE> const & circle) 
        : m_position(circle.GetPos()),
        m_radiusX(circle.GetRadius()),
        m_radiusY(circle.GetRadius())
    {}

    explicit EllipseType(RectType<BASE_TYPE> const & rect) 
        : m_position(rect.GetCenter()),
        m_radiusX(rect.GetWidth() / 2),
        m_radiusY(rect.GetHeight() / 2)
    {}

    bool operator==(EllipseType const & a) const 
    { 
        return 
            (m_position == a.m_position) &&
            (m_radiusX  == a.m_radiusX) &&
            (m_radiusY  == a.m_radiusY);
    }

    EllipseType operator+= (POS_TYPE const a) { m_position += a.m_position; return * this; }
    EllipseType operator-= (POS_TYPE const a) { m_position -= a.m_position; return * this; }

    EllipseType operator*= (float const d) { m_radiusX *= d; m_radiusY *= d; return * this; }
    EllipseType operator/= (float const d) { m_radiusX /= d; m_radiusY /= d; return * this; }

    POS_TYPE  const & GetPos() const { return m_position; }
    BASE_TYPE const & GetRadiusX () const { return m_radiusX; }
    BASE_TYPE const & GetRadiusY () const { return m_radiusY; }

    void SetPos    (POS_TYPE  const & newPos ) { m_position = newPos; }
    void SetRadiusX(BASE_TYPE const   newRadX) { m_radiusX  = newRadX; }
    void SetRadiusY(BASE_TYPE const   newRadY) { m_radiusY  = newRadY; }

    static EllipseType const & NULL_VAL() 
    { 
        static EllipseType res { EllipseType(POS_TYPE::NULL_VAL, BASE_TYPE::NULL_VAL(), BASE_TYPE::NULL_VAL()) }; 
        return res;
    };

    static EllipseType const & ZERO_VAL() 
    { 
        static PointType res { EllipseType(POS_TYPE::ZERO_VAL, BASE_TYPE::ZERO_VAL(), BASE_TYPE::ZERO_VAL()) }; 
        return res;
    };

    void Set2Zero() { * this = ZERO_VAL(); }
    void Set2Null() { * this = NULL_VAL(); }

    bool IsNull   () const { return * this == NULL_VAL(); };
    bool IsNotNull() const { return * this != NULL_VAL(); };
    bool IsZero   () const { return * this == ZERO_VAL(); };
    bool IsNotZero() const { return * this != ZERO_VAL(); };

    friend EllipseType const operator* (EllipseType const a, float const d) 
    { 
        EllipseType res { a }; 
        res *= d; 
        return res; 
    };

    friend EllipseType const operator/ (EllipseType const a, float const d) 
    { 
        EllipseType res { a }; 
        res /= d; 
        return res; 
    };

private:
    POS_TYPE  m_position;
    BASE_TYPE m_radiusX;
    BASE_TYPE m_radiusY;
};
