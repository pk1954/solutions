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
		HIGHLIGHT,
		PULSE_FREQ,
		PULSE_SPEED,
		MOVE_SHAPE_TO,
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
		NNetModel               * const
	);
	~NNetWorkThread( );

private:

	//GenerationCmd NNetCmd( NNetGenerationCmd::Id const cmd, Int24 const param )
	//{ 
	//	return GenerationCmd::ApplicationCmd( static_cast<GenerationCmd::Id>(cmd), param );  
	//}  

	//void editorCommand( NNetGenerationCmd::Id const cmd, WPARAM const wParam )
	//{
	//	EditorCommand( static_cast<GenerationCmd::Id>(cmd), wParam );
	//}

	virtual void SetRunModeHook( BOOL const bState ) 
	{
		m_timerTicks = Ticks( 0 );
	}
	
	virtual BOOL Dispatch( MSG const );

	virtual void WaitTilNextActivation( );

	NNetModel       * m_pNNetModel;
	SlowMotionRatio * m_pSlowMotionRatio;
	Ticks             m_timerTicks;
	Ticks             m_timerTicksLastTime;
	double            m_dutyCycle;
	HiResTimer        m_hrTimer;
};
