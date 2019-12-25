// win32_NNetWorkThreadInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "Shape.h"
#include "NNetModel.h"
#include "win32_WorkThreadInterface.h"

class ActionTimer;
class ObserverInterface;
class EventInterface;
class NNetWorkThread;
class SlowMotionRatio;

class NNetWorkThreadInterface: public WorkThreadInterface
{
public:
	NNetWorkThreadInterface( );
    ~NNetWorkThreadInterface( );

	void Start
    ( 
		HWND                const,
		ActionTimer       * const,
		EventInterface    * const,
		ObserverInterface * const,
		SlowMotionRatio   * const,
		NNetModel         * const,
		BOOL                const
	);
	void Stop(); 

	void PostSetPulseRate     ( ShapeId    const, float const );
	void PostSetParameter     ( tParameter const, float const );
	void PostResetTimer       ( );
	void PostConnect          ( ShapeId const, ShapeId const );
	void PostRemoveShape      ( ShapeId const );
	void PostDisconnect       ( ShapeId const );
	void PostSlowMotionChanged( );
	void PostResetModel       ( );
	void PostMoveShape        ( ShapeId const, MicroMeterPoint const & );
	void PostActionCommand    ( int const , ShapeId const, LPARAM const );

private:
	NNetWorkThread  * m_pNNetWorkThread;
	NNetModel const * m_pModel;
}; 
