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

    RECT ScrReadRECT( Script & );

    bool operator== ( RECT const &, RECT const & );
    bool operator!= ( RECT const &, RECT const & );
    
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
		return RECT{pixRect.m_lLeft, pixRect.m_lTop, pixRect.m_lRight, pixRect.m_lBottom};
	}

	inline PixelRect RECT2PixelRect(RECT rect)
	{
		return PixelRect{rect.left, rect.top, rect.right, rect.bottom};
	}

    inline RECT GetClRect( HWND const hwnd ) // xPos / yPos always 0
    {
        RECT rect;
        (void)GetClientRect( hwnd, &rect );                     
        return rect;
    }

    inline PixelRect GetClPixelRect( HWND const hwnd ) // xPos / yPos always 0
    {
        RECT rect;
        (void)GetClientRect( hwnd, &rect );                     
        return RECT2PixelRect(rect);
    }

    inline PixelRectSize GetClRectSize( HWND const hwnd )
    {
        RECT const rect = GetClRect( hwnd );
        PixelRectSize pntSize
        ( 
            (rect.right  - rect.left),
            (rect.bottom - rect.top) 
        );
        return pntSize;
    }

    inline PixelPoint GetClRectCenter( HWND const hwnd )
    {
        PixelRectSize const rectSize = GetClRectSize( hwnd );
        PixelPoint          pntCenter( rectSize.GetWidth() / 2, rectSize.GetHeight() / 2 );
        return pntCenter;
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

    inline PixelPoint GetClientAreaPos( HWND const hwnd )
    {
		POINT pnt{ 0, 0 };
        (void)ClientToScreen( hwnd, &pnt );
		PixelPoint pp = POINT2PixelPoint( pnt );
        return pp;
    }
	
    inline void UpsideDown( HWND const hwnd, PixelPoint * pnt )   // windows y-coordinates increase from top to bottom
    {                                                             // we use y-coordinates increasing from bottom to top
        pnt->y = GetClientWindowHeight( hwnd ) - pnt->y;          // because of DirectX
    }

    inline PixelPoint GetRelativeCrsrPosition( HWND const hwnd )   // Delivers cursor position relative to client area 
    {
		POINT pnt;
		(void)GetCursorPos( &pnt );
        (void)ScreenToClient( hwnd, &pnt );
		PixelPoint ptCrsr = POINT2PixelPoint( pnt );
        UpsideDown( hwnd, & ptCrsr ); 
        return ptCrsr;
    }

    inline PixelPoint GetWindowPos( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
        return { rect.left, rect.top };
    }

    inline PixelPoint GetWindowSize( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
        return { rect.right  - rect.left, rect.bottom - rect.top };
    }

    inline long GetWindowHeight( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
        return rect.bottom - rect.top;
    }

    inline long GetWindowWidth( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );                     
        return rect.right - rect.left;
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

    inline BOOL PixelPointInClientRect( HWND const hwnd, PixelPoint const pp )  // Is point in client rect?
    {
        RECT const rect = GetClRect( hwnd );  
		return PtInRect( &rect, PixelPoint2POINT( pp ) );
    } 

    inline BOOL CrsrInClientRect( HWND const hwnd )  // Is cursor position in client rect?
    {
		return PixelPointInClientRect( hwnd, GetRelativeCrsrPosition( hwnd )  );
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
