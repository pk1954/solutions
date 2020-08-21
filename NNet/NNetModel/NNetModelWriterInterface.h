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
class CommandStack;
class NNetModelStorage;
struct SoundDescr;

class NNetModelWriterInterface
{
public:
	void Initialize
    ( 
        wostream         * const, 
        CommandStack     * const,
        NNetModelStorage * const
    );

	void Start( NNetModel * const );
	void Stop(); 

    void AnalyzeAnomalies    ( );
    void AnalyzeLoops        ( );
    void AppendInputNeuron   ( ShapeId const );
    void AppendNeuron        ( ShapeId const );
    void AddIncoming2Knot    ( ShapeId const, MicroMeterPoint const & );
    void AddIncoming2Pipe    ( ShapeId const, MicroMeterPoint const & );
    void AddOutgoing2Knot    ( ShapeId const, MicroMeterPoint const & );
    void AddOutgoing2Pipe    ( ShapeId const, MicroMeterPoint const & );
    void ClearBeepers        ( );
    void Connect             ( ShapeId const, ShapeId const );
    void CopySelection       ( );
    void DeleteSelection     ( );
    void DeleteShape         ( ShapeId const );
    void Disconnect          ( ShapeId const );
    void InsertNeuron        ( ShapeId const, MicroMeterPoint const & );
    void MarkSelection       ( tBoolOp const );
    void MoveSelection       ( MicroMeterPoint const & );
    void MoveShape           ( ShapeId const, MicroMeterPoint const & );
    void NewInputNeuron      ( MicroMeterPoint const & );
    void NewNeuron           ( MicroMeterPoint const & );
    void ReadModel           ( bool, wstring const );
    void ResetModel          ( );
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

    NNetModel const & GetModel( ) { return * m_pModel; }

private:

	bool       IsTraceOn  ( ) const { return   m_bTrace; }
	wostream & TraceStream( )       { return * m_pTraceStream; }

    void deleteShape( Shape * const );

	bool               m_bTrace       { true };
	wostream         * m_pTraceStream { nullptr };
	NNetModel        * m_pModel       { nullptr };
    NNetModelStorage * m_pStorage     { nullptr };
    CommandStack     * m_pCmdStack    { nullptr };
}; 
