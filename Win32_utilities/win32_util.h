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
    wostream & operator << ( wostream &, PixelPoint const & );
    wostream & operator << ( wostream &, PixelRect const & );

    RECT ScrReadRECT( Script & );

    bool operator== ( RECT const &, RECT const & );
    bool operator!= ( RECT const &, RECT const & );
    
    inline void Show( HWND const hWnd, BOOL const bStateOld, BOOL const bStateNew )
    {
        if ( bStateNew != bStateOld )
            ShowWindow( hWnd, bStateNew ? SW_SHOW : SW_HIDE );
    }

    inline void Show( HWND const hWnd, BOOL const bStateNew )
    {
        Util::Show( hWnd, IsWindowVisible( hWnd ), bStateNew );
    }

    inline void Show( HWND const hWnd, tBoolOp const op )
    {
        BOOL const bStateOld = IsWindowVisible( hWnd );
        Util::Show( hWnd, ApplyOp2( bStateOld, op ) );
    }

	inline POINT PixelPoint2POINT(PixelPoint pp)
	{
		return POINT{pp.x, pp.y};
	}

	inline PixelPoint POINT2PixelPoint(POINT pnt)
	{
		return PixelPoint{pnt.x, pnt.y};
	}

	inline RECT PixelRect2RECT(PixelRect pixRect)
	{
		return RECT{pixRect.left, pixRect.top, pixRect.right, pixRect.bottom};
	}

	inline PixelRect RECT2PixelRect(RECT rect)
	{
		return PixelRect{rect.left, rect.top, rect.right, rect.bottom};
	}

    inline RECT GetClRect( HWND const hWnd ) // xPos / yPos always 0
    {
        RECT rect;
        (void)GetClientRect( hWnd, &rect );                     
        return rect;
    }

    inline PixelRect GetClPixelRect( HWND const hWnd ) // xPos / yPos always 0
    {
        RECT rect;
        (void)GetClientRect( hWnd, &rect );                     
        return RECT2PixelRect(rect);
    }

    inline PixelRectSize GetClRectSize( HWND const hWnd )
    {
        RECT const rect = GetClRect( hWnd );
        PixelRectSize pntSize
        ( 
            (rect.right  - rect.left),
            (rect.bottom - rect.top) 
        );
        return pntSize;
    }

    inline PixelPoint GetClRectCenter( HWND const hWnd )
    {
        PixelRectSize const rectSize = GetClRectSize( hWnd );
        PixelPoint          pntCenter( rectSize.GetWidth() / 2, rectSize.GetHeight() / 2 );
        return pntCenter;
    }

    inline long GetClientWindowHeight( HWND const hWnd )
    {
        RECT rect = GetClRect( hWnd );                     
        return rect.bottom - rect.top;
    }

    inline long GetClientWindowWidth( HWND const hWnd )
    {
        RECT rect = GetClRect( hWnd );
        return rect.right - rect.left;
    }

    inline PixelPoint GetClientAreaPos( HWND const hWnd )
    {
		POINT pnt{ 0, 0 };
        (void)ClientToScreen( hWnd, &pnt );
		PixelPoint pp = POINT2PixelPoint( pnt );
        return pp;
    }
	
    inline void UpsideDown( HWND const hWnd, PixelPoint * pnt )   // windows y-coordinates increase from top to bottom
    {                                                             // we use y-coordinates increasing from bottom to top
        pnt->y = GetClientWindowHeight( hWnd ) - pnt->y;          // because of DirectX
    }

    inline PixelPoint GetRelativeCrsrPosition( HWND const hWnd )   // Delivers cursor position relative to client area 
    {
		POINT pnt;
		(void)GetCursorPos( &pnt );
        (void)ScreenToClient( hWnd, &pnt );
		PixelPoint ptCrsr = POINT2PixelPoint( pnt );
        UpsideDown( hWnd, & ptCrsr ); 
        return ptCrsr;
    }

    inline PixelPoint GetWindowPos( HWND const hWnd )
    {
        RECT rect;
        (void)GetWindowRect( hWnd, &rect );
        return { rect.left, rect.top };
    }

    inline PixelPoint GetWindowSize( HWND const hWnd )
    {
        RECT rect;
        (void)GetWindowRect( hWnd, &rect );
        return { rect.right  - rect.left, rect.bottom - rect.top };
    }

    inline long GetWindowHeight( HWND const hWnd )
    {
        RECT rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.bottom - rect.top;
    }

    inline long GetWindowWidth( HWND const hWnd )
    {
        RECT rect;
        (void)GetWindowRect( hWnd, &rect );                     
        return rect.right - rect.left;
    }

    inline long GetWindowBottom( HWND const hWnd )
    {
        RECT rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.bottom;
    }

    inline long GetWindowTop( HWND const hWnd )
    {
        RECT rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.top;
    }

    inline long GetWindowLeftPos( HWND const hWnd )
    {
        RECT rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.left;
    }

    inline long GetWindowRightPos( HWND const hWnd )
    {
        RECT rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.right;
    }

    inline BOOL PixelPointInRect( PixelRect const * const pRect, PixelPoint const pp )
    {
		RECT const rect = PixelRect2RECT( * pRect );
        return PtInRect( &rect, PixelPoint2POINT( pp ) );  
    } 

    inline BOOL PixelPointInClientRect( HWND const hWnd, PixelPoint const pp )  // Is point in client rect?
    {
        RECT const rect = GetClRect( hWnd );  
		return PtInRect( &rect, PixelPoint2POINT( pp ) );
    } 

    inline BOOL CrsrInClientRect( HWND const hWnd )  // Is cursor position in client rect?
    {
		return PixelPointInClientRect( hWnd, GetRelativeCrsrPosition( hWnd )  );
    } 

    inline void FastFill( HDC const hDC, RECT const & rect )
    {
        (void)ExtTextOut( hDC, 0, 0, ETO_OPAQUE, & rect, L"", 0, 0 );
    }

    inline void AddWindowStyle( HWND const hWnd, DWORD const dwStyle )
    {
        DWORD const dwOldStyle = GetWindowLong( hWnd, GWL_EXSTYLE );
        DWORD const dwNewStyle = dwOldStyle | dwStyle;
        SetWindowLong( hWnd, GWL_EXSTYLE, dwNewStyle );
    }

    inline void DeleteWindowStyle( HWND const hWnd, DWORD const dwStyle )
    {
        DWORD const dwOldStyle = GetWindowLong( hWnd, GWL_EXSTYLE );
        DWORD const dwNewStyle = dwOldStyle & ~dwStyle;
        LONG  const lRes       = SetWindowLong( hWnd, GWL_EXSTYLE, dwNewStyle );
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
		BOOL bRes = AllocConsole( );
		_wfreopen_s( &fp, L"CONOUT$", L"w", stdout );
		std::wcout << "console output started" << endl << flush;
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
