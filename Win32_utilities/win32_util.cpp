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

wostream & Util::operator << ( wostream & out, PixelPoint const & pp )
{
    //lint -e747  Significant prototype coercion with setw
    out << L" " << setw( 3 ) << pp.x << L" " << setw( 3 ) << pp.y << L" ";
    return out;
}

wostream & Util::operator << ( wostream & out, PixelRect const & rect )
{
    out << rect.GetStartPoint( ) << L' ' << rect.GetEndPoint( );
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

void Util::AdjustRight( HWND hWnd, int iYpos )
{
    HWND const hWndParent   = GetParent( hWnd );
    int  const iWidthParent = GetClientWindowWidth( hWndParent );
    int  const iWidth       = GetWindowWidth( hWnd );
    int  const iHeight      = GetWindowHeight( hWnd );
    MoveWindow( hWnd, iWidthParent - iWidth, iYpos, iWidth, iHeight, TRUE );
    (void)BringWindowToTop( hWnd );
}

void Util::AdjustLeft( HWND hWnd, int iYpos )
{
	PixelPoint pnt = GetWindowSize( hWnd );
    MoveWindow( hWnd, 0, iYpos, pnt.x, pnt.y, TRUE );
    (void)BringWindowToTop( hWnd );
}

BOOL Util::MoveWindowAbsolute  // move window to given screen coordinates and set size
(
	HWND const hWnd,
	LONG const lXpos,
	LONG const lYpos,
	LONG const lWidth,
	LONG const lHeight,
	BOOL const bRepaint
)
{
	HWND  const hWndParent = GetAncestor( hWnd, GA_PARENT );
	POINT       pos{ lXpos, lYpos };

	if ( hWndParent != nullptr )
		ScreenToClient( hWndParent, &pos );

	return MoveWindow( hWnd, pos.x, pos.y, lWidth, lHeight, bRepaint );
}

BOOL Util::MoveWindowAbsolute  // move window to given screen coordinates 
(
	HWND const hWnd,
	LONG const lXpos,
	LONG const lYpos,
	BOOL const bRepaint
)
{
	PixelPoint const pixActSize = GetWindowSize( hWnd );

	return MoveWindowAbsolute( hWnd, lXpos, lYpos, pixActSize.x, pixActSize.y, bRepaint );
}

void Util::MakeLayered( HWND const hWnd, BOOL const bMode, COLORREF const crKey, UINT const uiAlpha )
{
    if ( bMode )
        AddWindowStyle( hWnd, WS_EX_LAYERED );
    else
        DeleteWindowStyle( hWnd, WS_EX_LAYERED );

    BOOL const bRes = SetLayeredWindowAttributes( hWnd, crKey, ( 255 * uiAlpha ) / 100, (crKey == 0) ? LWA_ALPHA : LWA_COLORKEY );
    assert( bRes );
}

HANDLE Util::MakeThread
( 
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID   lpParameter,
    LPDWORD  lpThreadId,
    HANDLE * phEvent
)
{
    //lint -esym( 954, hndThread )     HANDLE could be pointer to const
    HANDLE const hThread = CreateThread( nullptr, 0, lpStartAddress, lpParameter, 0, lpThreadId );

    assert( hThread != nullptr );

    if ( phEvent != nullptr )
    {
        *phEvent = CreateEvent( nullptr, FALSE, FALSE, nullptr );
        assert( * phEvent != nullptr );
        (void)WaitForSingleObject( *phEvent, INFINITE );  // wait until new thread has created a message queue
    }

    return hThread;
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
