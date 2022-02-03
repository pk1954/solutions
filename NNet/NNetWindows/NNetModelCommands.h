// NNetModelCommands.h
//
// NNetModel

#pragma once

#include <string>
#include "BoolOp.h"
#include "Uniform2D.h"
#include "ParameterType.h"
#include "MoreTypes.h"
#include "MonitorData.h"
#include "IoNeuron.h"
#include "NobType.h"
#include "NobId.h"

class Param;
class NNetCommand;
class Observable;
class NobIdList;
class CommandStack;
class AlignAnimation;
class NNetModelImporter;
class NNetModelReaderInterface;
class NNetModelWriterInterface;
class MicroMeterPntVector;
class SignalGenerator;

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

    void ResetModel();
    void PushCommand(unique_ptr<NNetCommand>);
    void UndoCommand();
    void RedoCommand();

    void AddIncoming2Knot   (NobId const, MicroMeterPnt const &);
    void AddIncoming2Pipe   (NobId const, MicroMeterPnt const &);
    void AddModel           ();
    void AddOutgoing2Knot   (NobId const, MicroMeterPnt const &);
    void AddOutgoing2Pipe   (NobId const, MicroMeterPnt const &);
    void AddSignal          (MicroMeterCircle const &, TrackNr const);
    void AnalyzeAnomalies   ();
    void AnalyzeLoops       ();
    void AnimateArrows      (MicroMeter &, MicroMeter const);
    void AnimateCoord       (Uniform2D<MicroMeter> &, Uniform2D<MicroMeter> const &);
    void AppendInputNeuron  (NobId const);
    void AppendOutputNeuron (NobId const);
    void ClearBeepers       ();
    void Connect            (NobId const, NobId const);
    void CopySelection      ();
    void CreateInitialNobs  ();
    void DeleteSelection    ();
    void DeleteNob          (NobId const);
    void DeleteSignal       (SignalId const &);
    void DeleteTrack        (TrackNr const);
    void DiscBaseKnot       (NobId const);
    void DiscIoConnector    (NobId const);
    void InsertKnot         (NobId const, MicroMeterPnt const &);
    void InsertNeuron       (NobId const, MicroMeterPnt const &);
    void InsertTrack        (TrackNr const);
    bool MakeIoConnector    ();
    void MoveSelection      (MicroMeterPnt const &);
    void MoveSensor         (SignalId const &, MicroMeterPnt const &);
    void MoveSignal         (SignalId const &, TrackNr const);
    void MoveNob            (NobId const, MicroMeterPnt const &);
    void NewInputNeuron     (MicroMeterPnt const &);
    void NewOutputNeuron    (MicroMeterPnt const &);
    void RestrictSelection  (NobType::Value const);
    void Rotate             (NobId const, MicroMeterPnt const &, MicroMeterPnt const &);
    void RotateModel        (MicroMeterPnt const &, MicroMeterPnt const &);
    void RotateSelection    (MicroMeterPnt const &, MicroMeterPnt const &);
    void SelectAll          (bool const);
    void SelectAllBeepers   ();
    void SelectNob          (NobId const, tBoolOp const);
    void SelectNobsInRect   (MicroMeterRect const &);
    void SelectSubtree      (NobId const, bool const);
    void SetParameter       (ParamType::Value const, float const);
    void SetStimulusParams  (SignalGenerator &, fMicroSecs const, fHertz const);
    void SetNob             (NobId const, MicroMeterPosDir const);
    void SetTriggerSound    (NobId const, SoundDescr const &);
    void SizeSelection      (float const);
    void SizeSensor         (SignalId const &, float const);
    void SplitNeuron        (NobId const);
    void ToggleEmphMode     (NobId const);
    void ToggleStopOnTrigger(NobId const);
    void TriggerStimulus    (NobId const);

    SignalId SetHighlightedSignal(MicroMeterPnt const &);

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