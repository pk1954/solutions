// win32_NNetWorkThread.h
//
// NNetWindows

#pragma once

#include "win32_hiResTimer.h"
#include "Observable.h"
#include "NNetModel.h"
#include "win32_thread.h"

class ActionTimer;
class RootWindow;
class WinManager;
class EventInterface;
class SlowMotionRatio;
class NNetHistorySysGlue;
class NNetWorkThreadInterface;

class NNetWorkThreadMessage
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
		RESET_TIMER,
		TRIGGER,
		CONNECT,
		REMOVE_SHAPE,
		DISCONNECT,
		PULSE_RATE,
		PULSE_SPEED,
		DAMPING_FACTOR,
	    THRESHOLD,
	    PEAK_VOLTAGE, 
	    PULSE_WIDTH,         
        REFRACTORY_PERIOD,
		MOVE_SHAPE,
		SLOW_MOTION_CHANGED,
		NEW_NEURON,
		NEW_INPUT_NEURON,
		APPEND_NEURON,
		APPEND_INPUT_NEURON,
		ADD_OUTGOING2KNOT,
		ADD_INCOMING2KNOT,
		ADD_OUTGOING2PIPE,
		ADD_INCOMING2PIPE,
		INSERT_NEURON,
		NNET_LAST,
		FIRST = REFRESH,
		LAST = NNET_LAST
	};

	static BOOL IsValid( NNetWorkThreadMessage::Id msg )
	{
		return (NNetWorkThreadMessage::Id::FIRST <= msg) && (msg <= NNetWorkThreadMessage::Id::LAST);
	}
};

class NNetWorkThread: public Util::Thread
{
public:
	NNetWorkThread
	( 
		HWND                      const,
		ActionTimer             * const,
		EventInterface          * const,
		ObserverInterface       * const,
		SlowMotionRatio         * const,
		NNetWorkThreadInterface * const,
		NNetModel               * const,
		BOOL                      const
	);
	~NNetWorkThread( );

	virtual void ThreadStartupFunc( );
	virtual void ThreadMsgDispatcher( MSG const );


	BOOL   IsRunning()    const { return m_bContinue; }
	double GetDutyCycle() const { return m_dDutyCycle; }

	void Continue( )
	{
		if ( m_pEventPOI != nullptr )
			m_pEventPOI->Continue( );     // trigger worker thread if waiting on POI event
	}

	void AddRunObserver( ObserverInterface * pObserver )
	{
		m_runObservable.RegisterObserver( pObserver );
	}

	void AddPerformanceObserver( ObserverInterface * pObserver )
	{
		m_performanceObservable.RegisterObserver( pObserver );
	}

private:

	void compute();
	BOOL dispatch( MSG const );
	void generationRun( bool const );
	void generationStop( );
	bool actionCommand( NNetWorkThreadMessage::Id const, ShapeId const, MicroMeterPoint const & );

	EventInterface          * m_pEventPOI;
	ObserverInterface       * m_pObserver;
	NNetWorkThreadInterface * m_pWorkThreadInterface;
	BOOL                      m_bContinue;
	HWND                      m_hwndApplication;
	NNetModel               * m_pNNetModel;
	SlowMotionRatio         * m_pSlowMotionRatio;
	HiResTimer                m_hrTimer;
	double                    m_dDutyCycle;
	Observable                m_runObservable;
	Observable                m_performanceObservable;
};
