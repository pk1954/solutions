// win32_evoHistWindow.cpp
//

#include "stdafx.h"
#include "config.h"
#include "EvolutionCore.h"
#include "EvoHistorySys.h"
#include "win32_focusPoint.h"
#include "win32_worker_thread.h"
#include "win32_evoHistWindow.h"

EvoHistWindow::EvoHistWindow( ) :
    HistWindow( ),
    m_pWorkThread( nullptr ),
    m_pFocusPoint( nullptr )
{ }

EvoHistWindow::~EvoHistWindow( )
{
    m_pWorkThread = nullptr;
	m_pFocusPoint = nullptr;
}

void EvoHistWindow::Start
(
    HWND            const hWndParent,
    FocusPoint    * const pFocusPoint,
	EvoHistorySys * const pEvoHistorySys,
	WorkThread    * const pWorkThread
)
{
    Config::tOnOffAuto const displayMode = static_cast<Config::tOnOffAuto>( Config::GetConfigValue( Config::tId::historyDisplay ) );
    BOOL               const bShow       = ( displayMode == Config::tOnOffAuto::on );

    HistWindow::Start( hWndParent, pEvoHistorySys->GetHistorySystem( ) );
	m_pWorkThread = pWorkThread;
	m_pFocusPoint = pFocusPoint;
    m_pFocusPoint->AttachFocusPointObserver( this, 75 );
    Show( bShow );
}

void EvoHistWindow::DoPaint( HDC const hDC )
{
    PaintAllGenerations( hDC );

    PaintHighlightGenerations( hDC, m_pWorkThread->GetGenDemanded( ) );

    if ( m_pFocusPoint->IsInGrid( ) && m_pFocusPoint->IsAlive( ) )
    {
		PaintLifeLine( hDC, m_pFocusPoint->GetGenBirth( ), m_pFocusPoint->GetGenDeath( ) );
	}
}
