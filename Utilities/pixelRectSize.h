// pixelRectSize.h
//

#pragma once

#include <assert.h>
#include "pixelPoint.h"

class PixelRectSize
{
public:
	PixelRectSize( ) : m_iWidth( 0 ), m_iHeight( 0 ) {};
    PixelRectSize( int const iWidth, int const iHeight ) : m_iWidth(iWidth), m_iHeight(iHeight) {};

    PixelRectSize const operator/= (long const l) { m_iWidth /= l; m_iHeight /= l; return *this; };

    int GetWidth ( ) const { return m_iWidth;  }
    int GetHeight( ) const { return m_iHeight; }

    void ReduceHeight( int const iDiff ) 
    {
        assert( m_iHeight >= iDiff );
        m_iHeight -= iDiff;
    }

	void SetHeight(int const iHeight)
	{
		m_iHeight = iHeight;
	}

    bool IsEmpty( ) const
    {
        return ( m_iWidth == 0 ) || ( m_iHeight == 0 );
    }

private:
    int m_iWidth;
    int m_iHeight;
};

inline PixelRectSize const operator/ (PixelRectSize const & a, long const l) { PixelRectSize res(a); res /= l; return res; };
