// win32_util.h : 
//

#pragma once

#include "windows.h"
#include "NamedType.h"
#include "PixelTypes.h"
#include "BoolOp.h"

#include <string>
#include <iostream>

class Script;

namespace Util
{
    std::wostream & operator << ( std::wostream &, RECT const & );

    RECT ScrReadRECT( Script & );

	inline bool operator== ( RECT const & a, RECT const & b ) 
	{ 
		return ( a.left == b.left ) && ( a.top == b.top ) && ( a.right == b.right ) && ( a.bottom == b.bottom ); 
	};

	inline bool operator!= ( RECT const & a, RECT const & b ) 
	{ 
		return ( a.left != b.left ) || ( a.top != b.top ) || ( a.right != b.right ) || ( a.bottom != b.bottom ); 
	};

	inline PixelPoint POINT2PixelPoint( POINT const & pnt ) 
	{ 
		return PixelPoint{ PIXEL(pnt.x), PIXEL(pnt.y) }; 
	}

	inline POINT PixelPoint2POINT( PixelPoint const & pp ) 
	{ 
		return POINT{ pp.GetXvalue(), pp.GetYvalue() }; 
	}

	inline PixelRect RECT2PixelRect( RECT const & rect ) 
	{ 
		return PixelRect
		{ 
			PIXEL(rect.left), 
			PIXEL(rect.top), 
			PIXEL(rect.right), 
			PIXEL(rect.bottom) 
		}; 
	}

	inline RECT PixelRect2RECT( PixelRect const & pixRect ) 
	{ 
		return RECT
		{ 
			pixRect.GetLeft  ().GetValue(),	
			pixRect.GetTop   ().GetValue(), 
			pixRect.GetRight ().GetValue(), 
			pixRect.GetBottom().GetValue() 
		}; 
	}

    inline PixelRectSize PixelRectSizeFromRECT( RECT const & rect )
    {
        return PixelRectSize
		{ 
			PIXEL(rect.right - rect.left), 
			PIXEL(rect.bottom - rect.top) 
		};
    }

	inline BOOL MoveWindow( HWND const hwnd, PIXEL const xPos, PIXEL const yPos, PIXEL const width, PIXEL const height, BOOL const bRedraw )
	{
		return ::MoveWindow( hwnd, xPos.GetValue(), yPos.GetValue(), width.GetValue(), height.GetValue(), bRedraw );
	}

	inline void SetText( HWND const hwnd, wchar_t const * const wstrText )
	{
		(void)::SendMessage( hwnd, WM_SETTEXT,	0, (LPARAM)( wstrText )	);
	}

    inline void Show( HWND const hwnd, BOOL const bStateOld, BOOL const bStateNew )
    {
        if ( bStateNew != bStateOld )
            ShowWindow( hwnd, bStateNew ? SW_SHOW : SW_HIDE );
    }

    inline void Show( HWND const hwnd, BOOL const bStateNew )
    {
        Util::Show( hwnd, IsWindowVisible( hwnd ), bStateNew );
    }

    inline BOOL Show( HWND const hwnd, tBoolOp const op )
    {
        BOOL const bStateOld = IsWindowVisible( hwnd );
        Util::Show( hwnd, ApplyOp2( bStateOld, op ) );
		return bStateOld;
    }

    inline RECT GetClRect( HWND const hwnd ) // left / top always 0
    {
        RECT rect;
        (void)GetClientRect( hwnd, &rect );                     
        return rect;
    }

    inline PIXEL GetClientWindowHeight( HWND const hwnd )
    {
        RECT rect = GetClRect( hwnd );                    
		return PIXEL(PIXEL(rect.bottom - rect.top));
    }

    inline PIXEL GetClientWindowWidth( HWND const hwnd )
    {
        RECT rect = GetClRect( hwnd );
		return PIXEL(PIXEL(rect.right - rect.left));
    }

    inline PixelRect GetClPixelRect( HWND const hwnd ) // left / top always 0
    {
		return RECT2PixelRect( GetClRect( hwnd ) ); 
    }

    inline PixelRectSize GetClRectSize( HWND const hwnd )
    {
        return PixelRectSizeFromRECT( GetClRect( hwnd ) );
    }

    inline PixelPoint GetClRectCenter( HWND const hwnd )
    {
        PixelRectSize pixSize = GetClRectSize( hwnd ) / 2;
		return PixelPoint( pixSize.GetX(), pixSize.GetY() );
    }

    inline BOOL IsInClientRect( HWND const hwnd, PixelPoint const pp )  // Is point in client rect?
    {
        RECT const rect = GetClRect( hwnd );  
		return PtInRect( &rect, PixelPoint2POINT( pp ) );
    } 

