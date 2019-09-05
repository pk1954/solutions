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
#include "ViewCollection.h"
#include "HistoryGeneration.h"
#include "EvolutionTypes.h"
#include "EvoGenerationCmd.h"
//#include "EvoReadBuffer.h"
#include "win32_thread.h"
#include "win32_event.h"

class Delay;
class ActionTimer;
class ColorManager;
class EditorWindow;
class RootWindow;
class EvolutionCore;
class WinManager;
class EventInterface;
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
		GENERATION_RUN,
		GOTO_ORIGIN,
		GOTO_DEATH,
		GOTO_GENERATION,
		GENERATION_STEP,
		PREV_GENERATION,
	    REDO,
		UNDO,
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
		RESET_MODEL,
		FIRST = REFRESH,
		LAST = RESET_MODEL
	};

	static BOOL IsValid( WorkerThreadMessage::Id msg )
	{
		return (WorkerThreadMessage::Id::FIRST <= msg) && (msg <= WorkerThreadMessage::Id::LAST);
	}
};

class WorkThread: public Util::Thread
{
public:
	WorkThread
	( 
		HWND                  const,
		ColorManager        * const,
		ActionTimer         * const,
		EventInterface      * const,
		Delay               * const,
		ObserverInterface   * const,
		EvoHistorySysGlue   * const,
		WorkThreadInterface * const
	);
	~WorkThread( );

	virtual void ThreadStartupFunc( );
	virtual void ThreadMsgDispatcher( MSG const );
	
	// WorkMessage - process incoming messages from main thread

	void WorkMessage( BOOL const, WorkerThreadMessage::Id const, WPARAM const, LPARAM const );

	void NGenerationSteps( int ); 

	HIST_GENERATION GetGenDemanded( ) const 
	{ 
		return m_genDemanded; 
	}

	BOOL IsRunning() const
	{
		return m_bContinue;
	}

	void Continue( )
	{
		if ( m_pEventPOI != nullptr )
			m_pEventPOI->Continue( );     // trigger worker thread if waiting on POI event
	}

private:
	void editorCommand( tEvoCmd const cmd, WPARAM const wParam )
	{
		m_pEvoHistGlue->EvoCreateEditorCommand( EvoHistorySysGlue::EvoCmd( cmd, Int24(CastToUnsignedInt(wParam)) ) );
	}

	void editorCommand( tEvoCmd const cmd, GridPoint24 const gp24 )
	{
		m_pEvoHistGlue->EvoCreateEditorCommand( EvoHistorySysGlue::EvoCmd( cmd, gp24 ) );
	}

	void setContinueFlag( BOOL const bState )
	{
		m_bContinue = bState;
	}

	void gotoGeneration( HIST_GENERATION const );
	void generationRun( );
	void dispatch( MSG const );
	bool userWantsHistoryCut( ) const;

	Delay               * m_pDelay;
	ColorManager        * m_pColorManager;
	ActionTimer         * m_pActionTimer;
    EventInterface      * m_pEventPOI;
	ObserverInterface   * m_pObserver;
    EvoHistorySysGlue   * m_pEvoHistGlue;
	WorkThreadInterface * m_pWorkThreadInterface;
    HIST_GENERATION       m_genDemanded;
    BOOL                  m_bContinue;
	HWND                  m_hwndApplication;
};
