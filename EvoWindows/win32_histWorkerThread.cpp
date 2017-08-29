// win32_histWorkerThread.cpp

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "resource.h"
#include "errhndl.h"
#include "EvolutionModelData.h"
#include "EvoHistorySys.h"
#include "win32_status.h"
#include "win32_editor.h"
#include "win32_histWorkerThread.h"

using namespace std;

HistWorkThread::HistWorkThread
( 
    wostream           *       pTraceStream,
    EvolutionModelData * const pModel,
    EvoHistorySys      * const pHistorySys
) :
    WorkThread( pTraceStream ),
    m_genDemanded( 0 ),
    m_pEvoHistorySys( pHistorySys ),
    m_pModelWork( pModel )
{ }

HistWorkThread::~HistWorkThread( )
{}

void HistWorkThread::ResetModel( )    // Layer 5
{
	m_pEvoHistorySys->EvoCreateResetCommand( );
}

// GenerationStep - perform one history step towards demanded generation
//                - update editor state if neccessary

void HistWorkThread::GenerationStep( )   // Layer 5
{
	if ( m_pEvoHistorySys->GetCurrentGeneration( ) != m_genDemanded )
	{
		m_pEvoHistorySys->EvoApproachHistGen( m_genDemanded );

		if ( EditorStateHasChanged( ) )
		{
	        SaveEditorState( );
			m_pEditorWindow->UpdateControls( );
		}

		postMsg2WorkThread( THREAD_MSG_REFRESH, 0, 0 );
    
		if ( m_pEvoHistorySys->GetCurrentGeneration( ) != m_genDemanded )
			WorkThread::PostGenerationStep(  );   // Loop! Will call indirectly HistWorkThread::GenerationStep again
	}
}

void HistWorkThread::GenerationRun( )
{
    m_genDemanded = m_pEvoHistorySys->GetCurrentGeneration( ) + 1;
    WorkThread::GenerationRun( );
}

void HistWorkThread::ApplyEditorCommand( tEvoCmd const evoCmd, unsigned short const usParam )
{
    if ( m_pEvoHistorySys->CreateEditorCommand( evoCmd, usParam ) )
        SaveEditorState( );
}

void HistWorkThread::StopComputation()
{
	m_genDemanded = m_pEvoHistorySys->GetCurrentGeneration();
	WorkThread::StopComputation();
}

void HistWorkThread::DoEdit( GridPoint const gp )
{
	m_pEvoHistorySys->CreateEditorCommand( tEvoCmd::editDoEdit, gp.Pack( ) );
}

void HistWorkThread::postGotoGeneration( HIST_GENERATION const gen )
{
	m_genDemanded = gen;
	WorkThread::PostGenerationStep(  );   // will call indirectly HistWorkThread::GenerationStep
}

void HistWorkThread::PostRedo( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	HIST_GENERATION gen = m_pEvoHistorySys->GetCurrentGeneration( );

	if ( ( gen < m_pEvoHistorySys->GetYoungestGeneration( ) ) && m_pEvoHistorySys->IsEditorCommand( gen + 1 ) )
		PostGotoGeneration( gen + 1 );
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void HistWorkThread::PostGenerationStep( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	postGotoGeneration( m_pEvoHistorySys->GetCurrentGeneration( ) + 1 );
}

void HistWorkThread::PostUndo( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	HIST_GENERATION gen = m_pEvoHistorySys->GetCurrentGeneration( );

	if ( ( gen > 0 ) && m_pEvoHistorySys->IsEditorCommand( gen - 1 ) )
		PostGotoGeneration( gen - 1 );
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void HistWorkThread::PostPrevGeneration( )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << endl;

	if (m_pEvoHistorySys->GetCurrentGeneration() > 0)
		PostGotoGeneration( m_pEvoHistorySys->GetCurrentGeneration() - 1 );
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void HistWorkThread::PostHistoryAction( UINT const uiID, GridPoint const gp )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << uiID << L" " << gp << endl;

	assert( m_pModelWork->IsAlive(gp) );
	assert( (uiID == IDM_GOTO_ORIGIN) || (uiID == IDM_GOTO_DEATH) );

	IndId           idTarget  = m_pModelWork->GetId(gp);
	HIST_GENERATION genTarget = ( uiID == IDM_GOTO_ORIGIN )
	                            ? m_pEvoHistorySys->GetFirstGenOfIndividual(idTarget)
		                        : m_pEvoHistorySys->GetLastGenOfIndividual(idTarget);
	
	postGotoGeneration( genTarget );
}

void HistWorkThread::PostGotoGeneration( HIST_GENERATION const gen )
{
    if ( m_bTrace )
        * m_pTraceStream << __func__ << L" " << gen << endl;

    assert( gen >= 0 );
    if ( gen > m_pEvoHistorySys->GetHistorySystem( )->GetYoungestGeneration( ) )
		ScriptErrorHandler::semanticError( L"PostGotoGeneration - GenNr too big");

	postGotoGeneration( gen );
}
