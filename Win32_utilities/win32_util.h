// win32_util.h : 
//

#pragma once

#include "pixelPoint.h"
#include "pixelRect.h"

#include <iostream>

using namespace std;

class Script;

enum class tBoolOp { opTrue, opFalse, opToggle, opNoChange };

namespace Util
{
    wostream & operator << ( wostream &, PixelPoint const & );
    wostream & operator << ( wostream &, PixelRect const & );

    RECT      ScrReadRECT     ( Script & );
    PixelRect ScrReadPixelRect( Script & );

    bool operator== ( RECT const &, RECT const & );
    bool operator!= ( RECT const &, RECT const & );

    inline BOOL ApplyOp2( BOOL const b, tBoolOp const op )
    {
        switch ( op )
        {
		case tBoolOp::opTrue:
            return TRUE;

        case tBoolOp::opFalse:
            return FALSE;

        case tBoolOp::opToggle:
            return ! b;

        case tBoolOp::opNoChange: 
            return b;

        default:
            assert( FALSE );
            return FALSE;
        }
    }

    inline void ApplyOp( BOOL & b, tBoolOp const op )
    {
        b = Util::ApplyOp2( b, op );
    }
    
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

    inline PixelRect GetClRect( HWND const hWnd ) // xPos / yPos always 0
    {
        PixelRect rect;
        (void)GetClientRect( hWnd, &rect );                     
        return rect;
    }

    inline PixelRectSize GetClRectSize( HWND const hWnd )
    {
        PixelRect  const rect = Util::GetClRect( hWnd );
        PixelRectSize    pntSize
        ( 
            (rect.right  - rect.left),
            (rect.bottom - rect.top) 
        );
        return pntSize;
    }

    inline PixelPoint GetClRectCenter( HWND const hWnd )
    {
        PixelRectSize const rectSize = Util::GetClRectSize( hWnd );
        PixelPoint          pntCenter( rectSize.GetWidth() / 2, rectSize.GetHeight() / 2 );
        return pntCenter;
    }

    inline long GetClientWindowHeight( HWND const hWnd )
    {
        PixelRect rect = GetClRect( hWnd );                     
        return rect.bottom - rect.top;
    }

    inline long GetClientWindowWidth( HWND const hWnd )
    {
        PixelRect rect = GetClRect( hWnd );
        return rect.right - rect.left;
    }

    inline PixelPoint GetClientAreaPos( HWND const hWnd )
    {
        PixelPoint pnt( 0, 0 );
        (void)ClientToScreen( hWnd, &pnt );
        return pnt;
    }

    inline PixelPoint GetCrsrPosition( )   // Delivers cursor position in screen coordinates
    {
        PixelPoint ptCrsr;
        (void)GetCursorPos( &ptCrsr );
        return ptCrsr;
    }

    inline PixelPoint GetRelativeCrsrPosition( HWND const hWnd )   // Delivers cursor position relative to client area 
    {
        PixelPoint ptCrsr = GetCrsrPosition( );               // In screen coordinates
        (void)ScreenToClient( hWnd, &ptCrsr );
        ptCrsr.y = GetClientWindowHeight( hWnd ) - ptCrsr.y;
        return ptCrsr;
    }

    inline long GetRelativeCrsrPositionX( HWND const hWnd )   // Delivers x value of cursor position relative to client area 
    {
        PixelPoint ptCrsr = GetCrsrPosition( );               // In screen coordinates
        (void)ScreenToClient( hWnd, &ptCrsr );
        return ptCrsr.x;
    }

    inline PixelPoint GetWindowPos( HWND const hWnd )
    {
        PixelRect rect;
        (void)GetWindowRect( hWnd, &rect );
        return { rect.left, rect.top };
    }

    inline long GetWindowHeight( HWND const hWnd )
    {
        PixelRect rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.bottom - rect.top;
    }

    inline long GetWindowWidth( HWND const hWnd )
    {
        PixelRect rect;
        (void)GetWindowRect( hWnd, &rect );                     
        return rect.right - rect.left;
    }

    inline long GetWindowBottom( HWND const hWnd )
    {
        PixelRect rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.bottom;
    }

    inline long GetWindowTop( HWND const hWnd )
    {
        PixelRect rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.top;
    }

    inline long GetWindowLeftPos( HWND const hWnd )
    {
        PixelRect rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.left;
    }

    inline long GetWindowRightPos( HWND const hWnd )
    {
        PixelRect rect;
        (void)GetWindowRect( hWnd, &rect );
        return rect.right;
    }

    inline BOOL CrsrInClientRect( HWND const hWnd )
    {
        PixelRect  const rect   = GetClRect( hWnd );                     
        PixelPoint const ptCrsr = GetRelativeCrsrPosition( hWnd );
        return PtInRect( &rect, ptCrsr );  // Is cursor position in client rect?
    } 


    inline void FastFill( HDC const hDC, RECT const & rect )
    {
        (void)ExtTextOut( hDC, 0, 0, ETO_OPAQUE, &rect, L"", 0, 0 );
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
		freopen_s( &fp, "CONOUT$", "w", stdout );
	}

void MakeLayered( HWND const , BOOL const, COLORREF const, UINT const );

    void AdjustRight( HWND, int );
    void AdjustLeft( HWND, int );

    HANDLE    MakeThread( LPTHREAD_START_ROUTINE, LPVOID, LPDWORD, HANDLE * );
    DWORD     GetNrOfCPUs( );
    ULONGLONG GetPhysicalMemory( );
	LONG      GetMaxNrOfSlots( ULONG );
};



