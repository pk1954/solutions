// win32_util.cpp :
//

#include "stdafx.h"
#include <iomanip>
#include <time.h>
#include <codecvt>
#include <sstream>
#include "script.h"
#include "win32_util.h"

RECT Util::ScrReadRECT( Script & script )
{
    RECT rect;
    rect.left   = script.ScrReadLong( );
    rect.top    = script.ScrReadLong( );
    rect.right  = script.ScrReadLong( );
    rect.bottom = script.ScrReadLong( );
    return rect;
}

wostream & Util::operator << ( wostream & out, PixelPoint const pp )
{
    //lint -e747  Significant prototype coercion with setw
    out << pp.GetX() << pp.GetY() << L" ";
    return out;
}

wostream & Util::operator << ( wostream & out, PixelRect const & rect )
{
    out << rect.GetStartPoint( ) << L' ' << rect.GetEndPoint( );
    return out;
}

wostream & Util::operator << ( wostream & out, RECT const & rect )
{
    out << rect.left << L' ' << rect.top << L' ' << rect.right << L' ' << rect.bottom;
    return out;
}

bool Util::operator== ( RECT const & a, RECT const & b ) 
{ 
    return ( a.left == b.left ) && ( a.top == b.top ) && ( a.right == b.right ) && ( a.bottom == b.bottom ); 
};

bool Util::operator!= ( RECT const & a, RECT const & b ) 
{ 
    return ( a.left != b.left ) || ( a.top != b.top ) || ( a.right != b.right ) || ( a.bottom != b.bottom ); 
};

// CalcWindowRect: Calculates the required size of the window rectangle, based on the desired client-rectangle size. 

PixelRect Util::CalcWindowRect( PixelRect pixRect, DWORD const dwStyle )
{
	RECT rect = Util::PixelRect2RECT( pixRect );
	(void)AdjustWindowRect( &rect, dwStyle, FALSE );	
	pixRect = Util::RECT2PixelRect( rect );
	return pixRect;
}

void Util::AdjustRight( HWND const hwnd, PIXEL const pixYpos )
{
    HWND  const hwndParent     = GetParent( hwnd );
    PIXEL const pixWidthParent = GetClientWindowWidth( hwndParent );
    PIXEL const pixWidth       = GetWindowWidth( hwnd );
    PIXEL const pixHeight      = GetWindowHeight( hwnd );
    MoveWindow( hwnd, (pixWidthParent - pixWidth).GetValue(), pixYpos.GetValue(), pixWidth.GetValue(), pixHeight.GetValue(), TRUE );
    (void)BringWindowToTop( hwnd );
}

void Util::AdjustLeft( HWND const hwnd, PIXEL const pixYpos )
{
	PixelPoint pnt = GetWindowSize( hwnd );
    MoveWindow( hwnd, 0, pixYpos.GetValue(), pnt.GetXlong(), pnt.GetYlong(), TRUE );
    (void)BringWindowToTop( hwnd );
}

BOOL Util::MoveWindowAbsolute  // move window to given screen coordinates and set size
(
	HWND  const hwnd,
	PIXEL const pixXpos,
	PIXEL const pixYpos,
	PIXEL const pixWidth,
	PIXEL const pixHeight,
	BOOL  const bRepaint
)
{
	HWND  const hwndParent = GetAncestor( hwnd, GA_PARENT );
	POINT       pos{ pixXpos.GetValue(), pixYpos.GetValue() };

	if ( hwndParent != nullptr )
		ScreenToClient( hwndParent, &pos );

	BOOL bRes = MoveWindow( hwnd, pos.x, pos.y, pixWidth.GetValue(), pixHeight.GetValue(), bRepaint );
	
	if ( GetWindowSize( hwnd ) != PixelPoint{ pixWidth, pixHeight } )   // can happen in strange situations
		bRes = MoveWindow( hwnd, pos.x, pos.y, pixWidth.GetValue(), pixHeight.GetValue(), bRepaint );

	return bRes;
}

BOOL Util::MoveWindowAbsolute  // move window to given screen coordinates 
(
	HWND  const hwnd,
	PIXEL const pixXpos,
	PIXEL const pixYpos,
	BOOL  const bRepaint
)
{
	PixelPoint const pixActSize = GetWindowSize( hwnd );
	return MoveWindowAbsolute( hwnd, pixXpos, pixYpos, pixActSize.GetX(), pixActSize.GetY(), bRepaint );
}

void Util::MakeLayered( HWND const hwnd, BOOL const bMode, COLORREF const crKey, UINT const uiAlpha )
{
    if ( bMode )
        AddWindowStyle( hwnd, WS_EX_LAYERED );
    else
        DeleteWindowStyle( hwnd, WS_EX_LAYERED );

    BOOL const bRes = SetLayeredWindowAttributes( hwnd, crKey, ( 255 * uiAlpha ) / 100, (crKey == 0) ? LWA_ALPHA : LWA_COLORKEY );
    assert( bRes );
}

//lint -esym( 714, Util::GetNrOfCPUs(void) )      not referenced
DWORD Util::GetNrOfCPUs( void )
{
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo); 
    return siSysInfo.dwNumberOfProcessors; 
}

ULONGLONG Util::GetPhysicalMemory( )  // in bytes
{
    ULONGLONG ramKB;
    (void)GetPhysicallyInstalledSystemMemory( &ramKB );   // get physical memory in KB
    return ramKB * 1024;                                  // compute number of bytes
}

LONG Util::GetMaxNrOfSlots( ULONG ulSlotSize )
{
	ULONGLONG const ramBytes        = Util::GetPhysicalMemory( );      // compute number of bytes
    ULONGLONG const ullMaxNrOfSlots = ramBytes / ulSlotSize;               assert( ullMaxNrOfSlots < LONG_MAX );
    LONG      const lMaxNrOfSlots   = static_cast<LONG>( ullMaxNrOfSlots );
	return lMaxNrOfSlots;
}

wstring Util::GetCurrentDateAndTime( )
{
	wstring_convert< std::codecvt_utf8_utf16<wchar_t> > converter;
    struct tm newtime;  
    __time64_t long_time;  
    _time64( & long_time );                                  // Get time as 64-bit integer.  
    errno_t err = _localtime64_s( & newtime, & long_time );  // Convert to local time.  
	stringbuf buf;
	ostream os ( & buf );
	os << put_time( & newtime, "%c" );
	wstring wstrTime = converter.from_bytes( buf.str( ) );
	return wstrTime;
}

void Util::SetApplicationTitle
( 
	HWND const hwndApp, 
	int  const iResource
)
{
    static int const MAX_LOADSTRING = 100;
    static TCHAR szTitle[ MAX_LOADSTRING ];			// Titelleistentext
    (void)LoadString( GetModuleHandle( nullptr ), iResource, szTitle, MAX_LOADSTRING );
    SetWindowText( hwndApp, szTitle );
}
