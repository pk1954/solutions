// win32_NNetWorkThread.h
//
// NNetWindows

#pragma once

#include <functional>
#include "win32_hiResTimer.h"
#include "Observable.h"
#include "ShapeId.h"
#include "win32_thread.h"

class NNetModel;
class ActionTimer;
class WinManager;
class SlowMotionRatio;
class EventInterface;
class NNetModelStorage;
class NNetHistorySysGlue;
class NNetWorkThreadInterface;

using tAppCallBack = function<void (int const)>;

class NNetWorkThreadMessage
{
public:
	enum class Id : UINT
	{
		REFRESH = WM_APP,
		STOP,
		SEND_BACK,
		REPEAT_NEXT_GENERATION,  // only used internally, not part of procedural interface
		GENERATION_RUN,
		NEXT_GENERATION,
		RESET_MODEL,
		READ_MODEL,
		RESET_TIMER,
		TRIGGER,
		CONNECT,
		REMOVE_SHAPE,
		DISCONNECT,
		PULSE_RATE,
		PULSE_SPEED,
	    THRESHOLD,
	    PEAK_VOLTAGE, 
	    PULSE_WIDTH,         
		REFRACTORY_PERIOD,
		TIME_RESOLUTION,
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
		ANALYZE,
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
	static void InitClass( tAppCallBack );

	NNetWorkThread
	( 
		HWND                      const,
		ActionTimer             * const,
		EventInterface          * const,
		ObserverInterface       * const,
		SlowMotionRatio         * const,
		NNetWorkThreadInterface * const,
		NNetModel               * const,
		NNetModelStorage        * const,
		BOOL                      const
	);
	~NNetWorkThread( );

	virtual void ThreadStartupFunc( );
	virtual void ThreadMsgDispatcher( MSG const );

	BOOL      IsRunning()              const { return m_bContinue; }
	fMicroSecs GetTimeSpentPerCycle ( ) const { return m_usRealTimeSpentPerCycle; }
	fMicroSecs GetTimeAvailPerCycle ( ) const { return m_usRealTimeAvailPerCycle; }
	fMicroSecs GetRealTimeTilStart  ( ) const { return m_hrTimer.GetMicroSecsTilStart( ); }
	fMicroSecs GetSimuTimeResolution( ) const;
	fMicroSecs GetSimulationTime    ( ) const;
	float     GetSlowMotionRatio   ( ) const;

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

	class TimeResObserver : public ObserverInterface
	{
	public:
		TimeResObserver( NNetWorkThread * const pNNetWorkThread )
			: m_pNNetWorkThread( pNNetWorkThread )
		{}

		virtual void Notify( bool const );

	private:
		NNetWorkThread * const m_pNNetWorkThread;
	};

private:

	void compute();
	BOOL dispatch( MSG const );
	void generationRun( bool const );
	void generationStop( );
	bool actionCommand( NNetWorkThreadMessage::Id const, ShapeId const, MicroMeterPoint const & );

	static tAppCallBack m_appCallback;

	NNetModelStorage        * m_pStorage;
	EventInterface          * m_pEventPOI;
	NNetWorkThreadInterface * m_pWorkThreadInterface;
	BOOL                      m_bContinue;
	HWND                      m_hwndApplication;
	NNetModel               * m_pNNetModel;
	SlowMotionRatio         * m_pSlowMotionRatio;
	HiResTimer                m_hrTimer;
	Observable                m_runObservable;
	Observable                m_performanceObservable;
	TimeResObserver         * m_pTimeResObserver;
	ObserverInterface       * m_pModelObserver;
	fMicroSecs                 m_usRealTimeAvailPerCycle;
	fMicroSecs                 m_usRealTimeSpentPerCycle;
};
