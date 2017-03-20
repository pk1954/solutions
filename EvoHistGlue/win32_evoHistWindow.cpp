// win32_evoHistWindow.cpp
//

#include "stdafx.h"
#include "config.h"
#include "EvolutionCore.h"
#include "EvoHistorySys.h"
#include "HistAllocThread.h"
#include "win32_focusPoint.h"
#include "win32_status.h"
#include "win32_histWorkerThread.h"
#include "win32_evoHistWindow.h"

EvoHistWindow::EvoHistWindow( ) :
    HistWindow( ),
    m_pHistWorkThread( nullptr ),
    m_pFocusPoint( nullptr ),
    m_pStatusBar( nullptr ),
	m_pHistAllocThread( nullptr )
{ }

EvoHistWindow::~EvoHistWindow( )
{
	m_pHistAllocThread->ExitHistAllocThread();
    shutDownHistoryCache( );
	delete m_pHistAllocThread;
    m_pHistWorkThread = nullptr;
	m_pEvoHistorySys = nullptr;
	m_pHistAllocThread = nullptr;
	m_pFocusPoint = nullptr;
    m_pStatusBar = nullptr;
}

void EvoHistWindow::Start
(
    HWND                 const hWndParent,
    FocusPoint         * const pFocusPoint,
    StatusBar          * const pStatusBar,
    EvolutionModelData * const pEvoModelData,
	EvoHistorySys      * const pEvoHistorySys,
	HistWorkThread     * const pHistWorkThread
)
{
    Config::tOnOffAuto const displayMode = static_cast<Config::tOnOffAuto>( Config::GetConfigValue( Config::tId::historyDisplay ) );
    BOOL               const bShow       = ( displayMode == Config::tOnOffAuto::on );

    m_pEvoHistorySys  = pEvoHistorySys;
    HistWindow::Start( hWndParent, m_pEvoHistorySys->GetHistorySystem( ) );

	m_pHistWorkThread = pHistWorkThread;
	m_pFocusPoint = pFocusPoint;
    m_pFocusPoint->AttachFocusPointObserver( this, 75 );
    m_pFocusPoint->Start( m_pEvoHistorySys, pEvoModelData );
    m_pStatusBar = pStatusBar;
    Show( bShow );
	m_pHistAllocThread = new HistAllocThread;
	m_pHistAllocThread->AllocateHistorySlots( m_pEvoHistorySys->GetHistorySystem( )	);  // delegate allocation of history slots to a work thread
}

void EvoHistWindow::PostGotoGeneration(HIST_GENERATION const gen)
{
	m_pHistWorkThread->PostGotoGeneration(gen);
}

HIST_GENERATION EvoHistWindow::GetGenDemanded() const
{
	return m_pHistWorkThread->GetGenDemanded();
}

void EvoHistWindow::DoPaint( HDC const hdc )
{
    HistWindow::DoPaint( hdc );
    paintLifeLine( hdc );
}

void EvoHistWindow::paintLifeLine( HDC const hDC ) const
{
    GridPoint const gpFocus = m_pFocusPoint->GetGridPoint( );

    if ( gpFocus.IsInGrid( ) && m_pFocusPoint->IsAlive( ) )
    {
        HIST_GENERATION const genBirth = m_pFocusPoint->GetGenBirth( );
        HIST_GENERATION       genDeath = m_pFocusPoint->GetGenDeath( );
        if ( genDeath < genBirth )
            genDeath = genBirth;

        RECT       pixRect = GetGenerationRect( genBirth, genDeath );
        long const lHeight4 = GetClientWindowHeight( ) / 4;
        pixRect.top += lHeight4;
        pixRect.bottom -= lHeight4;

        SetBkColor( hDC, CLR_POI );
        Util::FastFill( hDC, pixRect );
    }
}

void EvoHistWindow::shutDownHistoryCache( )
{
    int iMax = m_pEvoHistorySys->GetNrOfHistCacheSlots( ) - 1;
    int iPercentLast = 0;
    for ( int iRun = iMax; iRun >= 0; --iRun )
    {
        int iPercent = ( iRun * 100 ) / iMax;
        if ( iPercent != iPercentLast )
        {
            std::wstring wstrLine = L"... deleting history buffer: " + to_wstring( iPercent ) + L"%";
            m_pStatusBar->DisplayStatusLine( wstrLine );
            iPercentLast = iPercent;
        }
        m_pEvoHistorySys->ShutDownHistCacheSlot( iRun );
    }
}
