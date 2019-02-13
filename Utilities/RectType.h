// RectType.h
//

#pragma once

template <typename BASE_TYPE, typename POS_TYPE, typename SIZE_TYPE, typename Parameter> 
class RectType
{
public:

    RectType( ) 
	{ 
		Reset(); 
	}

    RectType( RectType const & rect ) :
        m_Left  (rect.m_Left),
        m_Bottom(rect.m_Bottom),
        m_Right (rect.m_Right),
        m_Top   (rect.m_Top)
    {
		assert( m_Bottom >= m_Top );
    };

    RectType
	( 
		POS_TYPE const pt1, 
		POS_TYPE const pt2 
	)
    { 
        if ( pt1.GetX() < pt2.GetX() )
        {
            m_Left = pt1.GetX();
            m_Right = pt2.GetX();
        }
        else
        {
            m_Left = pt2.GetX();
            m_Right = pt1.GetX();
        }
        if ( pt1.GetY() > pt2.GetY() )
        {
            m_Bottom = pt1.GetY();
            m_Top = pt2.GetY();
        }
        else
        {
            m_Bottom = pt2.GetY();
            m_Top = pt1.GetY();
        }
		assert( m_Bottom >= m_Top );
    };

    RectType( BASE_TYPE const pixLeft, BASE_TYPE const pixTop, BASE_TYPE const pixRight, BASE_TYPE const pixBottom ) :
        m_Left  (pixLeft), 
        m_Top   (pixTop), 
        m_Right (pixRight), 
        m_Bottom(pixBottom)
    { 
		assert( m_Bottom >= m_Top );
    };

	RectType( POS_TYPE const ptOrigin, SIZE_TYPE const & size ) :
        m_Left  (ptOrigin.GetX()),
        m_Top   (ptOrigin.GetY()),
        m_Right (m_Left + size.GetX() - BASE_TYPE(1)),
        m_Bottom(m_Top  + size.GetY() - BASE_TYPE(1))
	{
		assert( m_Bottom >= m_Top );
	}

	void Reset( )
    {
        m_Left   = BASE_TYPE(0);
        m_Top    = BASE_TYPE(0);
        m_Right  = BASE_TYPE(0);
        m_Bottom = BASE_TYPE(0);
    };

    bool IsEmpty   ( ) const { return (m_Left == m_Right) || (m_Top == m_Bottom); };
	bool IsNotEmpty( ) const { return (m_Left <  m_Right) && (m_Top <  m_Bottom); };

	BASE_TYPE const GetLeft  () const { return m_Left;   };
    BASE_TYPE const GetTop   () const { return m_Top;    };
    BASE_TYPE const GetRight () const { return m_Right;  };
    BASE_TYPE const GetBottom() const { return m_Bottom; };

    BASE_TYPE const GetWidth () const { return m_Right  - m_Left + BASE_TYPE(1); }
	BASE_TYPE const GetHeight() const { return m_Bottom - m_Top  + BASE_TYPE(1); }

    SIZE_TYPE const GetSize() const { return SIZE_TYPE( GetWidth(), GetHeight() ); }

    POS_TYPE const GetStartPoint( ) const { return POS_TYPE( GetLeft(),  GetTop()  ); }
    POS_TYPE const GetEndPoint  ( ) const { return POS_TYPE( GetRight(), GetBottom() ); }
    POS_TYPE const GetCenter    ( ) const { return ( GetStartPoint() + GetEndPoint() ) / 2; }

	bool Includes( POS_TYPE const pnt ) const
	{
		return (m_Left <= pnt.GetX()) && (pnt.GetX() < m_Right ) && 
			   (m_Top  <= pnt.GetY()) && (pnt.GetY() < m_Bottom);
	}

	bool Includes( SIZE_TYPE const size ) const
	{
		return ( GetWidth() >= size.GetX() ) && ( GetHeight() >= size.GetY() );
	}

	bool const operator== ( RectType const & a ) const 
	{ 
		return ( a.m_Left  == m_Left  ) && ( a.m_Top    == m_Top    ) && 
			   ( a.m_Right == m_Right ) && ( a.m_Bottom == m_Bottom ); 
	};

    bool const operator!= ( RectType const & a ) const 
	{ 
		return ( a.m_Left  != m_Left  ) || ( a.m_Top    != m_Top    ) || 
			   ( a.m_Right != m_Right ) || ( a.m_Bottom != m_Bottom ); 
	};

	RectType const Scale( BASE_TYPE const pix )  // positive values of pix enlarge rectangle
	{                                            // negative values reduce its size
		return RectType
		{
			m_Left   - pix,
			m_Top    - pix,
			m_Right  + pix,
			m_Bottom + pix
		};
	}

	RectType const operator+= ( POS_TYPE const offset )
	{ 
		m_Left   += offset.GetX();
		m_Top    += offset.GetY();
		m_Right  += offset.GetX();
		m_Bottom += offset.GetY();
		return * this;
	}

	RectType const operator-= ( POS_TYPE const offset )
	{ 
		m_Left   -= offset.GetX();
		m_Top    -= offset.GetY();
		m_Right  -= offset.GetX();
		m_Bottom -= offset.GetY();
		return * this;
	}

	static RectType const & ZERO_VAL() 
	{ 
		static RectType res( POS_TYPE::ZERO_VAL(), POS_TYPE::ZERO_VAL() ); 
		return res;
	};

private:
    BASE_TYPE m_Left;
    BASE_TYPE m_Top;
    BASE_TYPE m_Right;
    BASE_TYPE m_Bottom;
};

template <typename BASE_TYPE, typename POS_TYPE, typename SIZE_TYPE, typename Parameter> 
RectType<BASE_TYPE, POS_TYPE, SIZE_TYPE, Parameter> const operator+ 
(
	RectType<BASE_TYPE, POS_TYPE, SIZE_TYPE, Parameter> const a, 
	POS_TYPE const b
) 
{ 
	RectType<BASE_TYPE, POS_TYPE, SIZE_TYPE, Parameter> res(a); 
	res += b; 
	return res; 
};

template <typename BASE_TYPE, typename POS_TYPE, typename SIZE_TYPE, typename Parameter> 
RectType<BASE_TYPE, POS_TYPE, SIZE_TYPE, Parameter> const operator- 
(
	RectType<BASE_TYPE, POS_TYPE, SIZE_TYPE, Parameter> const a, 
	POS_TYPE const b
) 
{ 
	RectType<BASE_TYPE, POS_TYPE, SIZE_TYPE, Parameter> res(a); 
	res -= b; 
	return res; 
};

template <typename BASE_TYPE, typename POS_TYPE, typename SIZE_TYPE, typename Parameter> 
std::wostream & operator << 
( 
	std::wostream & out, 
	RectType<BASE_TYPE, POS_TYPE, SIZE_TYPE, Parameter> const & rect 
)
{
    out << rect.GetStartPoint() << L' ' << rect.GetEndPoint();
    return out;
}
