// win32_worker_thread.h

#pragma once

#include <string>
#include <fstream>
#include <wtypes.h>
#include "gridRect.h"
#include "gridPoint.h"
#include "EvolutionTypes.h"
#include "EvoGenerationCmd.h"

//lint -esym( 763, GridPoint )         redundant declaration
//lint -esym( 763, HiResTimer )        redundant declaration
//lint -esym( 763, EvolutionCore )     redundant declaration
//lint -esym( 763, Script )            redundant declaration

class PerformanceWindow;
class EditorWindow;
class RootWindow;
class EvolutionCore;
class EvolutionModelData;
class StatusBar;
class WinManager;
class DisplayAll;

class WorkThread
{
public:
	// Layer 7 : called by main thread
	
	explicit WorkThread( std::wostream * );
    ~WorkThread( );

    void Start
    ( 
        StatusBar          * const, 
        PerformanceWindow  * const, 
        DisplayAll   const * const, 
        EvolutionCore      * const,
        EvolutionModelData * const
    );

    void PostDoEdit( GridPoint const & );
    void PostSetBrushMode( tBrushMode const );
    void PostSetBrushShape( tShape );
    void PostSetBrushIntensity( INT const );
    void PostSetBrushSize( INT const );
    void PostRefresh( );
    void PostReset( );
    void PostEndThread( HWND );
    void PostProcessScript( std::wstring const & );
	void PostRunGenerations();
	void PostStopComputation();

	void DoProcessScript( std::wstring * const ); // parameter must be created with new, will be deleted here! 

	// functions called by worker thread  ( Layer 1 )

	virtual void PostGenerationStep();
	virtual void GenerationStep( );
	virtual void ApplyEditorCommand( tEvoCmd const, unsigned short const );
	virtual void DoEdit( GridPoint const );

protected:

    enum ThreadMessages
    {
        THREAD_MSG_REFRESH = WM_USER + 1,
        THREAD_MSG_STEP,
        THREAD_MSG_GENERATION_RUN,
        THREAD_MSG_DO_EDIT,
        THREAD_MSG_SET_BRUSH_SIZE,
        THREAD_MSG_SET_BRUSH_INTENSITY,
        THREAD_MSG_SET_BRUSH_SHAPE,
        THREAD_MSG_SET_BRUSH_MODE,
        THREAD_MSG_PROCESS_SCRIPT,
        THREAD_MSG_RESET_MODEL,
        THREAD_MSG_STOP,
        THREAD_MSG_EXIT,
        THREAD_MSG_FIRST = THREAD_MSG_REFRESH,
        THREAD_MSG_LAST = THREAD_MSG_EXIT
    };

    void postMsg2WorkThread( UINT, WPARAM, LPARAM );

	BOOL EditorStateHasChanged( );
	void SaveEditorState( );

    virtual void GenerationRun( );
	virtual void StopComputation();

	BOOL            m_bTrace;
    std::wostream * m_pTraceStream;

private:

    StatusBar          * m_pStatusBar;
    DisplayAll   const * m_pDisplayGridFunctor;
    PerformanceWindow  * m_pPerformanceWindow;
    EvolutionCore      * m_pEvolutionCore;
    EvolutionModelData * m_pModelWork;
    HANDLE               m_hEventThreadStarter;
    DWORD                m_dwThreadId;
    HANDLE               m_hTimer;
    BOOL                 m_bContinue;
    INT                  m_iScriptLevel;

    // private member functions

	DWORD processWorkerMessage( UINT, WPARAM, LPARAM );

friend static DWORD WINAPI WorkerThread( _In_ LPVOID );
}; 
