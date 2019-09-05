// win32_evoHistWindow.cpp
//
// EvoWindows

#include "stdafx.h"
#include "config.h"
#include "EvoHistorySysGlue.h"
#include "win32_focusPoint.h"
#include "win32_EvoWorkThreadInterface.h"
#include "win32_evoHistWindow.h"

EvoHistWindow::EvoHistWindow( ) :
    HistWindow( ),
    m_pWorkThreadInterface( nullptr ),
    m_pFocusPoint( nullptr )
{ }

void EvoHistWindow::Start
(
    HWND                     const hwndParent,
    FocusPoint             * const pFocusPoint,
	HistorySystem          * const pHistorySystem,
	EvoWorkThreadInterface * const pWorkThreadInterface
)
{
    HistWindow::Start( hwndParent, pHistorySystem, [&]() { return ! m_pWorkThreadInterface->IsRunning(); } );  // call base class
	m_pFocusPoint          = pFocusPoint;
	m_pWorkThreadInterface = pWorkThreadInterface;
//    m_pFocusPoint->RegisterFocusPointObserver( this );
}

void EvoHistWindow::Stop( )
{
	HistWindow::Stop( );
	m_pWorkThreadInterface = nullptr;
	m_pFocusPoint          = nullptr;
}

void EvoHistWindow::DoPaint( HDC const hDC )
{
	if ( m_pWorkThreadInterface )
	{
		PaintAllGenerations( hDC );
		PaintHighlightGenerations( hDC, m_pWorkThreadInterface->GetGenDemanded( ) );
	}

// Deactivated, see win32_focusPoint.cpp
//
//  if ( m_pFocusPoint->IsInGrid( ) && m_pFocusPoint->IsAlive( ) )
//  {
//		PaintLifeLine( hDC, m_pFocusPoint->GetGenBirth( ), m_pFocusPoint->GetGenDeath( ) );
//	}
}

void EvoHistWindow::GotoGeneration( HIST_GENERATION const gen ) 
{ 
	m_pWorkThreadInterface->PostGotoGeneration( gen ); 
}
