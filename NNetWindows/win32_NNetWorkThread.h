// win32_NNetWorkThread.h
//
// NNetWindows

#pragma once

#include "win32_hiResTimer.h"
#include "win32_WorkThread.h"

class ActionTimer;
class RootWindow;
class WinManager;
class EventInterface;
class SlowMotionRatio;
class NNetModel;
class NNetHistorySysGlue;
class NNetWorkThreadInterface;

class NNetWorkThreadMessage
{
public:
	enum class Id : UINT
	{
		NNET_FIRST = WorkThreadMessage::FIRST_APP_MESSAGE,
		TRIGGER,
		CONNECT,
		HIGHLIGHT,
		SUPER_HIGHLIGHT,
		PULSE_FREQ,
		PULSE_SPEED,
		DAMPING_FACTOR,
	    THRESHHOLD_POTENTIAL,
	    PEAK_VOLTAGE, 
	    PULSE_WIDTH,         
        REFRACTORY_PERIOD,
		MOVE_SHAPE_TO,
		SLOW_MOTION_CHANGED,
		SPLIT_PIPELINE,
		CREATE_NEW_BRANCH,
		CREATE_NEW_NEURON,
		CREATE_NEW_INPUT_NEURON,
		CREATE_NEW_OUTPUT_NEURON,
		NNET_LAST,
		FIRST = NNET_FIRST,
		LAST = NNET_LAST
	};

	static BOOL IsValid( NNetWorkThreadMessage::Id msg )
	{
		return (NNetWorkThreadMessage::Id::FIRST <= msg) && (msg <= NNetWorkThreadMessage::Id::LAST);
	}
};

class NNetWorkThread: public WorkThread
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

private:

	void ResetTimer()
	{
		m_hrTimer.Restart();
	}

	virtual void SetRunModeHook( BOOL const bState ) 
	{
		ResetTimer();
	}
	
	virtual BOOL Dispatch( MSG const );

	virtual void WaitTilNextActivation( );

	virtual void Compute();

	NNetModel       * m_pNNetModel;
	SlowMotionRatio * m_pSlowMotionRatio;
	double            m_dutyCycle;
	HiResTimer        m_hrTimer;
};
