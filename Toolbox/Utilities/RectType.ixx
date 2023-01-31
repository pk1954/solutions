// RectType.ixx
//
// Utilities

module;

#include <cassert>
#include <ostream>
#include <algorithm>

export module Types:RectType;

import :PointType;

using std::wostream;
using std::min;
using std::max;

export template <typename BASE_TYPE> 
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
		BASE_TYPE const left, 
		BASE_TYPE const top, 
		BASE_TYPE const right, 
		BASE_TYPE const bottom 
	) :
		m_Left  (left), 
		m_Top   (top), 
		m_Right (right), 
		m_Bottom(bottom)
	{ 
		assert(m_Bottom >= m_Top);
	};

	RectType
	(
		POS_TYPE  const & pos, 
		SIZE_TYPE const & size 
	) :
		m_Left  (pos.GetX()),
		m_Top   (pos.GetY()),
		m_Right (m_Left + size.GetX() - BASE_TYPE(1)),
		m_Bottom(m_Top  + size.GetY() - BASE_TYPE(1))
	{
		assert(m_Bottom >= m_Top);
	}

	void SetLeft  (BASE_TYPE const val) { m_Left   = val; }
	void SetTop   (BASE_TYPE const val) { m_Top    = val; }
	void SetRight (BASE_TYPE const val) { m_Right  = val; }
	void SetBottom(BASE_TYPE const val) { m_Bottom = val; }
	void SetWidth (BASE_TYPE const val) { m_Right  = m_Left + val - BASE_TYPE(BASE_TYPE(1)); }
	void SetHeight(BASE_TYPE const val) { m_Bottom = m_Top  + val - BASE_TYPE(BASE_TYPE(1)); }
	void SetPos   (POS_TYPE  const pos) { SetLeft (pos.GetX()); SetRight (pos.GetY()); }
	void SetSize  (SIZE_TYPE const siz) { SetWidth(siz.GetX()); SetHeight(siz.GetY()); }

	bool IsEmpty   () const { return (m_Left == m_Right) || (m_Top == m_Bottom); };
	bool IsNotEmpty() const { return (m_Left <  m_Right) && (m_Top <  m_Bottom); };

	BASE_TYPE GetLeft  () const { return m_Left;   };
	BASE_TYPE GetTop   () const { return m_Top;    };
	BASE_TYPE GetRight () const { return m_Right;  };
	BASE_TYPE GetBottom() const { return m_Bottom; };

	BASE_TYPE GetWidth () const { return m_Right  - m_Left + BASE_TYPE(BASE_TYPE(1)); }
	BASE_TYPE GetHeight() const { return m_Bottom - m_Top  + BASE_TYPE(BASE_TYPE(1)); }

	SIZE_TYPE GetSize() const { return SIZE_TYPE(GetWidth(), GetHeight()); }

	POS_TYPE  GetStartPoint() const { return POS_TYPE(GetLeft (), GetTop   ()); }
	POS_TYPE  GetEndPoint  () const { return POS_TYPE(GetRight(), GetBottom()); }
	POS_TYPE  GetCenter    () const { return (GetStartPoint() + GetEndPoint()) / 2; }

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

	bool operator== (RectType const & a) const 
	{ 
		return (a.m_Left  == m_Left ) && (a.m_Top    == m_Top   ) && 
			   (a.m_Right == m_Right) && (a.m_Bottom == m_Bottom); 
	};

	bool operator!= (RectType const & a) const 
	{ 
		return (a.m_Left  != m_Left ) || (a.m_Top    != m_Top   ) || 
			   (a.m_Right != m_Right) || (a.m_Bottom != m_Bottom); 
	};

	RectType ScaleRect(BASE_TYPE const val) const // positive values of val enlarge rectangle
	{                                             // negative values reduce its size
		return RectType
		{
			m_Left   - val,
			m_Top    - val,
			m_Right  + val,
			m_Bottom + val
		};
	}

	RectType operator+= (POS_TYPE const offset)
	{ 
		m_Left   += offset.GetX();
		m_Top    += offset.GetY();
		m_Right  += offset.GetX();
		m_Bottom += offset.GetY();
		return * this;
	}

	RectType operator-= (POS_TYPE const offset)
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

	friend RectType operator+ (RectType const a, POS_TYPE const b	) 
	{ 
		RectType res { a }; 
		res += b; 
		return res; 
	}

	friend RectType operator- (RectType const a, POS_TYPE const b	) 
	{ 
		RectType res { a }; 
		res -= b;
		return res; 
	}

	friend RectType Union(RectType const a, RectType const b	) 
	{ 
		return RectType
		{
			min(a.m_Left,   b.m_Left  ), 
			min(a.m_Top,    b.m_Top   ), 
			max(a.m_Right,  b.m_Right ), 
			max(a.m_Bottom, b.m_Bottom)
		};
	}

	friend wostream & operator << (wostream & out, RectType const & rect)
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
