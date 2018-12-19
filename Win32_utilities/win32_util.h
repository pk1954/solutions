// win32_util.h : 
//

#pragma once

#include "windows.h"
#include "pixelPoint.h"
#include "pixelRect.h"
#include "BoolOp.h"

#include <string>
#include <iostream>

using namespace std;

class Script;

namespace Util
{
    wostream & operator << ( wostream &, PixelPoint const );
    wostream & operator << ( wostream &, PixelRect const & );
    wostream & operator << ( wostream &, RECT const & );

    RECT ScrReadRECT( Script & );

    bool operator== ( RECT const &, RECT const & );
    bool operator!= ( RECT const &, RECT const & );
    
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

    inline void Show( HWND const hwnd, tBoolOp const op )
    {
        BOOL const bStateOld = IsWindowVisible( hwnd );
        Util::Show( hwnd, ApplyOp2( bStateOld, op ) );
    }

    inline RECT GetClRect( HWND const hwnd ) // left / top always 0
    {
        RECT rect;
        (void)GetClientRect( hwnd, &rect );                     
        return rect;
    }

    inline long GetClientWindowHeight( HWND const hwnd )
    {
        RECT rect = GetClRect( hwnd );                    
        return rect.bottom - rect.top;    
    }

    inline long GetClientWindowWidth( HWND const hwnd )
    {
        RECT rect = GetClRect( hwnd );
        return rect.right - rect.left;
    }

    inline PixelRect GetClPixelRect( HWND const hwnd ) // left / top always 0
    {
        RECT rect;
        (void)GetClientRect( hwnd, &rect );
		return PixelRect{ 0, 0,	rect.right,	rect.bottom	};
    }

    inline PixelRectSize GetClRectSize( HWND const hwnd )
    {
        RECT const rect = GetClRect( hwnd );
		return PixelRectSize{ rect.right, rect.bottom };
    }

    inline PixelPoint GetClRectCenter( HWND const hwnd )
    {
        return (GetClRectSize( hwnd ) / 2).ToPixelPoint();
    }

	inline PixelPoint Client2Screen( HWND const hwnd, POINT pnt )
    {
        (void)ClientToScreen( hwnd, &pnt );
		return PixelPoint( pnt.x, pnt.y );
    }
	
    inline PixelPoint GetRelativeCrsrPosition( HWND const hwnd )   // Delivers cursor position relative to client area 
    {
		POINT pnt;
		(void)GetCursorPos( &pnt );
        (void)ScreenToClient( hwnd, &pnt );
		return PixelPoint( pnt.x, pnt.y );
    }

    inline PixelPoint GetWindowSize( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
        return { rect.right  - rect.left, rect.bottom - rect.top };
    }

    inline long GetWindowWidth( HWND const hwnd )
    {
		return GetWindowSize( hwnd ).x;
    }

    inline long GetWindowHeight( HWND const hwnd )
    {
		return GetWindowSize( hwnd ).y;
    }

    inline long GetWindowBottom( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
        return rect.bottom;
    }

    inline long GetWindowTop( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
        return rect.top;
    }

    inline long GetWindowLeftPos( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
        return rect.left;
    }

    inline long GetWindowRightPos( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
        return rect.right;
    }

	inline PixelPoint POINT2PixelPoint( POINT const pnt ) 
	{ 
		return PixelPoint{ pnt.x, pnt.y }; 
	}

	inline POINT PixelPoint2POINT( PixelPoint const pp ) 
	{ 
		return POINT{ pp.x, pp.y }; 
	}

	inline PixelRect RECT2PixelRect( RECT const & rect ) 
	{ 
		return PixelRect{ rect.left, rect.top, rect.right, rect.bottom }; 
	}

	inline RECT PixelRect2RECT( PixelRect const & pixRect ) 
	{ 
		return RECT{ pixRect.m_lLeft,	pixRect.m_lTop, pixRect.m_lRight, pixRect.m_lBottom }; 
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

    inline BOOL CrsrInClientRect( HWND const hwnd )  // Is cursor position in client rect?
    {
		return IsInClientRect( hwnd, GetRelativeCrsrPosition( hwnd )  );
    } 

    inline void FastFill( HDC const hDC, RECT const & rect )
    {
        (void)ExtTextOut( hDC, 0, 0, ETO_OPAQUE, & rect, L"", 0, 0 );
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

	inline HWND StdOutConsole( )
	{
		FILE * fp;
		BOOL bRes = AllocConsole( );
		_wfreopen_s( &fp, L"CONOUT$", L"w", stdout );
		std::wcout << "console output started" << endl << flush;
		return GetConsoleWindow( );
	}

	void MakeLayered( HWND const , BOOL const, COLORREF const, UINT const );

    void AdjustRight( HWND, int );
    void AdjustLeft( HWND, int );

    BOOL MoveWindowAbsolute( HWND const, LONG const, LONG const, LONG const, LONG const, BOOL const );
    BOOL MoveWindowAbsolute( HWND const, LONG const, LONG const,                         BOOL const );
	
    DWORD     GetNrOfCPUs( );
    ULONGLONG GetPhysicalMemory( );
	LONG      GetMaxNrOfSlots( ULONG );
	wstring   GetCurrentDateAndTime( );
	void      SetApplicationTitle( HWND const, int const );
};
