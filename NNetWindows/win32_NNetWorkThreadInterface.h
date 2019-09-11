// win32_NNetWorkThreadInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "win32_WorkThreadInterface.h"

class Delay;
class ActionTimer;
class ObserverInterface;
class EventInterface;
class NNetHistorySysGlue;
class NNetWorkThread;

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
		Delay              * const,
		ObserverInterface  * const, 
	    NNetHistorySysGlue * const
    );

	void PostUndo();
	void PostRedo();

private:
	NNetWorkThread * m_pNNetWorkThread;
}; 
