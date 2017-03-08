// win32_histWorkerThread.cpp

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "resource.h"
#include "SYMTAB.h"
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
    m_pModelWork( pModel ),
    m_hThreadSlotAllocator( nullptr ),
    m_bContinueSlotAllocation( FALSE )
{ }

HistWorkThread::~HistWorkThread( )
{}

// generationStep - perform one history step towards demanded generation
//             - update editor state if neccessary

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

bool HistWorkThread::setEditParam( EvoGenerationCmd const genCmd )
{
    //    _TRACE_FUNCTION_( genCmd.GetEvoCommand( ) );

    if ( m_pEvoHistorySys->CreateNewGeneration( genCmd ) )
    {
        m_pCore->SaveEditorState( m_pModelWork );
        return true;
    }

    return false;
}

DWORD HistWorkThread::processWorkerMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uiMsg )
    {
    case THREAD_MSG_HIST_GOTO_GEN:
        m_genDemanded = static_cast<long>( lParam );
    // fall through!

    case THREAD_MSG_HIST_GOTO_DEM: // separat from THREAD_MSG_HIST_GOTO_GEN to allow THREAD_MSG_STOP
        generationStep( );
        if ( GetCurrentGeneration( ) != m_genDemanded )
            postMsg2WorkThread( THREAD_MSG_HIST_GOTO_DEM, 0, 0 );
        return 0;

    case THREAD_MSG_STOP:
        m_genDemanded = GetCurrentGeneration( );
        break;

    case THREAD_MSG_DO_EDIT:
        if ( m_pEvoHistorySys->CreateNewGeneration( EvoGenerationCmd( tEvoCmd::editSetXvalue, static_cast<SHORT>( wParam ) ) ) )
             m_pEvoHistorySys->CreateNewGeneration( EvoGenerationCmd( tEvoCmd::editSetYvalue, static_cast<SHORT>( lParam ) ) );
        return 0;

    case THREAD_MSG_SET_BRUSH_INTENSITY:
        setEditParam( EvoGenerationCmd( tEvoCmd::editSetBrushIntensity, static_cast<SHORT>( wParam ) ) );
        return 0;

    case THREAD_MSG_SET_BRUSH_SIZE:
        setEditParam( EvoGenerationCmd( tEvoCmd::editSetBrushSize, static_cast<SHORT>( wParam ) ) );
        return 0;

    case THREAD_MSG_SET_BRUSH_MODE:
        setEditParam( EvoGenerationCmd( tEvoCmd::editSetBrushMode, static_cast<SHORT>( wParam ) ) );
        return 0;

    case THREAD_MSG_EXIT:
        m_bContinueSlotAllocation = FALSE;
        WaitForSingleObject( m_hThreadSlotAllocator, INFINITE );
        CloseHandle( m_hThreadSlotAllocator );
        break;

    default:
        break;
    }

    return WorkThread::processWorkerMessage( uiMsg, wParam, lParam );
}

void HistWorkThread::PostNextGeneration( )
{
    m_genDemanded = GetCurrentGeneration( ) + 1;
    WorkThread::PostNextGeneration( );
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

    postMsg2WorkThread( THREAD_MSG_HIST_GOTO_GEN, 0, gen.GetLong( ) );
}

static DWORD WINAPI HistAllocThread( _In_ LPVOID lpParameter )
{
    HistWorkThread * const pWorkThread = static_cast<HistWorkThread *>( lpParameter );
    EvoHistorySys  * const pHistSys = pWorkThread->GetEvoHistorySys( );

    (void)SetThreadAffinityMask( GetCurrentThread( ), 0x0003 );

    while ( pWorkThread->ContinueSlotAllocation( ) && pHistSys->AddEvoHistorySlot( ) );

    return 0;
}

void HistWorkThread::PostAllocateHistorySlots( )
{
    DWORD m_dwThreadId;
    m_bContinueSlotAllocation = TRUE;
    m_hThreadSlotAllocator = Util::MakeThread( HistAllocThread, this, & m_dwThreadId, nullptr );
}
