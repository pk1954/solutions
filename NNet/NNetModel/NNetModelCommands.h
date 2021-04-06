// NNetModelCommands.h
//
// NNetModel

#pragma once

#include <string>
#include "boolOp.h"
#include "ParameterType.h"
#include "MoreTypes.h"
#include "MonitorData.h"
#include "ConnectionNeuron.h"
#include "ShapePtrList.h"
#include "ShapeType.h"
#include "ShapeId.h"

class Param;
class Observable;
class CommandStack;
class NNetModelImporter;
class NNetModelReaderInterface;
class NNetModelWriterInterface;
class MicroMeterPointVector;

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
    void AddModel            ();
    void AddSignal           ( MicroMeterCircle const &, TrackNr const );
    void AddOutgoing2Knot    ( ShapeId const, MicroMeterPoint const & );
    void AddOutgoing2Pipe    ( ShapeId const, MicroMeterPoint const & );
    void AnalyzeAnomalies    ();
    void AnalyzeLoops        ();
    void AppendInputNeuron   ( ShapeId const );
    void AppendNeuron        ( ShapeId const );
    void ClearBeepers        ();
    void Connect             ( ShapeId const, ShapeId const );
    void CopySelection       ();
    void CreateConnector     ( ShapePtrList<ConnNeuron> & );
    void CreateInitialShapes ();
    void DeleteSelection     ();
    void DeleteShape         ( ShapeId  const );
    void DeleteSignal        ( SignalId const );
    void DeleteTrack         ( TrackNr  const );
    void Disconnect          ( ShapeId  const );
    void InsertKnot          ( ShapeId  const, MicroMeterPoint const & );
    void InsertNeuron        ( ShapeId  const, MicroMeterPoint const & );
    void InsertTrack         ( TrackNr  const );
    void MoveSelection       ( MicroMeterPoint const & );
    void MoveShape           ( ShapeId const, MicroMeterPoint const & );
    void NewInputNeuron      ( MicroMeterPoint const & );
    void NewNeuron           ( MicroMeterPoint const & );
    void NewOutputNeuron     ( MicroMeterPoint const & );
    void ResetModel          ();
    void RestrictSelection   ( ShapeType::Value const );
    void SelectAll           ( bool const );
    void SelectAllBeepers    ();
    void SelectShape         ( ShapeId const, tBoolOp const );
    void SelectShapesInRect  ( MicroMeterRect const &, bool const );
    void SelectSubtree       ( ShapeId const, bool const );
    void SetConnectionNeurons( MicroMeterPointVector const &, ShapePtrList<ConnNeuron> & );
    void SetParameter        ( ParamType::Value const, float const );
    void SetPulseRate        ( ShapeId    const, fHertz const );
    void SetTriggerSound     ( ShapeId const, SoundDescr const & );
    void ToggleStopOnTrigger ( ShapeId const );

    void UndoCommand();
    void RedoCommand();

    void OpenSeries ();
    void CloseSeries();

private:
    void deleteShape( ShapeId const );

    bool       IsTraceOn  () const { return m_bTrace; }
    wostream & TraceStream()       { return wcout; }

    bool                       m_bTrace                  { true };
    CommandStack             * m_pCmdStack               { nullptr };
    NNetModelReaderInterface * m_pNMRI                   { nullptr };
    NNetModelWriterInterface * m_pNMWI                   { nullptr };
    NNetModelImporter        * m_pModelImporter          { nullptr };
    Observable               * m_pDynamicModelObservable { nullptr };
};