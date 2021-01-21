// NNetModelCommands.h
//
// NNetModel

#pragma once

#include <string>
#include "boolOp.h"
#include "ParameterType.h"
#include "MoreTypes.h"
#include "MonitorData.h"
#include "ShapeId.h"

class Param;
class Observable;
class CommandStack;
class NNetModelImporter;
class NNetModelReaderInterface;
class NNetModelWriterInterface;

struct SoundDescr;

using std::wcout;
using std::wstring;
using std::wostream;

class NNetModelCommands
{
public:
    void Initialize
    ( 
        NNetModelReaderInterface * const, 
        NNetModelWriterInterface * const,
        NNetModelImporter        * const,
        Observable               * const,
        CommandStack             * const 
    );

    void AddIncoming2Knot    ( ShapeId const, MicroMeterPoint const & );
    void AddIncoming2Pipe    ( ShapeId const, MicroMeterPoint const & );
    void AddModel            ( );
    void AddOutgoing2Knot    ( ShapeId const, MicroMeterPoint const & );
    void AddOutgoing2Pipe    ( ShapeId const, MicroMeterPoint const & );
    void AnalyzeAnomalies    ( );
    void AnalyzeLoops        ( );
    void AppendInputNeuron   ( ShapeId const );
    void AppendNeuron        ( ShapeId const );
    void ClearBeepers        ( );
    void Connect             ( ShapeId const, ShapeId const );
    void CopySelection       ( );
    void DeleteSelection     ( );
    void DeleteShape         ( ShapeId const );
    void Disconnect          ( ShapeId const );
    void InsertKnot          ( ShapeId const, MicroMeterPoint const & );
    void InsertNeuron        ( ShapeId const, MicroMeterPoint const & );
    void MoveSelection       ( MicroMeterPoint const & );
    void MoveShape           ( ShapeId const, MicroMeterPoint const & );
    void NewInputNeuron      ( MicroMeterPoint const & );
    void NewNeuron           ( MicroMeterPoint const & );
    void ResetModel          ( );
    void SelectAll           ( tBoolOp const );
    void SelectAllBeepers    ( );
    void SelectShape         ( ShapeId const, tBoolOp const );
    void SelectShapesInRect  ( MicroMeterRect const &, bool const );
    void SelectSubtree       ( ShapeId const, tBoolOp const );
    void SetParameter        ( ParameterType::Value const, float const );
    void SetPulseRate        ( ShapeId    const, fHertz const );
    void SetTriggerSound     ( ShapeId const, SoundDescr const & );
    void ToggleStopOnTrigger ( ShapeId const );

    void UndoCommand();
    void RedoCommand();

private:
    void deleteShape( ShapeId const );

    bool       IsTraceOn  ( ) const { return m_bTrace; }
    wostream & TraceStream( )       { return wcout; }

    bool                       m_bTrace                  { true };
    CommandStack             * m_pCmdStack               { nullptr };
    NNetModelReaderInterface * m_pNMRI                   { nullptr };
    NNetModelWriterInterface * m_pNMWI                   { nullptr };
    NNetModelImporter        * m_pModelImporter          { nullptr };
    Observable               * m_pDynamicModelObservable { nullptr };
};