// NNetModelCommands.ixx
//
// Commands

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
import Commands;
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
        Sound        * const,
        CommandStack * const
    );
    void SetModelInterface(NNetModelWriterInterface* const);

    void ResetModel();
    void PushCommand(unique_ptr<NNetCommand>);
    void UndoCommand();
    void RedoCommand();

    void ExtendOutputLine  (NobId const, MicroMeterPnt const&);
    void AddIncoming2Neuron(NobId const, MicroMeterPnt const&);
    void CreateSynapse     (NobId const, MicroMeterPnt const&);
    void AddModel();
    void AddSensor(MicroMeterCircle const&, TrackNr const);
    void AddSigGen2Monitor(TrackNr const);
    void AnalyzeLoops();
    void AnimateArrows(MicroMeter&, MicroMeter const);
    void AnimateCoord (Uniform2D<MicroMeter>&, Uniform2D<MicroMeter> const&);
    void AttachSigGen2Conn(NobId const);
    void AttachSigGen2Line(NobId const);
    void AttachSigGen2Sel();
    void Connect(NobId const, NobId const);
    void CopySelection();
    void CreateInitialNobs();
    void DeleteNob(NobId const);
    void DeleteSelection();
    void DeleteSensor(SensorId const&);
    void DeleteSignal(SignalId const&);
    void DeleteSigGen();
    void DeleteTrack(TrackNr const);
    void DiscIoConnector(NobId const);
    void InsertKnot  (NobId const, MicroMeterPnt const&);
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

    wostream& TraceStream() { return wcout; }

    bool                       m_bTrace                  { true };
    CommandStack             * m_pCmdStack               { nullptr };
    NNetModelWriterInterface * m_pNMWI                   { nullptr };
    NNetModelIO              * m_pModelIO                { nullptr };
    Observable               * m_pDynamicModelObservable { nullptr };
    Sound                    * m_pSound                  { nullptr };
};