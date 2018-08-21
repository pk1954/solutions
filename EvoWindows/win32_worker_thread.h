// win32_worker_thread.h

#pragma once

#include <string>
#include <fstream>
#include <wtypes.h>
#include "gridRect.h"
#include "gridPoint.h"
#include "HistoryGeneration.h"
#include "EvolutionTypes.h"
#include "EvoGenerationCmd.h"

//lint -esym( 763, GridPoint )         redundant declaration
//lint -esym( 763, HiResTimer )        redundant declaration
//lint -esym( 763, EvolutionCore )     redundant declaration
//lint -esym( 763, Script )            redundant declaration

class PerformanceWindow;
class EditorWindow;
class RootWindow;
class EvolutionModelData;
class WinManager;
class DisplayAll;
class EditorWindow;
class EvoHistorySys;
class WorkThreadInterface;

class WorkThread
{
public:
	explicit WorkThread( );
    ~WorkThread( );

    void Start
    ( 
        PerformanceWindow   * const,
		EditorWindow        * const,  
        DisplayAll   const  * const, 
        EvolutionModelData  * const,
	    EvoHistorySys       * const,
		WorkThreadInterface * const
    );

	void TerminateThread( HWND const );

	void DoProcessScript( std::wstring * const ); // parameter must be created with new, will be deleted here! 

	HIST_GENERATION GetGenDemanded( ) const 
	{ 
		return m_genDemanded; 
	}

    void GenerationStep( );

    void WorkMessage( UINT, WPARAM, LPARAM );

    enum ThreadMessage : UINT
    {
        THREAD_MSG_REFRESH = WM_USER + 1,
        THREAD_MSG_REPEAT_GENERATION_STEP,  // only used internaly, not part of procedural interface
        THREAD_MSG_GOTO_GENERATION,
        THREAD_MSG_GENERATION_RUN,
        THREAD_MSG_SET_POI,
        THREAD_MSG_DO_EDIT,
        THREAD_MSG_SET_BRUSH_SIZE,
        THREAD_MSG_SET_BRUSH_INTENSITY,
        THREAD_MSG_SET_BRUSH_SHAPE,
        THREAD_MSG_SET_BRUSH_OPERATOR,
        THREAD_MSG_SET_BRUSH_MODE,
        THREAD_MSG_PROCESS_SCRIPT,
        THREAD_MSG_SET_SIMULATION_MODE,
        THREAD_MSG_RESET_MODEL,
        THREAD_MSG_STOP,
        THREAD_MSG_EXIT,
        THREAD_MSG_FIRST = THREAD_MSG_REFRESH,
        THREAD_MSG_LAST = THREAD_MSG_EXIT
    };

private:

	WorkThreadInterface * m_pWorkThreadInterface;
    DisplayAll    const * m_pDisplayGridFunctor;
    PerformanceWindow   * m_pPerformanceWindow;
    EditorWindow        * m_pEditorWindow;
    EvoHistorySys       * m_pEvoHistorySys;
    EvolutionModelData  * m_pModelWork;
    HANDLE                m_hEventThreadStarter;
    DWORD                 m_dwThreadId;
	HANDLE			      m_hThread;
    BOOL                  m_bContinue;
    INT                   m_iScriptLevel;
    HIST_GENERATION       m_genDemanded;

    // private member functions

	void postMessage( UINT, WPARAM, LPARAM );
	void dispatchMessage( UINT, WPARAM, LPARAM );
	void editorCommand( UINT const,  WPARAM const );
    void generationRun( );

	bool IsValidThreadMessage(UINT msg)
	{
		return (THREAD_MSG_FIRST <= msg) && (msg <= THREAD_MSG_LAST);
	}

friend static DWORD WINAPI WorkerThread( _In_ LPVOID );
}; 
