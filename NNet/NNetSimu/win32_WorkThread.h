// win32_WorkThread.h
//
// NNetSimu

#pragma once

#include <functional>
#include "win32_hiResTimer.h"
#include "Observable.h"
#include "ShapeId.h"
#include "win32_thread.h"

class Param;
class NNetModel;
class ActionTimer;
class SlowMotionRatio;
class EventInterface;
class NNetModelStorage;
class NNetHistorySysGlue;
class WorkThreadInterface;

using tAppCallBack = function<void (int const)>;

class NNetWorkThreadMessage
{
public:
	enum class Id : UINT
	{
		REFRESH = WM_APP,
		STOP,
		SEND_BACK,
		RESET_MODEL,
		READ_MODEL,
		RESET_TIMER,
		TRIGGER,
		CONNECT,
		REMOVE_SHAPE,
		DISCONNECT,
		PULSE_RATE,
		TRIGGER_SOUND,
		PULSE_SPEED,
	    THRESHOLD,
	    PEAK_VOLTAGE, 
	    PULSE_WIDTH,         
		REFRACTORY_PERIOD,
		TIME_RESOLUTION,
		MOVE_SHAPE,
		MOVE_SELECTION,
		SELECT_SHAPES_IN_RECT,
		SELECT_SHAPE,
		SELECT_SUBTREE,
		SELECT_ALL,
		NEW_NEURON,
		NEW_INPUT_NEURON,
		APPEND_NEURON,
		APPEND_INPUT_NEURON,
		ADD_OUTGOING2KNOT,
		ADD_INCOMING2KNOT,
		ADD_OUTGOING2PIPE,
		ADD_INCOMING2PIPE,
		CONVERT2NEURON,
		CONVERT2INPUT_NEURON,
		TOGGLE_STOP_ON_TRIGGER,
		INSERT_NEURON,
		REMOVE_SELECTION,
		REMOVE_BEEPERS,
		SELECT_ALL_BEEPERS,
		MARK_SELECTION,
		COPY_SELECTION,
		ANALYZE_LOOPS,
		ANALYZE_ANOMALIES,
		NNET_LAST,
		FIRST = REFRESH,
		LAST = NNET_LAST
	};

	static bool IsValid( NNetWorkThreadMessage::Id msg )
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
		HWND                  const,
		ActionTimer         * const,
		EventInterface      * const,
		SlowMotionRatio     * const,
		WorkThreadInterface * const,
		NNetModel           * const,
		Param               * const,
		NNetModelStorage    * const,
		bool                  const
	);
	~NNetWorkThread( );

	virtual void ThreadStartupFunc( );
	virtual void ThreadMsgDispatcher( MSG const );

	void Continue( )
	{
		if ( m_pEventPOI != nullptr )
			m_pEventPOI->Continue( );     // trigger worker thread if waiting on POI event
	}

private:

	bool dispatch( MSG const );
	bool actionCommand( NNetWorkThreadMessage::Id const, ShapeId const, MicroMeterPoint const & );

	inline static tAppCallBack m_appCallback { nullptr };

	HWND                  m_hwndApplication         { (HWND)0 };
	NNetModelStorage    * m_pStorage                { nullptr };
	EventInterface      * m_pEventPOI               { nullptr };
	WorkThreadInterface * m_pWorkThreadInterface    { nullptr };
	NNetModel           * m_pNNetModel              { nullptr };
	Param               * m_pParam                  { nullptr };
	SlowMotionRatio     * m_pSlowMotionRatio        { nullptr };
};
