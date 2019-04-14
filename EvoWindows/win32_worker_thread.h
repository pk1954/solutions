// win32_worker_thread.h
//
// EvoWindows

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

class ColorManager;
class PerformanceWindow;
class EditorWindow;
class RootWindow;
class EvolutionCore;
class WinManager;
class EditorWindow;
class EventInterface;
class ReadBuffer;
class EvoHistorySysGlue;
class WorkThreadInterface;

class WorkerThreadMessage
{
public:
	enum class Id : UINT
	{
		REFRESH = WM_APP,
		STOP,
		REPEAT_GENERATION_STEP,  // only used internaly, not part of procedural interface
		GOTO_GENERATION,
		GENERATION_RUN,
		BENCHMARK,
		SET_POI,
		DO_EDIT,
		SET_BRUSH_RADIUS,
		SET_BRUSH_INTENSITY,
		SET_BRUSH_SHAPE,
		SET_BRUSH_OPERATOR,
		SET_BRUSH_MODE,
		SET_STRATEGY_COLOR,
		SET_SELECTION_COLOR,
		SET_HIGHLIGHT_COLOR,
		PROCESS_SCRIPT,
		SET_SIMULATION_MODE,
		RESET_MODEL,
		FIRST = REFRESH,
		LAST = RESET_MODEL
	};

	static BOOL IsValid( UINT msg )
	{
		Id const workerMsg = static_cast<Id>( msg );
		return (WorkerThreadMessage::Id::FIRST <= workerMsg) && (workerMsg <= WorkerThreadMessage::Id::LAST);
	}
};

class WorkThread: public Util::Thread
{
public:
	WorkThread
	( 
		HWND                  const,
		ColorManager        * const,
		PerformanceWindow   * const,
		EditorWindow        * const,
		EventInterface      * const,
		ReadBuffer          * const,
		EvoHistorySysGlue   * const,
		WorkThreadInterface * const
	);
	~WorkThread( );

	virtual void ThreadStartupFunc( );
	virtual void ThreadMsgDispatcher( MSG const );
	
	// WorkMessage - process incoming messages from main thread

	void WorkMessage( WorkerThreadMessage::Id const, WPARAM const, LPARAM const );
	void WorkMessage( MSG const );

	void GenerationStep( );
	void NGenerationSteps( int ); 

	void DoProcessScript( wstring * const );

	HIST_GENERATION GetGenDemanded( ) const 
	{ 
		return m_genDemanded; 
	}

	BOOL IsRunning() const
	{
		return m_bContinue;
	}

private:
	BOOL editorCommand( tEvoCmd const cmd, WPARAM const wParam )
	{
		return m_pEvoHistGlue->EvoCreateEditorCommand( EvoHistorySysGlue::EvoCmd( cmd, Int24(CastToUnsignedInt(wParam)) ) );
	}

	BOOL editorCommand( tEvoCmd const cmd, GridPoint24 const gp24 )
	{
		return m_pEvoHistGlue->EvoCreateEditorCommand( EvoHistorySysGlue::EvoCmd( cmd, gp24 ) );
	}

	void generationRun( );
	void dispatch( MSG const );

	ColorManager        * m_pColorManager;
    PerformanceWindow   * m_pPerformanceWindow;
    EditorWindow        * m_pEditorWindow;
    EventInterface      * m_pEventPOI;
    ReadBuffer          * m_pReadBuffer;
    EvoHistorySysGlue   * m_pEvoHistGlue;
	WorkThreadInterface * m_pWorkThreadInterface;
    HIST_GENERATION       m_genDemanded;
    BOOL                  m_bContinue;
    INT                   m_iScriptLevel;
	HWND                  m_hwndApplication;
};
