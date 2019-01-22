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
        if ( pt1.x < pt2.x )
        {
            m_pixLeft = pt1.x;
            m_pixRight = pt2.x;
        }
        else
        {
            m_pixLeft = pt2.x;
            m_pixRight = pt1.x;
        }
        if ( pt1.y > pt2.y )
        {
            m_pixBottom = pt1.y;
            m_pixTop = pt2.y;
        }
        else
        {
            m_pixBottom = pt2.y;
            m_pixTop = pt1.y;
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

    PixelRect( long const lLeft, long const lTop, long const lRight, long const lBottom ) :
        m_pixLeft  (lLeft),
        m_pixTop   (lTop),
        m_pixRight (lRight), 
        m_pixBottom(lBottom) 
    { 
		assert( m_pixBottom >= m_pixTop );
    };

	PixelRect( PixelPoint const ptOrigin, PixelRectSize const & rectSize ) :
        m_pixLeft  (ptOrigin.x),
        m_pixTop   (ptOrigin.y),
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
		return (m_pixLeft <= pnt.x) && (pnt.x < m_pixRight) && (m_pixTop <= pnt.y) && (pnt.y < m_pixBottom);
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
		m_pixLeft   += offset.x;
		m_pixTop    += offset.y;
		m_pixRight  += offset.x;
		m_pixBottom += offset.y;
		return *this;
	}

	PixelRect const operator-= ( PixelPoint const offset )
	{ 
		m_pixLeft   -= offset.x;
		m_pixTop    -= offset.y;
		m_pixRight  -= offset.x;
		m_pixBottom -= offset.y;
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
