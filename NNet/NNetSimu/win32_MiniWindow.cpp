// win32_MiniWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "MoreTypes.h"
#include "Resource.h"
#include "DrawModel.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelInterface.h"
#include "win32_thread.h"
#include "win32_MiniWindow.h"

MiniWindow::~MiniWindow( )
{
	m_pNNetWindowObserved = nullptr;
}

void CALLBACK CenterModelTimer( HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime )
{
	SendMessage( hwnd, WM_COMMAND, IDM_CENTER_MODEL, 0 );
}

void MiniWindow::Observe( NNetWindow * const pNNetWin )	
{ 
	m_pNNetWindowObserved = pNNetWin; 
	m_TimerId = SetTimer( GetWindowHandle(), 1, 300, CenterModelTimer );
}

long MiniWindow::AddContextMenuEntries( HMENU const hPopupMenu, PixelPoint const ptPos )
{
	UINT static const STD_FLAGS { MF_BYPOSITION | MF_STRING };

	AppendMenu( hPopupMenu, STD_FLAGS, IDM_CENTER_MODEL, L"Center model" );
	return 0L;
}

void MiniWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	if ( wParam & MK_LBUTTON )       	// Left mouse button: move or edit action
	{
		PixelPoint const ptCrsr { GetCrsrPosFromLparam( lParam ) };  // relative to client area

		if ( m_ptLast.IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			MicroMeterPoint const umCrsrPos { m_context.GetCoordC().Convert2MicroMeterPointPos( ptCrsr   ) };
			MicroMeterPoint const umLastPos { m_context.GetCoordC().Convert2MicroMeterPointPos( m_ptLast ) };
			MicroMeterPoint const umDelta   { umLastPos - umCrsrPos }; 

			// compute current position of main window view 

			MicroMeterRect const umRectCurrent   = m_pNNetWindowObserved->GetViewRect();
			PixelRect      const pixRectCurrent  = m_context.GetCoordC().Convert2PixelRect( umRectCurrent );

			// compute new position of main window view

//			MicroMeterRect const umRectNew = umRectCurrent + umDelta;
			PixelRect      const pixRectNew = pixRectCurrent + m_ptLast - ptCrsr;

			// Compute view area of mini window

			PixelRect      const pixRectMiniWindow { GetClPixelRect() };
//			MicroMeterRect const umRectMiniWindow  { m_context.GetCoordC().Convert2MicroMeterRect( pixRectMiniWindow )  };

			// is it completely inside of mini window?

			assert( pixRectMiniWindow.Includes( pixRectCurrent ) );

			if ( pixRectMiniWindow.Includes( pixRectNew ) )
				m_pNNetWindowObserved->NNetMove( umDelta );  // move the observed window in opposite direction 
		}

		m_ptLast = ptCrsr;
	}
	else
	{
		m_ptLast.Set2Null();   // make m_ptLast invalid
	}
}

void MiniWindow::OnLButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const ptCrsr { GetCrsrPosFromLparam( lParam ) };  // relative to client area
	m_ptLast = ptCrsr;
}

BOOL MiniWindow::OnCommand( WPARAM const wParam, LPARAM const lParam )
{
	switch ( wParam )
	{

	case IDM_CENTER_MODEL:
		CenterModel( false );
		break;

	default:
		return FALSE; // command has not been processed
	}

	return FALSE;
}

void MiniWindow::doPaint( bool const bShowScale ) 
{
	PixelRect const pixRect { GetClPixelRect( ) };

	m_pModelInterface->LockModelShared();
	m_pDrawModel->DrawExteriorInRect( pixRect, m_context );
	m_pModelInterface->UnlockModelShared();

	m_context.DrawTranspRect( m_pNNetWindowObserved->GetViewRect(), NNetColors::POSITION_RECT );
}
