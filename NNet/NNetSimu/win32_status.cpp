// win32_status.cpp : Verwaltet die Status Bar
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "win32_tooltip.h"
#include "win32_status.h"

static LRESULT CALLBACK OwnerDrawStatusBar( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
	StatusBar * const pStatusBar = (StatusBar *)dwRefData;
	switch ( uMsg )
	{

	case WM_COMMAND:
		pStatusBar->PostCommand2Application( LOWORD(wParam), 0 );
		return true;

	default: 
		break;
	}

	return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

StatusBar::StatusBar() :
	m_pModel( nullptr )
{ }

void StatusBar::Start
( 
	HWND                             const hwndParent,
	NNetModelWriterInterface const * const pModel
)
{
	m_pModel = pModel;

	HWND hwndStatus = CreateWindow
	(
		STATUSCLASSNAME, 
		nullptr, 
		WS_CHILD,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, STATUS_BAR_HEIGHT.GetValue(),
		hwndParent,
		nullptr, 
		GetModuleHandle( nullptr ), 
		nullptr
	); 

	SetWindowHandle( hwndStatus );

	(void)SetWindowSubclass( hwndStatus, OwnerDrawStatusBar, 0, (DWORD_PTR)this ) ;

	m_pixBorderX      = PIXEL(PIXEL(GetSystemMetrics( SM_CXSIZEFRAME ))) + 10_PIXEL;
	m_pixBorderY      = PIXEL(PIXEL(GetSystemMetrics( SM_CYSIZEFRAME )));
	m_pixClientHeight = GetHeight( ) - m_pixBorderY;
}

void StatusBar::AddCustomControl
( 
	int const width  // in number of characters
)
{
	static PIXEL const FONT_WIDTH = PIXEL( 9 );
	m_pixPosX += FONT_WIDTH * width;
}

int StatusBar::NewPart( )
{
	m_pixPosX += m_pixBorderX;
	m_statWidths.push_back( m_pixPosX );
	m_pixPosX += m_pixBorderX;
	return static_cast<int>(m_statWidths.size());
}

void StatusBar::LastPart( )
{
	NewPart( );
	m_statWidths.push_back( -1_PIXEL  ); // Stop
	(void)SendMessage( SB_SETPARTS, m_statWidths.size(), (LPARAM)( m_statWidths.data() ) );
}

void StatusBar::Stop( )
{
	Show( false );
	DestroyWindow( );
}

LRESULT StatusBar::UserProc
( 
	UINT   const uMsg, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	return DefSubclassProc( GetWindowHandle(), uMsg, wParam, lParam );
}

HWND WINAPI StatusBar::addControl
(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD   dwStyle,
    HMENU   hMenu
)
{
    PIXEL const pixWidth = PIXEL(static_cast<int>( wcslen( lpWindowName ) ) * 9);
    HWND  const hwnd     = CreateWindow
    (
        lpClassName,                     // class name 
        lpWindowName,                    // title (caption) 
        WS_CHILD | WS_VISIBLE | dwStyle, // style 
        m_pixPosX.GetValue(),            // x position
		m_pixBorderY.GetValue(),         // y position 
        pixWidth.GetValue(),             // width
		m_pixClientHeight.GetValue(),    // height
        GetWindowHandle( ),              // parent window 
        hMenu,                           // control identifier 
        GetModuleHandle( nullptr ),      // instance 
        nullptr                          // no WM_CREATE parameter 
    );
	m_pixPosX += pixWidth;
	return hwnd;
}

HWND WINAPI StatusBar::AddStaticControl( LPCTSTR lpWindowName )
{
    HWND hwnd = addControl( WC_STATIC, lpWindowName, 0, nullptr );
	return hwnd;
}

HWND WINAPI StatusBar::AddButton( LPCTSTR const lpWindowName, HMENU const hMenu, DWORD const dwStyle )
{ 
	HWND hwnd = addControl( WC_BUTTON, lpWindowName, dwStyle, hMenu );
	return hwnd;
}

HWND WINAPI StatusBar::AddTrackBar( HMENU hMenu )
{ 
	HWND hwnd = addControl( TRACKBAR_CLASS, L"   Trackbar Control   ", WS_TABSTOP | WS_BORDER | TBS_NOTICKS, hMenu );
	return hwnd;
};

PIXEL StatusBar::GetHeight( ) const
{
    return STATUS_BAR_HEIGHT;
}
    
void StatusBar::Resize( ) const 
{
    (void)SendNotifyMessage( WM_SIZE, 0, 0 );
}

void StatusBar::DisplayInPart( int const iPart, wstring const & wstrLine )
{
    (void)SendNotifyMessage( SB_SETTEXT, iPart, (LPARAM)( wstrLine.c_str( ) ) );
}
