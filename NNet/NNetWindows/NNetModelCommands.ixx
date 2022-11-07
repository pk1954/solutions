// NNetModelCommands.ixx
//
// NNetWindows

module;

#include <vector>
#include <string>
#include <iostream>

export module NNetModelCommands;

import Observable;
import Types;
import BoolOp;
import Uniform2D;
import SoundInterface;
import CommandStack;
import NNetModelIO;
import NNetCommand;
import NNetModel;

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

    void ExtendInputLine   (MicroMeterPnt const&);
    void ExtendOutputLine  (MicroMeterPnt const&);
    void AddIncoming2Neuron(MicroMeterPnt const&);
    void AddIncoming2Pipe  (MicroMeterPnt const&);
    void AddOutgoing2Pipe  (MicroMeterPnt const&);
    void AddModel();
    void AddSensor(MicroMeterCircle const&, TrackNr const);
    void AddSigGen2Monitor(TrackNr const);
    void AnalyzeAnomalies();
    void AnalyzeLoops();
    void AnimateArrows(MicroMeter&, MicroMeter const);
    void AnimateCoord (Uniform2D<MicroMeter>&, Uniform2D<MicroMeter> const&);
    void AttachSigGen2Conn();
    void AttachSigGen2Line();
    void AttachSigGen2Sel();
    void Connect();
    void CopySelection();
    void CreateInitialNobs();
    void DeleteBaseKnot();
    void DeleteNob();
    void DeleteSelection();
    void DeleteSignal(SignalId const&);
    void DeleteSigGen();
    void DeleteTrack(TrackNr const);
    void DiscIoConnector();
    void InsertKnot  (MicroMeterPnt const&);
    void InsertNeuron(MicroMeterPnt const&);
    void InsertTrack(TrackNr const);
    bool MakeIoConnector();
    void MoveSelection(MicroMeterPnt const&);
    void MoveSensor(SensorId const, MicroMeterPnt const&);
    void MoveSignal(SignalId const&, TrackNr const);
    void MoveNob(MicroMeterPnt const&);
    void NewIoLinePair(MicroMeterPnt const&);
    void NewSignalGenerator();
    void RenameSigGen(SigGenId const, wstring const&);
    void RestrictSelection(NobType::Value const);
    void Rotate(MicroMeterPnt const&, MicroMeterPnt const&);
    void RotateModel(MicroMeterPnt const&, MicroMeterPnt const&);
    void RotateSelection(MicroMeterPnt const&, MicroMeterPnt const&);
    void SelectAll(bool const);
    void SelectNob(tBoolOp const);
    void SelectNobsInRect(MicroMeterRect const&);
    void SelectSigGenClients();
    void SelectSubtree(bool const);
    void SetActiveSignalGenerator(SigGenId const);
    void SetParameter(ParamType::Value const, float const);
    void SetSigGenStaticData(SignalGenerator&, SigGenStaticData const&);
    void SetNob(MicroMeterPosDir const);
    void SizeSelection(float const);
    void SizeSensor(SensorId const, float const);
    void SplitNeuron();
    void ToggleEmphMode();
    void ToggleStopOnTrigger();
    void StartStimulus();

    SensorId SetHighlightedSensor(MicroMeterPnt const&);

    void SetHighlightedNob(NobId const id) { m_nobHighlighted = id; }
    void SetTargetNob     (NobId const id) { m_nobTarget = id; }

    NobId GetHighlightedNob() const { return m_nobHighlighted; }
    NobId GetTargetNob()      const { return m_nobTarget; }

private:

    bool      IsTraceOn() const { return m_bTrace; }
    wostream& TraceStream()     { return wcout; }

    Nob * nobHighPtr() { return m_pNMWI->GetNob(m_nobHighlighted); }
    Nob & nobHigh()    { return * nobHighPtr(); }

    NobId                      m_nobHighlighted         { NO_NOB };
    NobId                      m_nobTarget              { NO_NOB };
    bool                       m_bTrace                 { true };
    CommandStack             * m_pCmdStack              { nullptr };
    NNetModelWriterInterface * m_pNMWI                  { nullptr };
    NNetModelIO              * m_pModelIO               { nullptr };
    Observable               * m_pDynamicModelObservable{ nullptr };
};