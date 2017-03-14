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
    if ( GetCurrentGeneration( ) != m_genDemanded )
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

void HistWorkThread::generationRun( )
{
    m_genDemanded = GetCurrentGeneration( ) + 1;
    WorkThread::generationRun( );
}

void HistWorkThread::ApplyEditorCommand( tEvoCmd const evoCmd, short const sParam )
{
    if ( m_pEvoHistorySys->CreateNewGeneration( EvoGenerationCmd( evoCmd, sParam )) )
        m_pCore->SaveEditorState( m_pModelWork );
}
/*
void HistWorkThread::ResetModel()
{
	m_pEvoHistorySys->GetHistorySystem()->ApproachHistGen( 0 );
	m_pEvoHistorySys->GetHistorySystem()->ClearHistory( 0 );
}
*/
void HistWorkThread::StopComputation()
{
	m_genDemanded = GetCurrentGeneration();
	WorkThread::StopComputation();
}

void HistWorkThread::DoEdit( GridPoint const gp )
{
	if ( m_pEvoHistorySys->CreateNewGeneration( EvoGenerationCmd( tEvoCmd::editSetXvalue, gp.x ) ) )
		 m_pEvoHistorySys->CreateNewGeneration( EvoGenerationCmd( tEvoCmd::editSetYvalue, gp.y ) );
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
	while ( GetCurrentGeneration( ) != m_genDemanded );
}

void HistWorkThread::PostNextGeneration( )
{
	gotoGeneration( GetCurrentGeneration( ) + 1 );
}

void HistWorkThread::PostHistoryAction( UINT const uiID, GridPoint const gp )
{
    switch ( uiID )
    {
    case IDM_BACKWARDS:
        if ( GetCurrentGeneration( ) > 0 )
            PostGotoGeneration( GetCurrentGeneration( ) - 1 );
        else
            (void)MessageBeep( MB_OK );  // first generation reached
        return;

    case IDM_GOTO_ORIGIN:
        if ( m_pModelWork->IsAlive( gp ) )
            PostGotoGeneration( m_pEvoHistorySys->GetFirstGenOfIndividual( m_pModelWork->GetId( gp ) ) );
        else
            assert( false );
        return;

    case IDM_GOTO_DEATH:
        if ( m_pModelWork->IsAlive( gp ) )
            PostGotoGeneration( m_pEvoHistorySys->GetLastGenOfIndividual( m_pModelWork->GetId( gp ) ) );
        else
            assert( false );
        return;

    default:
        break;
    }

    WorkThread::PostHistoryAction( uiID, gp );
}

void HistWorkThread::PostGotoGeneration( HIST_GENERATION const gen )
{
    if ( m_bTrace )
        *m_pTraceStream << "PostGotoGeneration " << gen.GetLong( ) << endl;

    assert( gen >= 0 );
    assert( gen <= m_pEvoHistorySys->GetHistorySystem( )->GetYoungestGeneration( ) );

	gotoGeneration( gen );
}
