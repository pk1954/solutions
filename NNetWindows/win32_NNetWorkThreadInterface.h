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

	void PostSetParameter     ( tParameter const, float const );
	void PostResetTimer       ( );
	void PostConnect          ( );
	void PostRemoveShape      ( );
	void PostSlowMotionChanged( );
	void PostHighlight        ( ShapeId const, bool const );
	void PostMoveShape        ( ShapeId const, MicroMeterPoint const & );
	void PostSplitPipeline    ( MicroMeterPoint const & );
	void PostInsertNeuron     ( MicroMeterPoint const & );
	void PostAddOutputNeuron  ( MicroMeterPoint const & );
	void PostAddInputNeuron   ( MicroMeterPoint const & );

	bool const HighlightedShapeCanBeDeleted( ) const 
	{ 
		return m_pModel->HighlightedShapeCanBeDeleted();
	}

private:
	NNetWorkThread  * m_pNNetWorkThread;
	NNetModel const * m_pModel;
}; 
