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

class WorkThread: public Util::Thread
{
public:
	enum WorkerThreadMessage : UINT
	{
		THREAD_MSG_REFRESH = WM_APP,
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

	BOOL IsValidThreadMessage( UINT );

	WorkThread( );
	~WorkThread( );

	void Start
	( 
		PerformanceWindow   * const,
		EditorWindow        * const,
		EventInterface      * const,
		ObserverInterface   * const,
		EvoHistorySysGlue   * const,
		WorkThreadInterface * const
	);

	virtual void ThreadStartupFunc( );
	virtual void ThreadMsgDispatcher( MSG const );
	virtual void ThreadShutDownFunc( );
	
	// WorkMessage - process incoming messages from main thread

	void WorkMessage( UINT const, WPARAM const, LPARAM const );
	void WorkMessage( MSG const );

	void GenerationStep( );
	void generationRun( );

	void DoProcessScript( wstring * const );

	BOOL editorCommand( tEvoCmd const cmd, WPARAM const wParam )
	{
		return m_pEvoHistGlue->EvoCreateEditorCommand( EvoHistorySysGlue::EvoCmd( cmd, Int24(CastToUnsignedInt(wParam)) ) );
	}

	BOOL editorCommand( tEvoCmd const cmd, GridPoint24 const gp24 )
	{
		return m_pEvoHistGlue->EvoCreateEditorCommand( EvoHistorySysGlue::EvoCmd( cmd, gp24 ) );
	}

	HIST_GENERATION GetGenDemanded( ) const 
	{ 
		return m_genDemanded; 
	}

private:
    PerformanceWindow   * m_pPerformanceWindow;
    EditorWindow        * m_pEditorWindow;
    EventInterface      * m_pEventPOI;
    ObserverInterface   * m_pObservers;
    EvoHistorySysGlue   * m_pEvoHistGlue;
	WorkThreadInterface * m_pWorkThreadInterface;
    HIST_GENERATION       m_genDemanded;
    BOOL                  m_bContinue;
    INT                   m_iScriptLevel;
};
