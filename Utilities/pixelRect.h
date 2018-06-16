// pixelRect.h
//

#pragma once

#include <assert.h>
#include "pixelPoint.h"

class PixelRect
{
public:
    long left;
    long top;
    long right;
    long bottom;

    PixelRect( )
    {
        left = 0;
        top = 0;
        right = 0;
        bottom = 0;
    };

    PixelRect( RECT const & rect )
    {
        left   = rect.left;
        top    = rect.top;
        right  = rect.right;
        bottom = rect.bottom;
    };

    PixelRect( PixelPoint const & pt1, PixelPoint const & pt2 )
    { 
        if ( pt1.x < pt2.x )
        {
            left = pt1.x;
            right = pt2.x;
        }
        else
        {
            left = pt2.x;
            right = pt1.x;
        }
        if ( pt1.y < pt2.y )
        {
            top = pt1.y;
            bottom = pt2.y;
        }
        else
        {
            top = pt2.y;
            bottom = pt1.y;
        }
    };

    PixelRect( long const lLeft, long const lTop, long const lRight, long const lBottom ) 
    { 
        left   = lLeft; 
        top    = lTop; 
        right  = lRight; 
        bottom = lBottom; 
    };

    PixelPoint const GetStartPoint( ) const 
    {
        return PixelPoint( left, top );
    }

    PixelPoint const GetEndPoint( ) const
    {
        return PixelPoint( right, bottom );
    }

    PixelPoint const GetCenter( ) const
    {
        return PixelPoint( (left + right) / 2, (top + bottom) / 2 );
    }

    bool const operator== ( PixelRect const & a ) const { return ( a.left == left ) && ( a.top == top ) && ( a.right == right ) && ( a.bottom == bottom ); };
    bool const operator!= ( PixelRect const & a ) const { return ( a.left != left ) || ( a.top != top ) || ( a.right != right ) || ( a.bottom != bottom ); };
};

class PixelRectSize
{
public:
    PixelRectSize( int const iWidth, int const iHeight ) : m_iWidth(iWidth), m_iHeight(iHeight) {};

    int GetWidth ( ) const { return m_iWidth;  }
    int GetHeight( ) const { return m_iHeight; }

    void ReduceHeight( int const iDiff ) 
    {
        assert( m_iHeight >= iDiff );
        m_iHeight -= iDiff;
    }

    BOOL IsEmpty( ) const
    {
        return ( m_iWidth == 0 ) || ( m_iHeight == 0 );
    }

private:
    int m_iWidth;
    int m_iHeight;
};