    inline BOOL IsInClientRect( HWND const hwnd, PixelRect const & pixRect )  // Is rect in client rect?
    {
        RECT const rect = GetClRect( hwnd );  
		return PtInRect( &rect, PixelPoint2POINT( pixRect.GetStartPoint() ) ) && 
			   PtInRect( &rect, PixelPoint2POINT( pixRect.GetEndPoint  () ) );
    } 

	inline PixelPoint Client2Screen( HWND const hwnd, PixelPoint const & pixPoint )
    {
		POINT pnt { PixelPoint2POINT( pixPoint ) };
        (void)ClientToScreen( hwnd, &pnt );
		return POINT2PixelPoint( pnt );
    }
	
    inline PixelPoint const Screen2Client( HWND const hwnd, PixelPoint const & pixPoint )
    {
		POINT pnt { PixelPoint2POINT( pixPoint ) };
        (void)ScreenToClient( hwnd, &pnt );
		return POINT2PixelPoint( pnt );
    }

    inline PixelPoint GetRelativeCrsrPosition( HWND const hwnd )   // Delivers cursor position relative to client area 
    {
		POINT pnt;
		(void)GetCursorPos( &pnt );
		ScreenToClient( hwnd, &pnt );
		return POINT2PixelPoint( pnt );
    }

    inline PixelRectSize GetWindowSize( HWND const hwnd )
    {
        RECT rect;
        BOOL bRes = GetWindowRect( hwnd, &rect );
		assert( bRes );
        return PixelRectSizeFromRECT( rect );
    }

    inline PIXEL GetWindowWidth( HWND const hwnd )
    {
		return GetWindowSize( hwnd ).GetX();
    }

    inline PIXEL GetWindowHeight( HWND const hwnd )
    {
		return GetWindowSize( hwnd ).GetY();
    }

    inline PIXEL GetWindowBottom( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
		return PIXEL(PIXEL(rect.bottom));
    }

    inline PIXEL GetWindowTop( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
		return PIXEL(PIXEL(rect.top));
    }

    inline PIXEL GetWindowLeftPos( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
		return PIXEL(PIXEL(rect.left));
    }

    inline PIXEL GetWindowRightPos( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
		return PIXEL(PIXEL(rect.right));
    }

    inline BOOL CrsrInClientRect( HWND const hwnd )  // Is cursor position in client rect?
    {
		return IsInClientRect( hwnd, GetRelativeCrsrPosition( hwnd )  );
    } 

    inline void FastFill( HDC const hDC, RECT const & rect )
    {
        (void)ExtTextOut( hDC, 0, 0, ETO_OPAQUE, & rect, L"", 0, 0 );
    }

    inline void FastFill( HDC const hDC, PixelRect const & pixRect )
    {
        FastFill( hDC, PixelRect2RECT( pixRect ) );
    }

    inline void FastFill( HDC const hDC, HWND const hwnd )
    {
        Util::FastFill( hDC, GetClRect( hwnd ) );
    }

    inline void AddWindowStyle( HWND const hwnd, DWORD const dwStyle )
    {
        DWORD const dwOldStyle = GetWindowLong( hwnd, GWL_EXSTYLE );
        DWORD const dwNewStyle = dwOldStyle | dwStyle;
        SetWindowLong( hwnd, GWL_EXSTYLE, dwNewStyle );
    }

    inline void DeleteWindowStyle( HWND const hwnd, DWORD const dwStyle )
    {
        DWORD const dwOldStyle = GetWindowLong( hwnd, GWL_EXSTYLE );
        DWORD const dwNewStyle = dwOldStyle & ~dwStyle;
        LONG  const lRes       = SetWindowLong( hwnd, GWL_EXSTYLE, dwNewStyle );
        assert( lRes == dwNewStyle );
    }

    inline MONITORINFO GetMonitorInfo( HMONITOR hMonitor )
    {
        MONITORINFO monInfo;
        monInfo.cbSize = sizeof( MONITORINFO );
        GetMonitorInfo( hMonitor, &monInfo );
        return monInfo;
    }

	inline void StdOutConsole( )
	{
		FILE * fp;
		BOOL    bRes = AllocConsole( );
		errno_t res  = _wfreopen_s( &fp, L"CONOUT$", L"w", stdout );
		std::wcout << L"Console started" << std::endl;
	}

	void MakeLayered( HWND const , BOOL const, COLORREF const, UINT const );

    void AdjustRight( HWND const, PIXEL const );
    void AdjustLeft ( HWND const, PIXEL const );

    PixelRect CalcWindowRect( PixelRect const, DWORD const );

    BOOL MoveWindowAbsolute( HWND const, PixelRect  const &, BOOL const );
    BOOL MoveWindowAbsolute( HWND const, PixelPoint const &, BOOL const );
	
    DWORD        GetNrOfCPUs( );
    ULONGLONG    GetPhysicalMemory( );
	std::wstring GetCurrentDateAndTime( );
	void         SetApplicationTitle( HWND const, int const );
};
