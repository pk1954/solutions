// win32_worker_thread.h
//
// NNetWindows

#pragma once

#include "util.h"
#include "ModelInterface.h"
#include "win32_thread.h"

class ActionTimer;
class EvoEditorWindow;
class RootWindow;
class WinManager;
class EventInterface;
class ObserverInterface;
class NNetWorkThreadInterface;

class WorkThreadMessage
{
public:
	enum class Id : UINT
	{
		REFRESH = WM_APP,
		STOP,
		REPEAT_NEXT_GENERATION,  // only used internally, not part of procedural interface
		GENERATION_RUN,
		NEXT_GENERATION,
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
		ObserverInterface   * const,
		ModelInterface      * const,
		NNetWorkThreadInterface * const,
		BOOL                  const
	);
	~WorkThread( );

	virtual void ThreadStartupFunc( );
	virtual void ThreadMsgDispatcher( MSG const );
	
	// WorkMessage - process incoming messages from main thread

	void WorkMessage( BOOL const, WorkThreadMessage::Id const, WPARAM const, LPARAM const );

	BOOL IsRunning() const
	{
		return m_bContinue;
	}

	void Continue( )
	{
		if ( m_pEventPOI != nullptr )
			m_pEventPOI->Continue( );     // trigger worker thread if waiting on POI event
	}

protected:
	virtual BOOL Dispatch( MSG const );

	virtual void Compute() { m_pModel->Compute(); }

	virtual void WaitTilNextActivation( ) = 0;

	virtual void SetRunModeHook( BOOL const ) {};  // hook for application when run mode starts/stops

private:

	void generationRun( bool const );
	void generationStop( );

	void setRunMode( BOOL const bState )
	{
		m_bContinue = bState;
		SetRunModeHook( bState );
	}

	ActionTimer         * m_pComputeTimer;
    EventInterface      * m_pEventPOI;
	ObserverInterface   * m_pObserver;
	ModelInterface      * m_pModel;
	NNetWorkThreadInterface * m_pWorkThreadInterface;
    BOOL                  m_bContinue;
	HWND                  m_hwndApplication;
};
