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
        m_pixLeft  (PIXEL(0_PIXEL)), 
        m_pixTop   (PIXEL(0_PIXEL)), 
        m_pixRight (PIXEL(0_PIXEL)), 
        m_pixBottom(PIXEL(0_PIXEL))
    {};

    PixelRect( PixelRect const & rect ) :
        m_pixLeft  (rect.m_pixLeft),
        m_pixBottom(rect.m_pixBottom),
        m_pixRight (rect.m_pixRight),
        m_pixTop   (rect.m_pixTop)
    {
		assert( m_pixBottom >= m_pixTop );
    };

    PixelRect( PixelPoint const pt1, PixelPoint const pt2 )
    { 
        if ( pt1.GetX() < pt2.GetX() )
        {
            m_pixLeft = pt1.GetX();
            m_pixRight = pt2.GetX();
        }
        else
        {
            m_pixLeft = pt2.GetX();
            m_pixRight = pt1.GetX();
        }
        if ( pt1.GetY() > pt2.GetY() )
        {
            m_pixBottom = pt1.GetY();
            m_pixTop = pt2.GetY();
        }
        else
        {
            m_pixBottom = pt2.GetY();
            m_pixTop = pt1.GetY();
        }
		assert( m_pixBottom >= m_pixTop );
    };

    PixelRect( PIXEL const pixLeft, PIXEL const pixTop, PIXEL const pixRight, PIXEL const pixBottom ) :
        m_pixLeft  (pixLeft), 
        m_pixTop   (pixTop), 
        m_pixRight (pixRight), 
        m_pixBottom(pixBottom)
    { 
		assert( m_pixBottom >= m_pixTop );
    };

	PixelRect( PixelPoint const ptOrigin, PixelRectSize const & rectSize ) :
        m_pixLeft  (ptOrigin.GetX()),
        m_pixTop   (ptOrigin.GetY()),
        m_pixRight (m_pixLeft + rectSize.GetWidth()  - PIXEL(1_PIXEL)),
        m_pixBottom(m_pixTop  + rectSize.GetHeight() - PIXEL(1_PIXEL))
	{
		assert( m_pixBottom >= m_pixTop );
	}

    void SetEmpty( )
    {
        m_pixLeft   = PIXEL(0_PIXEL);
        m_pixTop    = PIXEL(0_PIXEL);
        m_pixRight  = PIXEL(0_PIXEL);
        m_pixBottom = PIXEL(0_PIXEL);
    };

    bool IsEmpty( ) const
    {
        return (m_pixLeft == m_pixRight) || (m_pixTop == m_pixBottom);
    };

    bool IsNotEmpty( ) const
    {
        return (m_pixLeft < m_pixRight) && (m_pixTop < m_pixBottom );
    };

    PIXEL const GetLeft( ) const 
    {
        return m_pixLeft;
    }

    PIXEL const GetRight( ) const 
    {
        return m_pixRight;
    }

    PIXEL const GetTop( ) const 
    {
        return m_pixTop;
    }

    PIXEL const GetBottom( ) const 
    {
        return m_pixBottom;
    }

    PIXEL const GetWidth( ) const 
    {
        return m_pixRight - m_pixLeft + PIXEL(1_PIXEL);
    }

    PIXEL const GetHeight( ) const 
    {
        return m_pixBottom - m_pixTop + PIXEL(1_PIXEL);
    }

    PixelRectSize const GetSize( ) const 
    {
        return PixelRectSize( GetWidth(), GetHeight() );
    }

    PixelPoint const GetStartPoint( ) const 
    {
        return PixelPoint( m_pixLeft, m_pixTop );
    }

    PixelPoint const GetEndPoint( ) const
    {
        return PixelPoint( m_pixRight, m_pixBottom );
    }

    PixelPoint const GetCenter( ) const
    {
        return ( GetStartPoint() + GetEndPoint() ) / 2;
    }

	bool Includes( PixelPoint const pnt ) const
	{
		return (m_pixLeft <= pnt.GetX()) && (pnt.GetX() < m_pixRight) && (m_pixTop <= pnt.GetY()) && (pnt.GetY() < m_pixBottom);
	}

	bool Includes( PixelRectSize const size ) const
	{
		return ( GetWidth() >= size.GetWidth() ) && ( GetHeight() >= size.GetHeight() );
	}

	bool const operator== ( PixelRect const & a ) const { return ( a.m_pixLeft == m_pixLeft ) && ( a.m_pixTop == m_pixTop ) && ( a.m_pixRight == m_pixRight ) && ( a.m_pixBottom == m_pixBottom ); };
    bool const operator!= ( PixelRect const & a ) const { return ( a.m_pixLeft != m_pixLeft ) || ( a.m_pixTop != m_pixTop ) || ( a.m_pixRight != m_pixRight ) || ( a.m_pixBottom != m_pixBottom ); };

	PixelRect const Scale( PIXEL const pix )  // positive values of pix enlarge rectangle
	{                                         // negative values reduce its size
		return PixelRect
		{
			m_pixLeft   - pix,
			m_pixTop    - pix,
			m_pixRight  + pix,
			m_pixBottom + pix
		};
	}

	PixelRect const operator+= ( PixelPoint const offset )
	{ 
		m_pixLeft   += offset.GetX();
		m_pixTop    += offset.GetY();
		m_pixRight  += offset.GetX();
		m_pixBottom += offset.GetY();
		return *this;
	}

	PixelRect const operator-= ( PixelPoint const offset )
	{ 
		m_pixLeft   -= offset.GetX();
		m_pixTop    -= offset.GetY();
		m_pixRight  -= offset.GetX();
		m_pixBottom -= offset.GetY();
		return *this;
	}

private:
    PIXEL m_pixLeft;
    PIXEL m_pixTop;
    PIXEL m_pixRight;
    PIXEL m_pixBottom;
};

inline PixelRect const operator+ (PixelRect const & r, PixelPoint const p) { PixelRect res(r); res += p; return res; };
inline PixelRect const operator- (PixelRect const & r, PixelPoint const p) { PixelRect res(r); res -= p; return res; };
