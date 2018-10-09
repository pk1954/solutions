// win32_worker_thread.h

#pragma once

#include <string>
#include <fstream>
#include <wtypes.h>
#include "util.h"
#include "assert.h"
#include "gridRect.h"
#include "gridPoint.h"
#include "GridPoint24.h"
#include "HistoryGeneration.h"
#include "EvolutionTypes.h"
#include "EvoGenerationCmd.h"
#include "EvoHistorySysGlue.h"
#include "win32_thread.h"
#include "win32_event.h"

class PerformanceWindow;
class EditorWindow;
class RootWindow;
class EvolutionCore;
class WinManager;
class EditorWindow;
class EventInterface;
class ObserverInterface;
class EvoHistorySysGlue;
class WorkThreadInterface;

class WorkThread : public Util::Thread
{
public:
	explicit WorkThread( );
    ~WorkThread( );

    void Start
    ( 
        PerformanceWindow   * const,
		EditorWindow        * const,  
        EventInterface      * const, 
        ObserverInterface   * const, 
        EvolutionCore       * const,
	    EvoHistorySysGlue   * const,
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

    enum WorkerThreadMessage : UINT
    {
        THREAD_MSG_REFRESH = Util::Thread::THREAD_MSG_APP_FIRST,
        THREAD_MSG_STOP,
        THREAD_MSG_REPEAT_GENERATION_STEP,  // only used internaly, not part of procedural interface
        THREAD_MSG_GOTO_GENERATION,
        THREAD_MSG_GENERATION_RUN,
        THREAD_MSG_SET_POI,
        THREAD_MSG_DO_EDIT,
        THREAD_MSG_SET_BRUSH_RADIUS,
        THREAD_MSG_SET_BRUSH_INTENSITY,
        THREAD_MSG_SET_BRUSH_SHAPE,
        THREAD_MSG_SET_BRUSH_OPERATOR,
        THREAD_MSG_SET_BRUSH_MODE,
        THREAD_MSG_PROCESS_SCRIPT,
        THREAD_MSG_SET_SIMULATION_MODE,
        THREAD_MSG_RESET_MODEL,
        THREAD_MSG_FIRST = THREAD_MSG_REFRESH,
        THREAD_MSG_LAST = THREAD_MSG_RESET_MODEL
    };

private:

	WorkThreadInterface * m_pWorkThreadInterface;
    PerformanceWindow   * m_pPerformanceWindow;
    EditorWindow        * m_pEditorWindow;
    EvoHistorySysGlue   * m_pEvoHistGlue;
    ObserverInterface   * m_pObservers;
    EventInterface      * m_pEvent;
    EvolutionCore       * m_pCore;
    BOOL                  m_bContinue;
    INT                   m_iScriptLevel;
    HIST_GENERATION       m_genDemanded;

    // private member functions

	virtual LRESULT DispatchThreadMsg( UINT const, WPARAM const, LPARAM const );

    void generationRun( );

	BOOL IsValidThreadMessage(UINT msg)
	{
		return (THREAD_MSG_FIRST <= msg) && (msg <= THREAD_MSG_LAST);
	}

	BOOL editorCommand( tEvoCmd const cmd, WPARAM const wParam )
	{
		return m_pEvoHistGlue->EvoCreateEditorCommand( EvoHistorySysGlue::EvoCmd( cmd, Int24(CastToUnsignedInt(wParam)) ) );
	}

	BOOL editorCommand( tEvoCmd const cmd, GridPoint24 const gp24 )
	{
		return m_pEvoHistGlue->EvoCreateEditorCommand( EvoHistorySysGlue::EvoCmd( cmd, gp24 ) );
	}
}; 
