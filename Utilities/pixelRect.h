// pixelRect.h
//
// pixel coordinates
// origin is bottom left

#pragma once

#include <assert.h>
#include "pixelPoint.h"
#include "pixelRectSize.h"

class PixelRect
{
public:
    long m_lLeft;
    long m_lTop;
    long m_lRight;
    long m_lBottom;

    PixelRect( )
    {
		SetEmpty( );
    };

    PixelRect( PixelRect const & rect )
    {
        m_lLeft   = rect.m_lLeft;
        m_lBottom = rect.m_lBottom;
        m_lRight  = rect.m_lRight;
        m_lTop    = rect.m_lTop;
		assert( m_lBottom <= m_lTop );
    };

    PixelRect( PixelPoint const pt1, PixelPoint const pt2 )
    { 
        if ( pt1.x < pt2.x )
        {
            m_lLeft = pt1.x;
            m_lRight = pt2.x;
        }
        else
        {
            m_lLeft = pt2.x;
            m_lRight = pt1.x;
        }
        if ( pt1.y < pt2.y )
        {
            m_lBottom = pt1.y;
            m_lTop = pt2.y;
        }
        else
        {
            m_lBottom = pt2.y;
            m_lTop = pt1.y;
        }
		assert( m_lBottom <= m_lTop );
    };

    PixelRect( long const lLeft, long const lBottom, long const lRight, long const lTop ) 
    { 
        m_lLeft   = lLeft; 
        m_lBottom = lBottom; 
        m_lRight  = lRight; 
        m_lTop    = lTop; 
		assert( m_lBottom <= m_lTop );
    };

	PixelRect( PixelPoint const ptOrigin, PixelRectSize const & rectSize )
	{
        m_lLeft   = ptOrigin.x; 
        m_lBottom = ptOrigin.y; 
        m_lRight  = m_lLeft   + rectSize.GetWidth(); 
        m_lTop    = m_lBottom + rectSize.GetHeight(); 
		assert( m_lBottom <= m_lTop );
	}

    void SetEmpty( )
    {
        m_lLeft   = 0;
        m_lTop    = 0;
        m_lRight  = 0;
        m_lBottom = 0;
    };

    bool IsEmpty( ) const
    {
        return (m_lLeft == m_lRight) || (m_lTop == m_lBottom);
    };

    bool IsNotEmpty( ) const
    {
        return (m_lLeft < m_lRight) && (m_lBottom < m_lTop);
    };

    long const GetWidth( ) const 
    {
        return m_lRight - m_lLeft + 1;
    }

    long const GetHeight( ) const 
    {
        return m_lTop - m_lBottom + 1;
    }

    PixelRectSize const GetSize( ) const 
    {
        return PixelRectSize( GetWidth(), GetHeight() );
    }

    PixelPoint const GetStartPoint( ) const 
    {
        return PixelPoint( m_lLeft, m_lBottom );
    }

    PixelPoint const GetEndPoint( ) const
    {
        return PixelPoint( m_lRight, m_lTop );
    }

    PixelPoint const GetCenter( ) const
    {
        return ( GetStartPoint() + GetEndPoint() ) / 2;
    }

	bool Includes( PixelPoint const pnt ) const
	{
		return (m_lLeft <= pnt.x) && (pnt.x < m_lRight) && (m_lBottom <= pnt.y) && (pnt.y < m_lTop);
	}

	bool const operator== ( PixelRect const & a ) const { return ( a.m_lLeft == m_lLeft ) && ( a.m_lTop == m_lTop ) && ( a.m_lRight == m_lRight ) && ( a.m_lBottom == m_lBottom ); };
    bool const operator!= ( PixelRect const & a ) const { return ( a.m_lLeft != m_lLeft ) || ( a.m_lTop != m_lTop ) || ( a.m_lRight != m_lRight ) || ( a.m_lBottom != m_lBottom ); };

	PixelRect const operator+= ( PixelPoint const offset )
	{ 
		m_lLeft   += offset.x;
		m_lBottom += offset.y;
		m_lRight  += offset.x;
		m_lTop    += offset.y;
		return *this;
	}

	PixelRect const operator-= ( PixelPoint const offset )
	{ 
		m_lLeft   -= offset.x;
		m_lBottom -= offset.y;
		m_lRight  -= offset.x;
		m_lTop    -= offset.y;
		return *this;
	}
};

inline PixelRect const operator+ (PixelRect const & r, PixelPoint const p) { PixelRect res(r); res += p; return res; };
inline PixelRect const operator- (PixelRect const & r, PixelPoint const p) { PixelRect res(r); res -= p; return res; };
