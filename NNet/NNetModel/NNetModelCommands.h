// NNetModelCommands.h
//
// NNetModel

#pragma once

#include <string>
#include "BoolOp.h"
#include "ParameterType.h"
#include "MoreTypes.h"
#include "MonitorData.h"
#include "IoNeuron.h"
#include "IoNeuronList.h"
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
class MicroMeterPntVector;

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

    void AddIncoming2Knot   ( NobId const, MicroMeterPnt const & );
    void AddIncoming2Pipe   ( NobId const, MicroMeterPnt const & );
    void AddModel           ();
    void AddSignal          ( MicroMeterCircle const &, TrackNr const );
    void AddOutgoing2Knot   ( NobId const, MicroMeterPnt const & );
    void AddOutgoing2Pipe   ( NobId const, MicroMeterPnt const & );
    void AnalyzeAnomalies   ();
    void AnalyzeLoops       ();
    void AppendInputNeuron  ( NobId const );
    void AppendOutputNeuron ( NobId const );
    void ClearBeepers       ();
    void CopySelection      ();
    void CreateInitialNobs  ();
    void DeleteSelection    ();
    void DeleteNob          ( NobId  const );
    void DeleteSignal       ( SignalId const );
    void DeleteTrack        ( TrackNr  const );
    void Disconnect         ( NobId  const );
    void InsertKnot         ( NobId  const, MicroMeterPnt const & );
    void InsertNeuron       ( NobId  const, MicroMeterPnt const & );
    void InsertTrack        ( TrackNr  const );
    void MoveSelection      ( MicroMeterPnt const & );
    void MoveNob            ( NobId const, MicroMeterPnt const & );
    void NewInputNeuron     ( MicroMeterPnt const & );
    void NewOutputNeuron    ( MicroMeterPnt const & );
    void ResetModel         ();
    void RestrictSelection  ( NobType::Value const );
    void Rotate             ( NobId const, MicroMeterPnt const &, MicroMeterPnt const & );
    void SelectAll          ( bool const );
    void SelectAllBeepers   ();
    void SelectNob          ( NobId const, tBoolOp const );
    void SelectNobsInRect   ( MicroMeterRect const & );
    void SelectSubtree      ( NobId const, bool const );
    void SetIoNeurons       ( MicroMeterPntVector const &, unique_ptr<NobIdList> );
    void SetParameter       ( ParamType::Value const, float const );
    void SetPulseRate       ( NobId    const, fHertz const );
    void SetNob             ( NobId const, MicroMeterPosDir const );
    void SetTriggerSound    ( NobId const, SoundDescr const & );
    void DiscClosedConnector( NobId  const );
    void ToggleStopOnTrigger( NobId const );
    void Unplug             ( NobId  const );

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