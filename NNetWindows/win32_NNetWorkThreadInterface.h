// win32_NNetWorkThreadInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "win32_WorkThreadInterface.h"

class ActionTimer;
class ObserverInterface;
class EventInterface;
class NNetHistorySysGlue;
class NNetWorkThread;
class SlowMotionRatio;

class NNetWorkThreadInterface: public WorkThreadInterface
{
public:
	NNetWorkThreadInterface( );
    ~NNetWorkThreadInterface( );

	void Start
    ( 
		HWND                 const,
		ActionTimer        * const,
		EventInterface     * const,
		ObserverInterface  * const,
		SlowMotionRatio    * const,
	    NNetHistorySysGlue * const
    );

private:
	NNetWorkThread * m_pNNetWorkThread;
}; 
