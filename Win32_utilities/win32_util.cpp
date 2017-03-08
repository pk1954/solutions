// win32_util.cpp :
//

#include "stdafx.h"
#include <iomanip>
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

PixelRect Util::ScrReadPixelRect( Script & script )
{
    PixelRect rect;
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
    int  const iWidthParent = Util::GetClientWindowWidth( hWndParent );
    int  const iWidth       = Util::GetWindowWidth( hWnd );
    int  const iHeight      = Util::GetWindowHeight( hWnd );
    MoveWindow( hWnd, iWidthParent - iWidth, iYpos, iWidth, iHeight, TRUE );
    (void)BringWindowToTop( hWnd );
}

void Util::AdjustLeft( HWND hWnd, int iYpos )
{
    int  const iWidth  = Util::GetWindowWidth( hWnd );
    int  const iHeight = Util::GetWindowHeight( hWnd );
    MoveWindow( hWnd, 0, iYpos, iWidth, iHeight, TRUE );
    (void)BringWindowToTop( hWnd );
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
