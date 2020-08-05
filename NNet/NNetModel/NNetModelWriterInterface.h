// NNetModelWriterInterface.h
//
// NNetModel

#pragma once

#include "boolOp.h"
#include "tParameter.h"
#include "MoreTypes.h"
#include "ShapeId.h"

using std::wostream;

class NNetModel;
class ActionTimer;
class CommandStack;
class SlowMotionRatio;
class NNetModelStorage;
class PixelCoordsFp;
struct SoundDescr;

class NNetModelWriterInterface
{
public:
	void Initialize( wostream * const, CommandStack * const );

	void Start( NNetModel * const );
	void Stop(); 

    void NewNeuron           ( MicroMeterPoint const & );
    void NewInputNeuron      ( MicroMeterPoint const & );
    void AddOutgoing2Knot    ( ShapeId const, MicroMeterPoint const & );
    void AddIncoming2Knot    ( ShapeId const, MicroMeterPoint const & );
    void AddOutgoing2Pipe    ( ShapeId const, MicroMeterPoint const & );
    void AddIncoming2Pipe    ( ShapeId const, MicroMeterPoint const & );
    void Connect             ( ShapeId const, ShapeId const );
    void InsertNeuron        ( ShapeId const, MicroMeterPoint const & );
    void Convert2InputNeuron ( ShapeId const );
    void Convert2Neuron      ( ShapeId const );
    void AppendNeuron        ( ShapeId const );
    void AppendInputNeuron   ( ShapeId const );
    void CopySelection       ( );
    void AnalyzeLoops        ( );
    void AnalyzeAnomalies    ( );
    void DeleteSelection     ( );
    void Disconnect          ( ShapeId const );
    void MarkSelection       ( tBoolOp const );
    void MoveSelection       ( MicroMeterPoint const & );
    void MoveShape           ( ShapeId const, MicroMeterPoint const & );
    void ClearBeepers        ( );
    void DeleteShape         ( ShapeId const );
    void ResetModel          ( );
    void ResetTimer          ( );
    void SelectAll           ( tBoolOp const );
    void SelectAllBeepers    ( );
    void SelectShape         ( ShapeId const, tBoolOp const );
    void SelectShapesInRect  ( MicroMeterRect const & );
    void SelectSubtree       ( ShapeId const, tBoolOp const );
    void SetParameter        ( tParameter const, float const );
    void SetPulseRate        ( ShapeId    const, fHertz const );
    void SetTriggerSound     ( ShapeId const, SoundDescr const & );
    void ToggleStopOnTrigger ( ShapeId const );

	void UndoCommand();
	void RedoCommand();

private:

	bool       IsTraceOn  ( ) const { return   m_bTrace; }
	wostream & TraceStream( )       { return * m_pTraceStream; }

	bool           m_bTrace       { true };
	wostream     * m_pTraceStream { nullptr };
	NNetModel    * m_pModel       { nullptr };
	CommandStack * m_pCmdStack    { nullptr };
}; 
