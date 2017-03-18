// win32_histWorkerThread.cpp

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "resource.h"
#include "ModelData.h"
#include "EvolutionCore.h"
#include "win32_status.h"
#include "win32_editor.h"
#include "win32_histWorkerThread.h"

using namespace std;

HistWorkThread::HistWorkThread
( 
    wofstream     *       pTraceStream,
    EvolutionCore * const pCore,
    ModelData     * const pModel,
    EvoHistorySys * const pHistorySys
) :
    WorkThread( pTraceStream ),
    m_genDemanded( 0 ),
    m_pEvoHistorySys( pHistorySys ),
    m_pCore( pCore ),
    m_pModelWork( pModel )
{ }

HistWorkThread::~HistWorkThread( )
{}

// generationStep - perform one history step towards demanded generation
//                - update editor state if neccessary

void HistWorkThread::generationStep( )
{
    if ( m_pEvoHistorySys->GetCurrentGeneration( ) != m_genDemanded )
    {
        m_pEvoHistorySys->EvoApproachHistGen( m_genDemanded );

        if ( m_pCore->EditorStateHasChanged( m_pModelWork ) )
        {
            m_pCore->SaveEditorState( m_pModelWork );
            m_pEditorWindow->UpdateControls( );
        }

        postMsg2WorkThread( THREAD_MSG_REFRESH, 0, 0 );
    }
}

void HistWorkThread::GenerationRun( )
{
    m_genDemanded = m_pEvoHistorySys->GetCurrentGeneration( ) + 1;
    WorkThread::GenerationRun( );
}

void HistWorkThread::ApplyEditorCommand( tEvoCmd const evoCmd, short const sParam )
{
    if ( m_pEvoHistorySys->CreateNewGeneration( evoCmd, sParam ) )
        m_pCore->SaveEditorState( m_pModelWork );
}

void HistWorkThread::StopComputation()
{
	m_genDemanded = m_pEvoHistorySys->GetCurrentGeneration();
	WorkThread::StopComputation();
}

void HistWorkThread::DoEdit( GridPoint const gp )
{
	if ( m_pEvoHistorySys->CreateNewGeneration( tEvoCmd::editSetXvalue, gp.x ) )
		 m_pEvoHistorySys->CreateNewGeneration( tEvoCmd::editSetYvalue, gp.y );
}

void HistWorkThread::DoExit( HWND hwndApp )
{
	WorkThread::DoExit( hwndApp );
}

void HistWorkThread::gotoGeneration( HIST_GENERATION const gen )
{
	m_genDemanded = gen;
	do 
		generationStep( );
	while ( m_pEvoHistorySys->GetCurrentGeneration( ) != m_genDemanded );
}

void HistWorkThread::PostNextGeneration( )
{
	gotoGeneration( m_pEvoHistorySys->GetCurrentGeneration( ) + 1 );
}

void HistWorkThread::PostPrevGeneration( )
{
	if (m_pEvoHistorySys->GetCurrentGeneration() > 0)
		PostGotoGeneration(m_pEvoHistorySys->GetCurrentGeneration() - 1);
	else
		(void)MessageBeep(MB_OK);  // first generation reached
}

void HistWorkThread::PostHistoryAction( UINT const uiID, GridPoint const gp )
{
	assert( m_pModelWork->IsAlive(gp) );
	assert( (uiID == IDM_GOTO_ORIGIN) || (uiID == IDM_GOTO_DEATH) );

	IndId           idTarget  = m_pModelWork->GetId(gp);
	HIST_GENERATION genTarget = ( uiID == IDM_GOTO_ORIGIN )
	                            ? m_pEvoHistorySys->GetFirstGenOfIndividual(idTarget)
		                        : m_pEvoHistorySys->GetLastGenOfIndividual(idTarget);
	
	PostGotoGeneration( genTarget );
}

void HistWorkThread::PostGotoGeneration( HIST_GENERATION const gen )
{
    if ( m_bTrace )
        *m_pTraceStream << "PostGotoGeneration " << gen.GetLong( ) << endl;

    assert( gen >= 0 );
    assert( gen <= m_pEvoHistorySys->GetHistorySystem( )->GetYoungestGeneration( ) );

	gotoGeneration( gen );
}
