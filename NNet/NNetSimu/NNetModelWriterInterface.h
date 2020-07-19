// NNetModelWriterInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "MoreTypes.h"
#include "ShapeId.h"
#include "NNetModel.h"
#include "Command.h"

using std::wostream;

class NNetModel;
class ActionTimer;
class SlowMotionRatio;
class NNetModelStorage;
class PixelCoordsFp;

class NNetModelWriterInterface
{
public:
	void Initialize( std::wostream * );

	void Start( NNetModel * const );
	void Stop(); 

/* !! */ void InsertNeuron        ( ShapeId const, MicroMeterPoint const & );
/* ok */ void Connect             ( ShapeId const, ShapeId const );
/* ok */ void Convert2InputNeuron ( ShapeId const );
/* ok */ void Convert2Neuron      ( ShapeId const );
		 void AppendNeuron        ( ShapeId const );
		 void AppendInputNeuron   ( ShapeId const );
		 void CopySelection       ( );
/* ok */ void AddOutgoing2Knot    ( ShapeId const, MicroMeterPoint const & );
/* ok */ void AddIncoming2Knot    ( ShapeId const, MicroMeterPoint const & );
/* ok */ void AddOutgoing2Pipe    ( ShapeId const, MicroMeterPoint const & );
/* ok */ void AddIncoming2Pipe    ( ShapeId const, MicroMeterPoint const & );
/* ok */ void Analyze             ( int const );
		 void NewNeuron           ( MicroMeterPoint const & );
		 void NewInputNeuron      ( MicroMeterPoint const & );
/* !! */ void DeleteSelection     ( );
		 void Disconnect          ( ShapeId const );
         void MarkSelection       ( tBoolOp const );
/* ok */ void MoveSelection       ( MicroMeterPoint const & );
/* ok */ void MoveShape           ( ShapeId const, MicroMeterPoint const & );
         void ClearBeepers        ( );
/* ok */ void DeleteShape         ( ShapeId const );
/* ok */ void ResetModel          ( );
/* ok */ void ResetTimer          ( );
/* ok */ void SelectAll           ( tBoolOp const );
/* ok */ void SelectAllBeepers    ( );
/* ok */ void SelectShape         ( ShapeId const, tBoolOp const );
/* ok */ void SelectShapesInRect  ( MicroMeterRect const & );
/* ok */ void SelectSubtree       ( ShapeId const, tBoolOp const );
/* ok */ void SetParameter        ( tParameter const, float const );
/* ok */ void SetPulseRate        ( ShapeId    const, fHertz const );
/* ok */ void SetTriggerSound     ( ShapeId const, bool const, Hertz const, MilliSecs const );
/* ok */ void ToggleStopOnTrigger ( ShapeId const );

	void UndoCommand();
	void RedoCommand();

private:

	bool       IsTraceOn  ( ) const { return   m_bTrace; }
	wostream & TraceStream( )       { return * m_pTraceStream; }

	bool        m_bTrace       { true };
	wostream  * m_pTraceStream { nullptr };
	NNetModel * m_pModel       { nullptr };

	CommandStack m_CmdStack {};
}; 
