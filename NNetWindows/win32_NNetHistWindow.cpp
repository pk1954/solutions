// win32_NNetHistWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "NNetHistorySysGlue.h"
#include "win32_workThreadInterface.h"
#include "win32_NNetHistWindow.h"

NNetHistWindow::NNetHistWindow( ) :
	HistWindow( ),
	m_pWorkThreadInterface( nullptr )
{ }

void NNetHistWindow::Start
(
	HWND                  const hwndParent,
	HistorySystem       * const pHistorySystem,
	WorkThreadInterface * const pWorkThreadInterface
)
{
	HistWindow::Start( hwndParent, pHistorySystem, [&]() { return ! m_pWorkThreadInterface->IsRunning(); } );  // call base class
	m_pWorkThreadInterface = pWorkThreadInterface;
	//    m_pFocusPoint->RegisterFocusPointObserver( this );
}

void NNetHistWindow::Stop( )
{
	HistWindow::Stop( );
	m_pWorkThreadInterface = nullptr;
}

void NNetHistWindow::DoPaint( HDC const hDC )
{
	if ( m_pWorkThreadInterface )
	{
		PaintAllGenerations( hDC );
		PaintHighlightGenerations( hDC, m_pWorkThreadInterface->GetGenDemanded( ) );
	}
}

void NNetHistWindow::GotoGeneration( HIST_GENERATION const gen ) 
{ 
	m_pWorkThreadInterface->PostGotoGeneration( gen ); 
}
