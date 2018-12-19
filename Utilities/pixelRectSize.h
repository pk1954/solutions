// pixelRectSize.h
//

#pragma once

#include <assert.h>
#include "pixelPoint.h"

class PixelRectSize
{
public:
	PixelRectSize( ) : m_iWidth( 0 ), m_iHeight( 0 ) {};
    PixelRectSize( int const iSideLength ) : m_iWidth( iSideLength ), m_iHeight( iSideLength ) {};
    PixelRectSize( int const iWidth, int const iHeight ) : m_iWidth(iWidth), m_iHeight(iHeight) {};

    PixelRectSize const operator+= (long const l) { m_iWidth += l; m_iHeight += l; return *this; };
    PixelRectSize const operator-= (long const l) { m_iWidth -= l; m_iHeight -= l; return *this; };

    PixelRectSize const operator*= (long const l) { m_iWidth *= l; m_iHeight *= l; return *this; };
    PixelRectSize const operator/= (long const l) { m_iWidth /= l; m_iHeight /= l; return *this; };

    PixelRectSize const operator*= (PixelRectSize const a) { m_iWidth *= a.m_iWidth; m_iHeight *= a.m_iHeight; return *this; };
    PixelRectSize const operator/= (PixelRectSize const a) { m_iWidth /= a.m_iWidth; m_iHeight /= a.m_iHeight; return *this; };

    int GetWidth ( ) const { return m_iWidth;  }
    int GetHeight( ) const { return m_iHeight; }

	PixelPoint ToPixelPoint() const { return PixelPoint( m_iWidth, m_iHeight ); }

    void ReduceHeight( int const iDiff ) 
    {
        assert( m_iHeight >= iDiff );
        m_iHeight -= iDiff;
    }

	void SetHeight( int const iHeight )
	{
		m_iHeight = iHeight;
	}

    void SetEmpty( )
    {
        m_iWidth  = 0;
		m_iHeight = 0;
    }

    bool IsEmpty( ) const
    {
        return ( m_iWidth == 0 ) || ( m_iHeight == 0 );
    }

    bool Includes( PixelRectSize const size ) const
    {
        return ( m_iWidth >= size.m_iWidth ) && ( m_iHeight >= size.m_iHeight );
    }

private:
    int m_iWidth;
    int m_iHeight;
};

inline PixelRectSize const operator+ (PixelRectSize const & a, long const l) { PixelRectSize res(a); res += l; return res; };
inline PixelRectSize const operator- (PixelRectSize const & a, long const l) { PixelRectSize res(a); res -= l; return res; };

inline PixelRectSize const operator* (PixelRectSize const & a, long const l) { PixelRectSize res(a); res *= l; return res; };
inline PixelRectSize const operator/ (PixelRectSize const & a, long const l) { PixelRectSize res(a); res /= l; return res; };

inline PixelRectSize const operator* (PixelRectSize const & a, PixelRectSize const & b) { PixelRectSize res(a); res *= b; return res; };
inline PixelRectSize const operator/ (PixelRectSize const & a, PixelRectSize const & b) { PixelRectSize res(a); res /= b; return res; };
