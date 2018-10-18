// win32_workThreadInterface.cpp
//

#include "stdafx.h"
#include "assert.h"
#include "Resource.h"
#include "GridPoint24.h"
#include "EvolutionCore.h"
#include "EvoHistorySysGlue.h"
#include "win32_worker_thread.h"
#include "win32_workThreadInterface.h"

using namespace std;

WorkThreadInterface::WorkThreadInterface( wostream * pTraceStream ) :
	m_pCore( nullptr ),
	m_pEvoHistGlue( nullptr ),
	m_pWorkThread( nullptr ),
    m_pTraceStream( pTraceStream ),
    m_bTrace( TRUE )
{ }

void WorkThreadInterface::Start
( 
    PerformanceWindow  * const pPerformanceWindow,
	EditorWindow       * const pEditorWindow,
    EventInterface     * const pEvent,
    ObserverInterface  * const pObservers, 
    EvolutionCore      * const pCore,
    EvoHistorySysGlue  * const pEvoHistGlue

)
{
    m_pCore        = pCore;
	m_pEvoHistGlue = pEvoHistGlue;
	m_pWorkThread  = new WorkThread();
	m_pWorkThread->Start( pPerformanceWindow, pEditorWindow, pEvent, pObservers, pEvoHistGlue, this );
}

WorkThreadInterface::~WorkThreadInterface( )
{
	m_pCore        = nullptr;
	m_pEvoHistGlue = nullptr;
	m_pWorkThread  = nullptr;
    m_pTraceStream = nullptr;
}

void WorkThreadInterface::postGotoGeneration( HIST_GENERATION const gen )
{
    assert( gen >= 0 );

    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_GOTO_GENERATION, 0, static_cast<LPARAM>(gen.GetLong()) );
}

// procedural interface of worker thread

void WorkThreadInterface::DoProcessScript( wstring * const pwstr )
{
	m_pWorkThread->DoProcessScript( pwstr );
}

HIST_GENERATION WorkThreadInterface::GetGenDemanded( ) const 
{ 
	return m_pWorkThread->GetGenDemanded( );
}

void WorkThreadInterface::PostReset( BOOL bResetHistSys )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << (bResetHistSys ? 1 : 0) << endl;
    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_RESET_MODEL, bResetHistSys, 0 );
}

void WorkThreadInterface::PostRefresh( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_REFRESH, 0, 0 );
}

void WorkThreadInterface::PostSetPOI( GridPoint const & gp )
{
    if ( gp.IsInGrid() )
    {
        if ( m_bTrace )
            * m_pTraceStream << __func__ << L" " << gp << endl;
        m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_SET_POI, gp.x, gp.y );
    }
}

void WorkThreadInterface::PostDoEdit( GridPoint const & gp )
{
    if ( gp.IsInGrid() )
    {
        if ( m_bTrace )
            * m_pTraceStream << __func__ << L" " << gp << endl;
        m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_DO_EDIT, gp.x, gp.y );
    }
}

void WorkThreadInterface::PostSetBrushIntensity( LONG const lValue )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << lValue << endl;
    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_SET_BRUSH_INTENSITY, lValue, 0 );
}

void WorkThreadInterface::PostSetBrushRadius( INT const iValue )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << iValue << endl;
    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_SET_BRUSH_RADIUS, iValue, 0 );
}

void WorkThreadInterface::PostSetBrushMode( tBrushMode const mode )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetBrushModeName( mode ) << endl;
    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_SET_BRUSH_MODE, static_cast<WPARAM>( mode ), 0 );
}

void WorkThreadInterface::PostSetSimulationMode( tBoolOp const op )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetBoolOpName( op ) << endl;
	PostStopComputation( );
    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_SET_SIMULATION_MODE, static_cast<WPARAM>( op ), 0 );
}

void WorkThreadInterface::PostSetBrushShape( tShape const shape )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetShapeName( shape ) << endl;
    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_SET_BRUSH_SHAPE, static_cast<WPARAM>( shape ), 0 );
}

void WorkThreadInterface::PostSetBrushManipulator( tManipulator const op )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetManipulatorName( op ) << endl;
    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_SET_BRUSH_OPERATOR, static_cast<WPARAM>( op ), 0 );
}

void WorkThreadInterface::PostRunGenerations( bool const bFirst )
{
    if ( m_bTrace )
        * m_pTraceStream << L"PostGenerationStep" << endl;
	m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_GENERATION_RUN, 0, bFirst );
}

void WorkThreadInterface::PostRedo( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	HIST_GENERATION gen = m_pEvoHistGlue->GetCurrentGeneration( );

	if ( ( gen < m_pEvoHistGlue->GetYoungestGeneration( ) ) && m_pEvoHistGlue->IsEditorCommand( gen + 1 ) )
		postGotoGeneration( gen + 1 );
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void WorkThreadInterface::PostGenerationStep( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
	postGotoGeneration( m_pEvoHistGlue->GetCurrentGeneration( ) + 1 );
}

void WorkThreadInterface::PostRepeatGenerationStep( )
{
    if ( m_bTrace )
        * m_pTraceStream << L"PostGenerationStep" << endl;
    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_REPEAT_GENERATION_STEP, 0, 0 );
}

void WorkThreadInterface::PostUndo( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	HIST_GENERATION gen = m_pEvoHistGlue->GetCurrentGeneration( );

	if ( ( gen > 0 ) && m_pEvoHistGlue->IsEditorCommand( gen - 1 ) )
		postGotoGeneration( gen - 1 );
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void WorkThreadInterface::PostPrevGeneration( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
	if (m_pEvoHistGlue->GetCurrentGeneration() > 0)
		postGotoGeneration( m_pEvoHistGlue->GetCurrentGeneration() - 1 );
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void WorkThreadInterface::PostHistoryAction( UINT const uiID, GridPoint const gp )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << uiID << L" " << gp << endl;

	assert( m_pCore->IsAlive(gp) );
	assert( (uiID == IDM_GOTO_ORIGIN) || (uiID == IDM_GOTO_DEATH) );

	IndId           idTarget  = m_pCore->GetId(gp);
	HIST_GENERATION genTarget = ( uiID == IDM_GOTO_ORIGIN )
	                            ? m_pEvoHistGlue->GetFirstGenOfIndividual(idTarget)
		                        : m_pEvoHistGlue->GetLastGenOfIndividual(idTarget);
	
	postGotoGeneration( genTarget );
}

void WorkThreadInterface::PostGotoGeneration( HIST_GENERATION const gen )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << gen << endl;
	postGotoGeneration( gen );
}

void WorkThreadInterface::PostStopComputation( )
{
	m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_STOP, 0, 0 );
}

void WorkThreadInterface::PostProcessScript( wstring const & wstrPath )
{
//lint -esym( 429, pwstr )     not freed here, happens in worker thread
    wstring * const pwstr = new wstring( wstrPath );
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" \"" << wstrPath.c_str( )  << "\""<< endl;

    m_pWorkThread->WorkMessage( WorkThread::THREAD_MSG_PROCESS_SCRIPT, 0, (LPARAM)pwstr );
}

// no trace output

void WorkThreadInterface::PostEndThread( HWND const hwndCtl )
{
	m_pWorkThread->TerminateThread( hwndCtl );
	DestroyWindow( hwndCtl);           // TODO: shift to WorkerThread after termination of message pump
}
