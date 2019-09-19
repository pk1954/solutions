// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWindow.h"

NNetWorkThreadInterface * NNetWindow::m_pNNetWorkThreadInterface = nullptr;

void NNetWindow::InitClass
(        
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	ActionTimer             * const pActionTimer
)
{
	ModelWindow::InitClass( pActionTimer );
	m_pNNetWorkThreadInterface = pNNetWorkThreadInterface;
}

NNetWindow::NNetWindow( ) :
	ModelWindow( ),
	m_hPopupMenu( nullptr ),
	m_bMoveAllowed( TRUE )
{ }

void NNetWindow::Start
( 
	HWND  const hwndApp, 
	DWORD const dwStyle,
	std::function<bool()> const visibilityCriterion
)
{
	HWND hwnd = StartBaseWindow
	( 
		hwndApp,
		CS_OWNDC | CS_DBLCLKS,
		L"ClassNNetWindow",
		dwStyle,
		nullptr,
		visibilityCriterion
	);
}

void NNetWindow::Stop( )
{
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pNNetWorkThreadInterface = nullptr;
}

void NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu, POINT const pntPos )
{
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );  // relative to client area
}

void NNetWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		static COLORREF const CLR_RED = RGB( 128, 0, 0 );
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );
		FillBackground( hDC, CLR_RED );
		(void)EndPaint( &ps );
	}
}

void NNetWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{
}

void NNetWindow::OnLButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	if ( inObservedClientRect( lParam ) || m_bMoveAllowed )
		OnMouseMove( wParam, lParam );
}

void NNetWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	(void)ReleaseCapture( );
}

void NNetWindow::OnSetCursor( WPARAM const wParam, LPARAM const lParam )
{
}

BOOL NNetWindow::OnCommand( WPARAM const wParam, LPARAM const lParam )
{
	UINT uiCmdId = LOWORD( wParam );
	switch ( uiCmdId )
	{

	default:
		return TRUE;
	}
	return FALSE;
}

BOOL NNetWindow::inObservedClientRect( LPARAM const lParam )
{
	return TRUE;  // Is cursor position in observed client rect?
}
