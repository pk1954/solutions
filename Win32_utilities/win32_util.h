// win32_util.h : 
//

#pragma once

#include "windows.h"
#include "pixel.h"
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
    
	inline PixelPoint POINT2PixelPoint( POINT const pnt ) 
	{ 
		return PixelPoint{ PIXEL(pnt.x), PIXEL(pnt.y) }; 
	}

	inline POINT PixelPoint2POINT( PixelPoint const pp ) 
	{ 
		return POINT{ GetXvalue(pp), GetYvalue(pp) }; 
	}

	inline PixelRect RECT2PixelRect( RECT const & rect ) 
	{ 
		return PixelRect{ PIXEL(rect.left), PIXEL(rect.top), PIXEL(rect.right), PIXEL(rect.bottom) }; 
	}

	inline RECT PixelRect2RECT( PixelRect const & pixRect ) 
	{ 
		return RECT
		{ 
			pixRect.GetLeft().GetValue(),	
			pixRect.GetTop().GetValue(), 
			pixRect.GetRight().GetValue(), 
			pixRect.GetBottom().GetValue() 
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

    inline PIXEL GetClientWindowHeight( HWND const hwnd )
    {
        RECT rect = GetClRect( hwnd );                    
		return PIXEL{rect.bottom - rect.top};    
    }

    inline PIXEL GetClientWindowWidth( HWND const hwnd )
    {
        RECT rect = GetClRect( hwnd );
		return PIXEL{rect.right - rect.left};
    }

    inline PixelRect GetClPixelRect( HWND const hwnd ) // left / top always 0
    {
        RECT rect;
        (void)GetClientRect( hwnd, &rect );
		return RECT2PixelRect( rect ); 
    }

    inline PixelRectSize GetClRectSize( HWND const hwnd )
    {
        RECT const rect = GetClRect( hwnd );
		return PixelRectSize{ PIXEL(rect.right), PIXEL(rect.bottom) };
    }

    inline PixelPoint GetClRectCenter( HWND const hwnd )
    {
        return (GetClRectSize( hwnd ) / 2).ToPixelPoint();
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

	inline PixelPoint Client2Screen( HWND const hwnd, POINT pnt )
    {
        (void)ClientToScreen( hwnd, &pnt );
		return PixelPoint{ PIXEL(pnt.x), PIXEL(pnt.y) };
    }
	
    inline PixelPoint const ScreenToClient( HWND const hwnd, POINT pnt )
    {
        (void)ScreenToClient( hwnd, &pnt );
		return PixelPoint{ PIXEL(pnt.x), PIXEL(pnt.y) };
    }

    inline void ScreenToClient( HWND const hwnd, PixelPoint & pixPoint )
    {
		pixPoint = ScreenToClient( hwnd, PixelPoint2POINT( pixPoint ) );
    }

    inline PixelPoint GetRelativeCrsrPosition( HWND const hwnd )   // Delivers cursor position relative to client area 
    {
		POINT pnt;
		(void)GetCursorPos( &pnt );
		return ScreenToClient( hwnd, pnt );
    }

    inline PixelPoint GetWindowSize( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
        return PixelPoint{ PIXEL(rect.right - rect.left), PIXEL(rect.bottom - rect.top) };
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
		return PIXEL{rect.bottom};
    }

    inline PIXEL GetWindowTop( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
		return PIXEL{rect.top};
    }

    inline PIXEL GetWindowLeftPos( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
		return PIXEL{rect.left};
    }

    inline PIXEL GetWindowRightPos( HWND const hwnd )
    {
        RECT rect;
        (void)GetWindowRect( hwnd, &rect );
		return PIXEL{rect.right};
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

    void AdjustRight( HWND const, PIXEL const );
    void AdjustLeft ( HWND const, PIXEL const );

    PixelRect CalcWindowRect( PixelRect const, DWORD const );

    BOOL MoveWindowAbsolute( HWND const, PIXEL const, PIXEL const, PIXEL const, PIXEL const, BOOL const );
    BOOL MoveWindowAbsolute( HWND const, PIXEL const, PIXEL const,                          BOOL const );
	
    DWORD     GetNrOfCPUs( );
    ULONGLONG GetPhysicalMemory( );
	LONG      GetMaxNrOfSlots( ULONG );
	wstring   GetCurrentDateAndTime( );
	void      SetApplicationTitle( HWND const, int const );
};
