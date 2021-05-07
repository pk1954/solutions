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
#include "NobPtrList.h"
#include "NobType.h"
#include "NobId.h"

class Param;
class Command;
class Observable;
class NobIdList;
class CommandStack;
class AlignAnimation;
class NNetModelImporter;
class NNetModelReaderInterface;
class NNetModelWriterInterface;
class MicroMeterPointVector;

struct SoundDescr;

using std::wcout;
using std::wstring;
using std::wostream;
using std::vector;
using std::unique_ptr;

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

    void AddIncoming2Knot    ( NobId const, MicroMeterPoint const & );
    void AddIncoming2Pipe    ( NobId const, MicroMeterPoint const & );
    void AddModel            ();
    void AddSignal           ( MicroMeterCircle const &, TrackNr const );
    void AddOutgoing2Knot    ( NobId const, MicroMeterPoint const & );
    void AddOutgoing2Pipe    ( NobId const, MicroMeterPoint const & );
    void AnalyzeAnomalies    ();
    void AnalyzeLoops        ();
    void AppendInputNeuron   ( NobId const );
    void AppendNeuron        ( NobId const );
    void ClearBeepers        ();
    void CopySelection       ();
    void CreateInitialNobs ();
    void DeleteSelection     ();
    void DeleteNob         ( NobId  const );
    void DeleteSignal        ( SignalId const );
    void DeleteTrack         ( TrackNr  const );
    void Disconnect          ( NobId  const );
    void InsertKnot          ( NobId  const, MicroMeterPoint const & );
    void InsertNeuron        ( NobId  const, MicroMeterPoint const & );
    void InsertTrack         ( TrackNr  const );
    void MoveSelection       ( MicroMeterPoint const & );
    void MoveNob           ( NobId const, MicroMeterPoint const & );
    void NewInputNeuron      ( MicroMeterPoint const & );
    void NewNeuron           ( MicroMeterPoint const & );
    void NewOutputNeuron     ( MicroMeterPoint const & );
    void ResetModel          ();
    void RestrictSelection   ( NobType::Value const );
    void Rotate              ( NobId const, MicroMeterPoint const &, MicroMeterPoint const & );
    void SelectAll           ( bool const );
    void SelectAllBeepers    ();
    void SelectNob         ( NobId const, tBoolOp const );
    void SelectNobsInRect  ( MicroMeterRect const &, bool const );
    void SelectSubtree       ( NobId const, bool const );
    void SetConnectionNeurons( MicroMeterPointVector const &, unique_ptr<NobIdList> );
    void SetParameter        ( ParamType::Value const, float const );
    void SetPulseRate        ( NobId    const, fHertz const );
    void SetNob            ( NobId const, MicroMeterPosDir const );
    void SetTriggerSound     ( NobId const, SoundDescr const & );
    void ToggleStopOnTrigger ( NobId const );

    void UndoCommand();
    void RedoCommand();

    void PushCommand(unique_ptr<Command>);

private:

    bool       IsTraceOn  () const { return m_bTrace; }
    wostream & TraceStream()       { return wcout; }

    bool                       m_bTrace                  { true };
    CommandStack             * m_pCmdStack               { nullptr };
    NNetModelReaderInterface * m_pNMRI                   { nullptr };
    NNetModelWriterInterface * m_pNMWI                   { nullptr };
    NNetModelImporter        * m_pModelImporter          { nullptr };
    Observable               * m_pDynamicModelObservable { nullptr };
};