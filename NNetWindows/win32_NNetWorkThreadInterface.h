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

	void PostSetParameter         ( tParameter const, float const );
	void PostResetTimer           ( );
	void PostConnect              ( );
	void PostDeleteShape          ( );
	void PostSlowMotionChanged    ( );
	void PostHighlight            ( ShapeId const );
	void PostSuperHighlight       ( ShapeId const );
	void PostMoveShape            ( ShapeId const, MicroMeterPoint const & );
	void PostCreateNewNeuron      ( MicroMeterPoint const & );
	void PostCreateNewInputNeuron ( MicroMeterPoint const & );
	void PostCreateNewOutputNeuron( MicroMeterPoint const & );
	void PostSplitPipeline        ( MicroMeterPoint const & );

private:
	NNetWorkThread  * m_pNNetWorkThread;
	NNetModel const * m_pModel;
}; 
