// RectType.h
//
// Utilities

#pragma once

template <typename BASE_TYPE> 
class RectType
{
	using POS_TYPE  = PosType<BASE_TYPE>;
	using SIZE_TYPE = SizeType<BASE_TYPE>;

public:

    RectType() 
	{ 
		* this = ZERO_VAL(); 
	}

 	RectType
	(
		POS_TYPE const & pt1, 
		POS_TYPE const & pt2 
	)
	{ 
		if (pt1.GetX() > pt2.GetX())
		{
			m_Left  = pt2.GetX();
			m_Right = pt1.GetX();
		}
		else
		{
			m_Left  = pt1.GetX();
			m_Right = pt2.GetX();
		}
		if (pt1.GetY() > pt2.GetY())
		{
			m_Bottom = pt1.GetY();
			m_Top    = pt2.GetY();
		}
		else
		{
			m_Bottom = pt2.GetY();
			m_Top    = pt1.GetY();
		}
		assert(m_Bottom >= m_Top);
	};

	RectType
	(
		BASE_TYPE const pixLeft, 
		BASE_TYPE const pixTop, 
		BASE_TYPE const pixRight, 
		BASE_TYPE const pixBottom 
	) :
        m_Left  (pixLeft), 
        m_Top   (pixTop), 
        m_Right (pixRight), 
        m_Bottom(pixBottom)
    { 
		assert(m_Bottom >= m_Top);
    };

	RectType
	(
		POS_TYPE  const & ptOrigin, 
		SIZE_TYPE const & size 
	) :
        m_Left  (ptOrigin.GetX()),
        m_Top   (ptOrigin.GetY()),
        m_Right (m_Left + size.GetX() - BASE_TYPE(1)),
        m_Bottom(m_Top  + size.GetY() - BASE_TYPE(1))
	{
		assert(m_Bottom >= m_Top);
	}

    bool IsEmpty   () const { return (m_Left == m_Right) || (m_Top == m_Bottom); };
	bool IsNotEmpty() const { return (m_Left <  m_Right) && (m_Top <  m_Bottom); };

	BASE_TYPE const GetLeft  () const { return m_Left;   };
    BASE_TYPE const GetTop   () const { return m_Top;    };
    BASE_TYPE const GetRight () const { return m_Right;  };
    BASE_TYPE const GetBottom() const { return m_Bottom; };

    BASE_TYPE const GetWidth () const { return m_Right  - m_Left + BASE_TYPE(BASE_TYPE(1)); }
	BASE_TYPE const GetHeight() const { return m_Bottom - m_Top  + BASE_TYPE(BASE_TYPE(1)); }

    SIZE_TYPE const GetSize() const { return SIZE_TYPE(GetWidth(), GetHeight()); }

    POS_TYPE const GetStartPoint() const { return POS_TYPE(GetLeft (), GetTop   ()); }
    POS_TYPE const GetEndPoint  () const { return POS_TYPE(GetRight(), GetBottom()); }
    POS_TYPE const GetCenter    () const { return (GetStartPoint() + GetEndPoint()) / 2; }

	bool Includes(POS_TYPE const pnt) const
	{
		return (m_Left <= pnt.GetX()) && (pnt.GetX() < m_Right) && 
			   (m_Top  <= pnt.GetY()) && (pnt.GetY() < m_Bottom);
	}

	bool Includes(SIZE_TYPE const size) const
	{
		return (GetWidth() >= size.GetX()) && (GetHeight() >= size.GetY());
	}

	bool Includes(RectType const & rect) const
	{
		return (m_Left <= rect.GetLeft()) && (rect.GetRight () < m_Right) && 
			   (m_Top  <= rect.GetTop ()) && (rect.GetBottom() < m_Bottom);
	}

	bool IsIncludedIn(RectType const & rect) const
	{
		return rect.Includes(this);
	}

	void Expand(POS_TYPE const & pnt)
	{
		if (* this == ZERO_VAL())
		{
			* this = RectType(pnt, pnt);
		}
		else
		{
			if      (pnt.GetX() < m_Left  ) m_Left   = pnt.GetX();
			else if (pnt.GetX() > m_Right ) m_Right  = pnt.GetX();
			if      (pnt.GetY() < m_Top   ) m_Top    = pnt.GetY();
			else if (pnt.GetY() > m_Bottom) m_Bottom = pnt.GetY();
		}
	}

	bool const operator== (RectType const & a) const 
	{ 
		return (a.m_Left  == m_Left ) && (a.m_Top    == m_Top   ) && 
			   (a.m_Right == m_Right) && (a.m_Bottom == m_Bottom); 
	};

    bool const operator!= (RectType const & a) const 
	{ 
		return (a.m_Left  != m_Left ) || (a.m_Top    != m_Top   ) || 
			   (a.m_Right != m_Right) || (a.m_Bottom != m_Bottom); 
	};

	RectType const ScaleRect(BASE_TYPE const val) const // positive values of val enlarge rectangle
	{                                                   // negative values reduce its size
		return RectType
		{
			m_Left   - val,
			m_Top    - val,
			m_Right  + val,
			m_Bottom + val
		};
	}

	RectType const operator+= (POS_TYPE const offset)
	{ 
		m_Left   += offset.GetX();
		m_Top    += offset.GetY();
		m_Right  += offset.GetX();
		m_Bottom += offset.GetY();
		return * this;
	}

	RectType const operator-= (POS_TYPE const offset)
	{ 
		m_Left   -= offset.GetX();
		m_Top    -= offset.GetY();
		m_Right  -= offset.GetX();
		m_Bottom -= offset.GetY();
		return * this;
	}

	void SetZero()
	{
		* this = ZERO_VAL();
	}

	friend RectType const operator+ (RectType const a, POS_TYPE const b	) 
	{ 
		RectType res { a }; 
		res += b; 
		return res; 
	}

	friend RectType const operator- (RectType const a, POS_TYPE const b	) 
	{ 
		RectType res { a }; 
		res -= b;
		return res; 
	}

	friend RectType const Union(RectType const a, RectType const b	) 
	{ 
		return RectType
		{
			min(a.m_Left,   b.m_Left  ), 
			min(a.m_Top,    b.m_Top   ), 
			max(a.m_Right,  b.m_Right ), 
			max(a.m_Bottom, b.m_Bottom)
		};
	}

	friend std::wostream & operator << (std::wostream & out, RectType const & rect)
	{
		out << rect.GetStartPoint() << rect.GetEndPoint();
		return out;
	}

	static RectType const & ZERO_VAL() 
	{ 
		static RectType res { POS_TYPE::ZERO_VAL(), POS_TYPE::ZERO_VAL() }; 
		return res;
	};

private:
    BASE_TYPE m_Left;
    BASE_TYPE m_Top;
    BASE_TYPE m_Right;
    BASE_TYPE m_Bottom;
};
