// pixelRectSize.h
//

#pragma once

#include <assert.h>
#include "pixelPoint.h"

class PixelRectSize
{
public:
	PixelRectSize( ) : m_pixWidth( 0 ), m_pixHeight( 0 ) {};
    PixelRectSize( PIXEL const pixSideLength ) : m_pixWidth( pixSideLength ), m_pixHeight( pixSideLength ) {};
    PixelRectSize( PIXEL const pixWidth, PIXEL const pixHeight ) : m_pixWidth(pixWidth), m_pixHeight(pixHeight) {};

    PixelRectSize const operator*= (long const l) { m_pixWidth *= l; m_pixHeight *= l; return *this; };
    PixelRectSize const operator/= (long const l) { m_pixWidth /= l; m_pixHeight /= l; return *this; };

    PixelRectSize const operator+= (PIXEL const p) { m_pixWidth += p; m_pixHeight += p; return *this; };
    PixelRectSize const operator-= (PIXEL const p) { m_pixWidth -= p; m_pixHeight -= p; return *this; };

    //PixelRectSize const operator*= (PixelRectSize const a) { m_pixWidth *= a.m_pixWidth; m_pixHeight *= a.m_pixHeight; return *this; };
    //PixelRectSize const operator/= (PixelRectSize const a) { m_pixWidth /= a.m_pixWidth; m_pixHeight /= a.m_pixHeight; return *this; };

    PIXEL GetWidth ( ) const { return m_pixWidth;  }
    PIXEL GetHeight( ) const { return m_pixHeight; }

	PixelPoint ToPixelPoint() const { return PixelPoint( m_pixWidth, m_pixHeight ); }

    void ReduceHeight( PIXEL const pixDiff ) 
    {
        assert( m_pixHeight >= pixDiff );
        m_pixHeight -= pixDiff;
    }

	void SetHeight( PIXEL const pixHeight )
	{
		m_pixHeight = pixHeight;
	}

    void SetEmpty( )
    {
        m_pixWidth  = PIXEL(0_PIXEL);
		m_pixHeight = PIXEL(0_PIXEL);
    }

    bool IsEmpty( ) const
    {
        return ( m_pixWidth == PIXEL(0_PIXEL) ) || ( m_pixHeight == PIXEL(0_PIXEL) );
    }

    bool Includes( PixelRectSize const size ) const
    {
        return ( m_pixWidth >= size.m_pixWidth ) && ( m_pixHeight >= size.m_pixHeight );
    }

private:
    PIXEL m_pixWidth;
    PIXEL m_pixHeight;
};

inline PixelRectSize const operator* (PixelRectSize const & a, long const l) { PixelRectSize res(a); res *= l; return res; };
inline PixelRectSize const operator/ (PixelRectSize const & a, long const l) { PixelRectSize res(a); res /= l; return res; };

inline PixelRectSize const operator+ (PixelRectSize const & a, PIXEL const p ) { PixelRectSize res(a); res += p; return res; };
inline PixelRectSize const operator- (PixelRectSize const & a, PIXEL const p ) { PixelRectSize res(a); res -= p; return res; };
