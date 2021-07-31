// win32_util.cpp :
//

#include "stdafx.h"
#include <iomanip>
#include <time.h>
#include <sstream>
#include "script.h"
#include "win32_util.h"

using std::wostream;
using std::stringbuf;
using std::ostream;

RECT Util::ScrReadRECT( Script & script )
{
    RECT rect;
    rect.left   = script.ScrReadLong();
    rect.top    = script.ScrReadLong();
    rect.right  = script.ScrReadLong();
    rect.bottom = script.ScrReadLong();
    return rect;
}

wostream & Util::operator << ( wostream & out, RECT const & rect )
{
    out << rect.left << L' ' << rect.top << L' ' << rect.right << L' ' << rect.bottom;
    return out;
}

// CalcWindowRect: Calculates the required size of the window rectangle, based on the desired client-rectangle size. 

PixelRect Util::CalcWindowRect( PixelRect pixRect, DWORD const dwStyle )
{
	RECT rect = Util::PixelRect2RECT( pixRect );
	(void)AdjustWindowRect( &rect, dwStyle, false );	
	pixRect = Util::RECT2PixelRect( rect );
	return pixRect;
}

void Util::AdjustRight( HWND const hwnd, PIXEL const pixYpos )
{
    HWND  const hwndParent     = GetParent( hwnd );
    PIXEL const pixWidthParent = GetClientWindowWidth( hwndParent );
    PIXEL const pixWidth       = GetWindowWidth( hwnd );
    PIXEL const pixHeight      = GetWindowHeight( hwnd );
    MoveWindow( hwnd, (pixWidthParent - pixWidth), pixYpos, pixWidth, pixHeight, true );
    (void)BringWindowToTop( hwnd );
}

void Util::AdjustLeft( HWND const hwnd, PIXEL const pixYpos )
{
	PixelRectSize pnt = GetWindowSize( hwnd );
    MoveWindow( hwnd, 0_PIXEL, pixYpos, pnt.GetX(), pnt.GetY(), true );
    (void)BringWindowToTop( hwnd );
}

bool Util::MoveWindowAbsolute  // move window to given screen coordinates and set size
(
	HWND      const   hwnd,
	PixelRect const & pixRect,
	bool      const   bRepaint
)
{
	HWND const hwndParent { GetAncestor( hwnd, GA_PARENT ) };
	PixelPoint pixPoint{ pixRect.GetStartPoint() };
	bool       bRes;

	if ( hwndParent )
		pixPoint = Screen2Client( hwndParent, pixPoint );

	bRes = MoveWindow( hwnd, pixPoint.GetX(), pixPoint.GetY(), pixRect.GetWidth(), pixRect.GetHeight(), bRepaint );
	
	if ( GetWindowSize( hwnd ) != pixRect.GetSize() )   // can happen in strange situations
		bRes = MoveWindow( hwnd, pixPoint.GetX(), pixPoint.GetY(), pixRect.GetWidth(), pixRect.GetHeight(), bRepaint );

	return bRes;
}

bool Util::MoveWindowAbsolute  // move window to given screen coordinates 
(
	HWND       const   hwnd,
	PixelPoint const & pixPos,
	bool       const   bRepaint
)
{
	return MoveWindowAbsolute( hwnd, PixelRect{ pixPos, GetWindowSize( hwnd ) }, bRepaint );
}

void Util::MakeLayered( HWND const hwnd, bool const bMode, COLORREF const crKey, UINT const uiAlpha )
{
    if ( bMode )
        AddWindowStyle( hwnd, WS_EX_LAYERED );
    else
        DeleteWindowStyle( hwnd, WS_EX_LAYERED );

    bool const bRes = SetLayeredWindowAttributes( hwnd, crKey, ( 255 * uiAlpha ) / 100, (crKey == 0) ? LWA_ALPHA : LWA_COLORKEY );
    assert( bRes );
}

DWORD Util::GetNrOfCPUs( void )
{
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo); 
    return siSysInfo.dwNumberOfProcessors; 
}

ULONGLONG Util::GetPhysicalMemory()  // in bytes
{
    ULONGLONG ramKB;
    (void)GetPhysicallyInstalledSystemMemory( &ramKB );   // get physical memory in KB
    return ramKB * 1024;                                  // compute number of bytes
}

wstring Util::GetCurrentDateAndTime()
{
    __time64_t long_time;  
    _time64( & long_time ); 
    static int const SIZE { 128 };
    wchar_t buffer[SIZE];
    errno_t err = _wctime64_s( buffer, SIZE, & long_time );
    assert( ! err );
    return wstring( buffer );
}

wstring Util::GetComputerName()
{
    static int const SIZE { 128 };
    wchar_t buffer[SIZE];
    DWORD   bufCharCount { SIZE };
    ::GetComputerName( buffer, & bufCharCount );
    return wstring( buffer );
}

wstring Util::GetUserName()
{
    static int const SIZE { 128 };
    wchar_t buffer[SIZE];
    DWORD   bufCharCount { SIZE };
    ::GetUserName( buffer, & bufCharCount );
    return wstring( buffer );
}

void Util::SetApplicationTitle
( 
	HWND    const hwndApp, 
	int     const iResource,
    wstring const wstrAdd
)
{
    static int const MAX_LOADSTRING = 100;
    static TCHAR szTitle[ MAX_LOADSTRING ];			// Titelleistentext
    (void)LoadString( GetModuleHandle( nullptr ), iResource, szTitle, MAX_LOADSTRING );
    wstring newTitle { szTitle };
    if ( wstrAdd != L"" )
        newTitle += L" -" + wstrAdd;
    SetWindowText( hwndApp, newTitle.c_str() );
}

void Util::StdOutConsole()
{
    FILE  * fp;
    bool    bRes = AllocConsole();
    errno_t res  = _wfreopen_s( &fp, L"CONOUT$", L"w", stdout );
    wcout << L"Console started" << endl;
    HWND hwnd = ::GetConsoleWindow();
    if (hwnd != NULL)
    {
        LONG style = GetWindowLong( hwnd , GWL_STYLE );
        style = style & ~(WS_SYSMENU);
        SetWindowLong( hwnd, GWL_STYLE, style );
    }	
}

HMENU Util::PopupMenu(HMENU const hMenuParent, LPCTSTR const text)
{
    HMENU hMenuPopup = CreatePopupMenu();
    AppendMenu( hMenuParent, MF_POPUP, (UINT_PTR)hMenuPopup, text );
    return hMenuPopup;
}
