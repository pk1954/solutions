// win32_evoHistWindow.cpp
//

#include "stdafx.h"
#include "config.h"
#include "EvoHistorySysGlue.h"
#include "win32_focusPoint.h"
#include "win32_workThreadInterface.h"
#include "win32_evoHistWindow.h"

EvoHistWindow::EvoHistWindow( ) :
    HistWindow( ),
    m_pWorkThreadInterface( nullptr ),
    m_pFocusPoint( nullptr )
{ }

EvoHistWindow::~EvoHistWindow( )
{
    m_pWorkThreadInterface = nullptr;
	m_pFocusPoint          = nullptr;
}

void EvoHistWindow::Start
(
    HWND                  const hwndParent,
    FocusPoint          * const pFocusPoint,
	HistorySystem       * const pHistorySystem,
	WorkThreadInterface * const pWorkThreadInterface
)
{
    HistWindow::Start( hwndParent, pHistorySystem );  // call base class
	m_pFocusPoint          = pFocusPoint;
	m_pWorkThreadInterface = pWorkThreadInterface;
    m_pFocusPoint->AttachFocusPointObserver( this );
	Show( Config::GetConfigValueOnOffAuto( Config::tId::historyDisplay ) == Config::tOnOffAuto::on );
}

void EvoHistWindow::DoPaint( HDC const hDC )
{
    PaintAllGenerations( hDC );

    PaintHighlightGenerations( hDC, m_pWorkThreadInterface->GetGenDemanded( ) );

// Deactivated, see win32_focusPoint.cpp
//
//    if ( m_pFocusPoint->IsInGrid( ) && m_pFocusPoint->IsAlive( ) )
//    {
//		PaintLifeLine( hDC, m_pFocusPoint->GetGenBirth( ), m_pFocusPoint->GetGenDeath( ) );
//	}
}

void EvoHistWindow::GotoGeneration( HIST_GENERATION const gen ) 
{ 
	m_pWorkThreadInterface->PostGotoGeneration( gen ); 
}
