// win32_workThreadInterface.cpp
//

#include "stdafx.h"
#include "assert.h"
#include "Resource.h"
#include "gridPOI.h"
#include "GridPoint24.h"
#include "EvolutionTypes.h"
#include "EvolutionCore.h"
#include "EvoHistorySysGlue.h"
#include "win32_script.h"
#include "win32_worker_thread.h"
#include "win32_workThreadInterface.h"

using std::wostream;
using std::wcout;
using std::endl;

WorkThreadInterface::WorkThreadInterface( ) :
	m_pEvoHistGlue( nullptr ),
	m_pWorkThread ( nullptr ),
	m_pTraceStream( nullptr ),
	m_bTrace( TRUE )
{ }

void WorkThreadInterface::Initialize( wostream * pTraceStream ) 
{ 
	m_pTraceStream = pTraceStream;
}

void WorkThreadInterface::Start
( 
	HWND                 const hwndApplication,
    ColorManager       * const pColorManager,
    PerformanceWindow  * const pPerformanceWindow,
	EditorWindow       * const pEditorWindow,
    EventInterface     * const pEvent,
    ReadBuffer         * const pReadBuffer, 
    EvolutionCore      * const pCore,
    EvoHistorySysGlue  * const pEvoHistGlue
)
{
	m_pEvoHistGlue = pEvoHistGlue;
	m_pWorkThread  = new WorkThread
	( 
		hwndApplication, 
		pColorManager, 
		pPerformanceWindow, 
		pEditorWindow, 
		pEvent, 
		pReadBuffer, 
		pEvoHistGlue, 
		this
	);
}

WorkThreadInterface::~WorkThreadInterface( )
{
	m_pEvoHistGlue = nullptr;
	m_pWorkThread  = nullptr;
    m_pTraceStream = nullptr;
}

BOOL WorkThreadInterface::IsRunning( ) const
{
	return m_pWorkThread->IsRunning( );
}

void WorkThreadInterface::postGotoGeneration( HIST_GENERATION const gen )
{
    assert( gen >= 0 );

    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::GOTO_GENERATION, 0, static_cast<LPARAM>(gen.GetLong()) );
}

// procedural interface of worker thread

HIST_GENERATION WorkThreadInterface::GetGenDemanded( ) const 
{ 
	return m_pWorkThread->GetGenDemanded( );
}

void WorkThreadInterface::PostReset( BOOL bResetHistSys )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << (bResetHistSys ? 1 : 0) << endl;
    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::RESET_MODEL, bResetHistSys, 0 );
}

void WorkThreadInterface::PostBenchmark( int const iNrOfGenerations )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << iNrOfGenerations <<endl;
    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::BENCHMARK, 0, iNrOfGenerations );
}

void WorkThreadInterface::PostDoEdit( GridPoint const gp )
{
    if ( IsInGrid( gp ) )
    {
        if ( m_bTrace )
            * m_pTraceStream << __func__ << L" " << gp << endl;
        m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::DO_EDIT, gp.GetXvalue(), gp.GetYvalue() );
    }
}

void WorkThreadInterface::PostSetBrushIntensity( PERCENT const intensity )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << intensity.GetValue() << endl;
    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::SET_BRUSH_INTENSITY, intensity.GetValue(), 0 );
}

void WorkThreadInterface::PostSetBrushRadius( GRID_COORD const radius )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << radius.GetValue() << endl;
    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::SET_BRUSH_RADIUS, radius.GetValue(), 0 );
}

void WorkThreadInterface::PostSetBrushMode( tBrushMode const mode )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetBrushModeName( mode ) << endl;
    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::SET_BRUSH_MODE, static_cast<WPARAM>( mode ), 0 );
}

void WorkThreadInterface::PostSetBrushShape( tShape const shape )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetShapeName( shape ) << endl;
    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::SET_BRUSH_SHAPE, static_cast<WPARAM>( shape ), 0 );
}

void WorkThreadInterface::PostSetBrushManipulator( tManipulator const op )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" tManipulator::" << GetManipulatorName( op ) << endl;
    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::SET_BRUSH_OPERATOR, static_cast<WPARAM>( op ), 0 );
}

void WorkThreadInterface::PostSetColor( COLORREF const col, tColorObject const obj, Strategy::Id const strat )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << GetColorObjectName( obj ) << endl;
	switch ( obj )
	{
	case tColorObject::individual:
	    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::SET_STRATEGY_COLOR, static_cast<WPARAM>( strat ), static_cast<LPARAM>( col ) );
		break;
	case tColorObject::selection:
	    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::SET_SELECTION_COLOR, 0, static_cast<LPARAM>( col ) );
		break;
	case tColorObject::highlight:
	    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::SET_HIGHLIGHT_COLOR, 0, static_cast<LPARAM>( col ) );
		break;
	default:
		assert( false );
	}
}

void WorkThreadInterface::PostSetPOI( GridPoint const gp )
{
	if ( IsInGrid( gp ) )
	{
		if ( m_bTrace )
			* m_pTraceStream << __func__ << L" " << gp << endl;

		GridPOI::SetPoi( gp );

		m_pWorkThread->Continue( );     // trigger worker thread if waiting on POI event
	}
}

void WorkThreadInterface::PostGenerationStep( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	m_pWorkThread->Continue( );     // trigger worker thread if waiting on POI event
	
	m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::GENERATION_STEP, 0, 0 );
}

void WorkThreadInterface::PostRunGenerations( bool const bFirst )
{
    if ( m_bTrace )
        * m_pTraceStream << L"PostGenerationStep" << endl;
	m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::GENERATION_RUN, 0, bFirst );
}

void WorkThreadInterface::PostRedo( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
	m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::REDO, 0, 0 );
}

void WorkThreadInterface::PostRepeatGenerationStep( )
{
    if ( m_bTrace )
        * m_pTraceStream << L"PostGenerationStep" << endl;
    m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::REPEAT_GENERATION_STEP, 0, 0 );
}

void WorkThreadInterface::PostUndo( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
	m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::UNDO, 0, 0 );
}

void WorkThreadInterface::PostPrevGeneration( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;
	m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::PREV_GENERATION, 0, 0 );
}

void WorkThreadInterface::PostGotoOrigin( GridPoint const gp )
{
	if ( m_bTrace )
		* m_pTraceStream << __func__ << L" " << gp << endl;
	m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::GOTO_ORIGIN, gp.GetXvalue(), gp.GetYvalue() );
}

void WorkThreadInterface::PostGotoDeath( GridPoint const gp )
{
	if ( m_bTrace )
		* m_pTraceStream << __func__ << L" " << gp << endl;
	m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::GOTO_DEATH, gp.GetXvalue(), gp.GetYvalue() );
}

void WorkThreadInterface::PostGotoGeneration( HIST_GENERATION const gen )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << gen << endl;
	postGotoGeneration( gen );
}

void WorkThreadInterface::PostStopComputation( )
{
	m_pWorkThread->WorkMessage( WorkerThreadMessage::Id::STOP, 0, 0 );
}

// no trace output

void WorkThreadInterface::TerminateThread( )
{
	m_pWorkThread->Terminate( );    // wait until thread has stopped
}
