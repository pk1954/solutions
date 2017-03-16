// win32_worker_thread.h

#pragma once

#include <string>
#include <fstream>
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
class ModelData;
class StatusBar;
class WinManager;
class DisplayAll;

class WorkThread
{
public:
    explicit WorkThread( std::wofstream * );
    ~WorkThread( );

    void Start
    ( 
        StatusBar         * const, 
        EditorWindow      * const,
        PerformanceWindow * const, 
        DisplayAll  const * const, 
        EvolutionCore     * const,
        ModelData         * const
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
	void PostNextGeneration();
	void PostRunGenerations();
	void PostStopComputation();

    void SetGenerationDelay( DWORD );
	void ResetModel();
	void GenerationStep();

	virtual void ApplyEditorCommand( tEvoCmd const, short const );

protected:

    enum ThreadMessages
    {
        THREAD_MSG_REFRESH = WM_USER + 1,
        THREAD_MSG_FORWARD_STEP,
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

    virtual void GenerationRun( );
	virtual void StopComputation();
	virtual void DoEdit( GridPoint const );
	virtual void DoExit( HWND );


	BOOL             m_bTrace;
    std::wofstream * m_pTraceStream;
    EditorWindow   * m_pEditorWindow;

private:

    StatusBar         * m_pStatusBar;
    DisplayAll  const * m_pDisplayGridFunctor;
    PerformanceWindow * m_pPerformanceWindow;
    EvolutionCore     * m_pEvolutionCore;
    ModelData         * m_pModelWork;
    HANDLE              m_hEventThreadStarter;
    DWORD               m_dwThreadId;
    GridRect            m_gridRectSelection;
    HANDLE              m_hTimer;
    BOOL                m_bContinue;
    INT                 m_iScriptLevel;
    GridPoint           m_gpEdit;

    // private member functions

	DWORD processWorkerMessage( UINT, WPARAM, LPARAM );
	void  processScript( std::wstring * const );

friend static DWORD WINAPI WorkerThread( _In_ LPVOID );
}; 
