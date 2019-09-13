// win32_worker_thread.h
//
// Toolbox/win32_utilities

#pragma once

#include "util.h"
#include "GenerationCmd.h"
#include "HistorySystem.h"
#include "HistoryGeneration.h"
#include "win32_thread.h"

class Delay;
class ActionTimer;
class EditorWindow;
class RootWindow;
class WinManager;
class HistorySystem;
class EventInterface;
class ObserverInterface;
class WorkThreadInterface;

class WorkThreadMessage
{
public:
	enum class Id : UINT
	{
		REFRESH = WM_APP,
		STOP,
		REPEAT_NEXT_GENERATION,  // only used internally, not part of procedural interface
		GENERATION_RUN,
		GOTO_GENERATION,
		NEXT_GENERATION,
		PREV_GENERATION,
		UNDO_OPERATION,
		REDO_OPERATION,
		RESET_MODEL,
		FIRST = REFRESH,
		LAST = RESET_MODEL
	};

	static UINT const FIRST_APP_MESSAGE = static_cast<UINT>(Id::LAST) + 1;
};

class WorkThread: public Util::Thread
{
public:
	WorkThread
	( 
		HWND                  const,
		ActionTimer         * const,
		EventInterface      * const,
		Delay               * const,
		ObserverInterface   * const,
		HistorySystem       * const,
		WorkThreadInterface * const
	);
	~WorkThread( );

	virtual void ThreadStartupFunc( );
	virtual void ThreadMsgDispatcher( MSG const );
	
	// WorkMessage - process incoming messages from main thread

	void WorkMessage( BOOL const, WorkThreadMessage::Id const, WPARAM const, LPARAM const );

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

	bool IsEditorCommand( HIST_GENERATION const gen )
	{
		return GetHistorySystem()->GetGenerationCmd( gen ).IsAppCommand( );
	}

protected:
	virtual BOOL Dispatch( MSG const );

	void GotoGeneration( HIST_GENERATION const );

	HistorySystem * GetHistorySystem( ) { return m_pHistorySystem; }

	void EditorCommand( GenerationCmd::Id const cmd, WPARAM const wParam )
	{
		GetHistorySystem( )->CreateAppCommand( MakeGenerationCmd( cmd, Int24(CastToUnsignedInt(wParam)) ) );
	}

private:
	void setContinueFlag( BOOL const bState )
	{
		m_bContinue = bState;
	}

	GenerationCmd MakeGenerationCmd( GenerationCmd::Id const cmd, Int24 const param )
	{ 
		return GenerationCmd::ApplicationCmd( static_cast<GenerationCmd::Id>(cmd), param );  
	}  

	void generationRun( );
	bool userWantsHistoryCut( ) const;

	Delay               * m_pDelay;
	ActionTimer         * m_pActionTimer;
    EventInterface      * m_pEventPOI;
	ObserverInterface   * m_pObserver;
    HistorySystem       * m_pHistorySystem;
	WorkThreadInterface * m_pWorkThreadInterface;
    HIST_GENERATION       m_genDemanded;
    BOOL                  m_bContinue;
	HWND                  m_hwndApplication;
};
