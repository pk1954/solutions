// NNetModelCommands.ixx
//
// NNetWindows

module;

#include <string>
#include "UPSensorList.h"
#include "MonitorData.h"
#include "IoLine.h"
#include "NobType.h"
#include "NobId.h"
#include "SignalId.h"
#include "NNetParameters.h"
#include "NNetCommand.h"
#include "SigGenStaticData.h"
#include "NNetModelIO.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "SignalGenerator.h"
#include "SigGenId.h"

export module NNetModelCommands;

import Observable;
import MoreTypes;
import BoolOp;
import Uniform2D;
import SoundInterface;
import CommandStack;
import NobIdList;
import ParamType;
import TrackNr;

using std::wcout;
using std::wstring;
using std::wostream;
using std::vector;
using std::unique_ptr;

export class NNetModelCommands
{
public:
    void Initialize
    (
        NNetModelIO  * const,
        Observable   * const,
        CommandStack * const
    );
    void SetModelInterface(NNetModelWriterInterface* const);

    void ResetModel();
    void PushCommand(unique_ptr<NNetCommand>);
    void UndoCommand();
    void RedoCommand();

    void AddIncoming2BaseKnot(NobId const, MicroMeterPnt const&);
    void AddIncoming2Pipe(NobId const, MicroMeterPnt const&);
    void AddModel();
    void AddOutgoing2BaseKnot(NobId const, MicroMeterPnt const&);
    void AddOutgoing2Pipe(NobId const, MicroMeterPnt const&);
    void AddSensor(MicroMeterCircle const&, TrackNr const);
    void AddSigGen2Monitor(TrackNr const);
    void AnalyzeAnomalies();
    void AnalyzeLoops();
    void AnimateArrows(MicroMeter&, MicroMeter const);
    void AnimateCoord(Uniform2D<MicroMeter>&, Uniform2D<MicroMeter> const&);
    void AttachSigGen2Conn(NobId const);
    void AttachSigGen2Line(NobId const);
    void AttachSigGen2Sel();
    void Connect(NobId const, NobId const, ConnectionType const);
    void CopySelection();
    void CreateInitialNobs();
    void DeleteBaseKnot(NobId const);
    void DeleteNob(NobId const);
    void DeleteSelection();
    void DeleteSignal(SignalId const&);
    void DeleteSigGen();
    void DeleteTrack(TrackNr const);
    void DiscIoConnector(NobId const);
    void InsertKnot(NobId const, MicroMeterPnt const&);
    void InsertNeuron(NobId const, MicroMeterPnt const&);
    void InsertTrack(TrackNr const);
    bool MakeIoConnector();
    void MoveSelection(MicroMeterPnt const&);
    void MoveSensor(SensorId const, MicroMeterPnt const&);
    void MoveSignal(SignalId const&, TrackNr const);
    void MoveNob(NobId const, MicroMeterPnt const&);
    void NewIoLinePair(MicroMeterPnt const&);
    void NewSignalGenerator();
    void RenameSigGen(SigGenId const, wstring const&);
    void RestrictSelection(NobType::Value const);
    void Rotate(NobId const, MicroMeterPnt const&, MicroMeterPnt const&);
    void RotateModel(MicroMeterPnt const&, MicroMeterPnt const&);
    void RotateSelection(MicroMeterPnt const&, MicroMeterPnt const&);
    void SelectAll(bool const);
    void SelectNob(NobId const, tBoolOp const);
    void SelectNobsInRect(MicroMeterRect const&);
    void SelectSigGenClients();
    void SelectSubtree(NobId const, bool const);
    void SetActiveSignalGenerator(SigGenId const);
    void SetHighlightedNob(NobId const);
    void SetTargetNob(NobId const);
    void SetParameter(ParamType::Value const, float const);
    void SetSigGenStaticData(SignalGenerator&, SigGenStaticData const&);
    void SetNob(NobId const, MicroMeterPosDir const);
    void SizeSelection(float const);
    void SizeSensor(SensorId const, float const);
    void SplitNeuron(NobId const);
    void ToggleEmphMode(NobId const);
    void ToggleStopOnTrigger(NobId const);
    void StartStimulus();

    SensorId SetHighlightedSensor(MicroMeterPnt const&);

private:

    bool       IsTraceOn() const { return m_bTrace; }
    wostream& TraceStream() { return wcout; }

    bool                       m_bTrace{ true };
    CommandStack* m_pCmdStack{ nullptr };
    NNetModelWriterInterface* m_pNMWI{ nullptr };
    NNetModelIO* m_pModelIO{ nullptr };
    Observable* m_pDynamicModelObservable{ nullptr };
};