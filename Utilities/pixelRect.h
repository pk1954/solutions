// pixelRect.h
//

#pragma once

#include <assert.h>
#include "pixelPoint.h"
#include "pixelRectSize.h"

class PixelRect
{
public:
    PixelRect( ) :
        m_Left  (PIXEL(0_PIXEL)), 
        m_Top   (PIXEL(0_PIXEL)), 
        m_Right (PIXEL(0_PIXEL)), 
        m_Bottom(PIXEL(0_PIXEL))
    {};

    PixelRect( PixelRect const & rect ) :
        m_Left  (rect.m_Left),
        m_Bottom(rect.m_Bottom),
        m_Right (rect.m_Right),
        m_Top   (rect.m_Top)
    {
		assert( m_Bottom >= m_Top );
    };

    PixelRect( PixelPoint const pt1, PixelPoint const pt2 )
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

    PixelRect( PIXEL const pixLeft, PIXEL const pixTop, PIXEL const pixRight, PIXEL const pixBottom ) :
        m_Left  (pixLeft), 
        m_Top   (pixTop), 
        m_Right (pixRight), 
        m_Bottom(pixBottom)
    { 
		assert( m_Bottom >= m_Top );
    };

	PixelRect( PixelPoint const ptOrigin, PixelRectSize const & rectSize ) :
        m_Left  (ptOrigin.GetX()),
        m_Top   (ptOrigin.GetY()),
        m_Right (m_Left + rectSize.GetWidth()  - PIXEL(1_PIXEL)),
        m_Bottom(m_Top  + rectSize.GetHeight() - PIXEL(1_PIXEL))
	{
	PIXEL p = PIXEL(1_PIXEL);
	p += PIXEL(3_PIXEL);
	p -= PIXEL(2_PIXEL);
	PIXEL p2 = p - PIXEL(2_PIXEL);
	PIXEL p3 = p + PIXEL(2_PIXEL);

		assert( m_Bottom >= m_Top );
	}

    void SetEmpty( )
    {
        m_Left   = PIXEL(0_PIXEL);
        m_Top    = PIXEL(0_PIXEL);
        m_Right  = PIXEL(0_PIXEL);
        m_Bottom = PIXEL(0_PIXEL);
    };

    bool IsEmpty( ) const
    {
        return (m_Left == m_Right) || (m_Top == m_Bottom);
    };

    bool IsNotEmpty( ) const
    {
        return (m_Left < m_Right) && (m_Top < m_Bottom );
    };

    PIXEL const GetLeft( ) const 
    {
        return m_Left;
    }

    PIXEL const GetRight( ) const 
    {
        return m_Right;
    }

    PIXEL const GetTop( ) const 
    {
        return m_Top;
    }

    PIXEL const GetBottom( ) const 
    {
        return m_Bottom;
    }

    PIXEL const GetWidth( ) const 
    {
        return m_Right - m_Left + PIXEL(1_PIXEL);
    }

    PIXEL const GetHeight( ) const 
    {
        return m_Bottom - m_Top + PIXEL(1_PIXEL);
    }

    PixelRectSize const GetSize( ) const 
    {
        return PixelRectSize( GetWidth(), GetHeight() );
    }

    PixelPoint const GetStartPoint( ) const 
    {
        return PixelPoint( m_Left, m_Top );
    }

    PixelPoint const GetEndPoint( ) const
    {
        return PixelPoint( m_Right, m_Bottom );
    }

    PixelPoint const GetCenter( ) const
    {
        return ( GetStartPoint() + GetEndPoint() ) / 2;
    }

	bool Includes( PixelPoint const pnt ) const
	{
		return (m_Left <= pnt.GetX()) && (pnt.GetX() < m_Right) && (m_Top <= pnt.GetY()) && (pnt.GetY() < m_Bottom);
	}

	bool Includes( PixelRectSize const size ) const
	{
		return ( GetWidth() >= size.GetWidth() ) && ( GetHeight() >= size.GetHeight() );
	}

	bool const operator== ( PixelRect const & a ) const { return ( a.m_Left == m_Left ) && ( a.m_Top == m_Top ) && ( a.m_Right == m_Right ) && ( a.m_Bottom == m_Bottom ); };
    bool const operator!= ( PixelRect const & a ) const { return ( a.m_Left != m_Left ) || ( a.m_Top != m_Top ) || ( a.m_Right != m_Right ) || ( a.m_Bottom != m_Bottom ); };

	PixelRect const Scale( PIXEL const pix )  // positive values of pix enlarge rectangle
	{                                         // negative values reduce its size
		return PixelRect
		{
			m_Left   - pix,
			m_Top    - pix,
			m_Right  + pix,
			m_Bottom + pix
		};
	}

	PixelRect const operator+= ( PixelPoint const offset )
	{ 
		m_Left   += offset.GetX();
		m_Top    += offset.GetY();
		m_Right  += offset.GetX();
		m_Bottom += offset.GetY();
		return *this;
	}

	PixelRect const operator-= ( PixelPoint const offset )
	{ 
		m_Left   -= offset.GetX();
		m_Top    -= offset.GetY();
		m_Right  -= offset.GetX();
		m_Bottom -= offset.GetY();
		return *this;
	}

private:
    PIXEL m_Left;
    PIXEL m_Top;
    PIXEL m_Right;
    PIXEL m_Bottom;
};

inline PixelRect const operator+ (PixelRect const & r, PixelPoint const p) { PixelRect res(r); res += p; return res; };
inline PixelRect const operator- (PixelRect const & r, PixelPoint const p) { PixelRect res(r); res -= p; return res; };
