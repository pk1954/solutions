// win32_NNetWorkThreadInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "Shape.h"
#include "win32_WorkThreadInterface.h"

class ActionTimer;
class ObserverInterface;
class EventInterface;
class NNetWorkThread;
class NNetModel;
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
		NNetModel          * const 
	);

	void PostHighlight     ( ShapeId const );
	void PostPulseFrequency( ShapeId const, Hertz            const );
	void PostPulseSpeed    ( ShapeId const, milliMeterPerSec const );

private:
	NNetWorkThread * m_pNNetWorkThread;
}; 
