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
		HWND                const,
		ActionTimer       * const,
		EventInterface    * const,
		ObserverInterface * const,
		SlowMotionRatio   * const,
		NNetModel         * const,
		BOOL                const
	);
	void Stop(); 

	void PostConnect              ( );
	void PostSlowMotionChanged    ( );
	void PostSetDampingFactor     ( float        const );  
	void PostSetThresholdPotential( mV           const );
	void PostSetPeakVoltage       ( mV           const );    
	void PostSetPulseWidth        ( microseconds const );     
	void PostSetRefractoryPeriod  ( microseconds const ); 
	void PostPulseSpeed           ( meterPerSec  const );
	void PostTrigger              ( ShapeId const );
	void PostHighlight            ( ShapeId const );
	void PostSuperHighlight       ( ShapeId const );
	void PostPulseFrequency       ( ShapeId const, fHertz           const );
	void PostMoveShape            ( ShapeId const, MicroMeterPoint const & );
	void PostSplitPipeline        ( ShapeId const, MicroMeterPoint const & );
	void PostCreateNewBranch      ( ShapeId const );
	void PostCreateNewNeuron      ( MicroMeterPoint const & );
	void PostCreateNewInputNeuron ( MicroMeterPoint const & );
	void PostCreateNewOutputNeuron( MicroMeterPoint const & );

private:
	NNetWorkThread * m_pNNetWorkThread;
}; 
