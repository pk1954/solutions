// NNetModelCommands.cpp
//
// NNetWindows

module;

#include <cassert>
#include <iostream>
#include <memory>
#include <source_location>
#include <Windows.h>

module NNetModelCommands;

import Uniform2D;
import AddNobsCommand;
import DeleteTrackCommand;
import MoveSensorCmd;
import ArrowAnimation;
import DeleteBaseKnotCmd;
import DeleteSigGenCmd;
import SelectNobsInRectCommand;
import CoordAnimation;
import NewIoLinePairCmd;
import SizeSensorCmd;
import NewSigGenCmd;
import ToggleStopOnTriggerCmd;
import SelectAllCommand;
import MoveNobCommand;
import ToggleEmphModeCmd;
import MoveSignalCmd;
import DeleteSelectionCommand;
import ExtendInputLineCmd;
import ExtendOutputLineCmd;
import AddPipe2NeuronCmd;
import SelectionCommand;
import InsertTrackCommand;
import SplitNeuronCmd;
import SizeSelectionCmd;
import SetNobCommand;
import SetSigGenStaticDataCmd;
import AnalyzeCommand;
import MoveSelectionCommand;
import InsertBaseKnotCommand;
import DeleteSignalCommand;
import RotateNobCommand;
import SelectNobCommand;
import RotateModelCommand;
import RotateSelectionCommand;
import AttachSigGen2SelCmd;
import RenameSigGenCmd;
import CreateSynapseCommand;
import CreateForkCommand;
import AddSensorSignalCmd;
import AddSigGen2MonitorCmd;
import RestrictSelectionCommand;
import SetParameterCommand;
import DiscIoConnectorCmd;
import SetTargetNobCmd;
import NNetModelCommands;
import SetHighlightedNobCmd;
import AttachSigGen2ConCmd;
import Connect2NeuronCommand;
import ConnectCreateSynapseCommand;
import ConnectCreateForkCommand;
import AttachSigGen2LineCmd;
import SelectSubtreeCommand;
import SelSigGenClientsCmd;
import PlugIoConnectorAnimation;
import PlugIoLineAnimation;
import ConnAnimationCommand;
import CommandFunctions;
import CommandStack;
import SetActiveSigGenCmd;
import NNetModelIO;
import NNetModel;

using std::wstring;
using std::wcout;
using std::endl;
using std::move;
using std::make_unique;
using std::unique_ptr;
using std::source_location;

void NNetModelCommands::Initialize
(
	NNetModelIO  * const pModelIO,
	Observable   * const pDynamicModelObservable,
	CommandStack * const pCmdStack
) 
{ 
	m_pModelIO                = pModelIO;
	m_pDynamicModelObservable = pDynamicModelObservable;
	m_pCmdStack               = pCmdStack;
}

void NNetModelCommands::SetModelInterface
(
	NNetModelWriterInterface * const pNMWI
) 
{ 
	m_pNMWI = pNMWI;
}

void NNetModelCommands::UndoCommand()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	if (! m_pCmdStack->UndoCommand())
		MessageBeep(MB_ICONWARNING);
}

void NNetModelCommands::RedoCommand()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	if (! m_pCmdStack->RedoCommand())
		MessageBeep(MB_ICONWARNING);
}

void NNetModelCommands::PushCommand(unique_ptr<NNetCommand> upCmd)
{
	m_pCmdStack->PushCommand(move(upCmd));
}

void NNetModelCommands::ResetModel()
{ 
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pNMWI->ResetModel();
	m_pCmdStack->Clear();
	m_pDynamicModelObservable->NotifyAll(false);
}

void NNetModelCommands::ExtendInputLine(NobId const id, MicroMeterPnt const& pos) // case 10
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<ExtendInputLineCmd>(id, pos - STD_OFFSET));
}

void NNetModelCommands::ExtendOutputLine(NobId const id, MicroMeterPnt const& pos) // case 11
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<ExtendOutputLineCmd>(id, pos + STD_OFFSET));
}

void NNetModelCommands::AddIncoming2Neuron(NobId const id, MicroMeterPnt const& pos) // case 9
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<AddPipe2NeuronCmd>(id, pos - STD_OFFSET));
}

void NNetModelCommands::AddIncoming2Pipe(NobId const id, MicroMeterPnt const & pos)  // case 8
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<CreateSynapseCommand>(id, pos));
}

void NNetModelCommands::AddModel()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() 
		              << L" \"" << m_pModelIO->GetModelFileName() << L"\" " << endl;
	unique_ptr<Model> upImportedModel { m_pModelIO->GetImportedModel() };
	m_pCmdStack->PushCommand(make_unique<AddNobsCommand>(upImportedModel->GetUPNobs()));
}

void NNetModelCommands::AddOutgoing2Pipe(NobId const id, MicroMeterPnt const & pos)  // case 7
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<CreateForkCommand>(pos, id));
}

void NNetModelCommands::AddSensor
(
	MicroMeterCircle const & umCircle,
	TrackNr          const   trackNr
)
{ 
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << umCircle << L" " << trackNr << endl;
	m_pCmdStack->PushCommand(make_unique<AddSensorSignalCmd>(umCircle, trackNr));
}

void NNetModelCommands::AnalyzeAnomalies()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AnalyzeCommand>(ModelAnalyzer::FindAnomaly(*m_pNMWI)));
}

void NNetModelCommands::AnalyzeLoops()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AnalyzeCommand>(ModelAnalyzer::FindLoop(*m_pNMWI)));
}

void NNetModelCommands::AnimateCoord
(
	Uniform2D<MicroMeter>       & actual, 
	Uniform2D<MicroMeter> const & target
)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<CoordAnimation>(actual, target));
}

void NNetModelCommands::AnimateArrows
(
	MicroMeter     & umActual, 
	MicroMeter const umTarget
)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<ArrowAnimation>(umActual, umTarget));
}

void NNetModelCommands::AttachSigGen2Line(NobId const idInputLine)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << idInputLine << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2LineCmd>(idInputLine));
}

void NNetModelCommands::AttachSigGen2Sel()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2SelCmd>());
}

void NNetModelCommands::AttachSigGen2Conn(NobId const idInputLine)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << idInputLine << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2ConCmd>(idInputLine));
}

void NNetModelCommands::Connect(NobId const idSrc, NobId const idDst)
{ 
	using enum ConnectionType;

	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << idSrc << L" " << idDst << endl;

	unique_ptr<Command> upCmd;
	switch (m_pNMWI->ConnectionResult(idSrc, idDst))
	{
		case ct_fork:        upCmd = make_unique<ConnectCreateForkCommand>   (idSrc, idDst); break;  // case 1                                                           // case 1
		case ct_synapse:	 upCmd = make_unique<ConnectCreateSynapseCommand>(idSrc, idDst); break;  // case 2
		case ct_neuron:      upCmd = make_unique<Connect2NeuronCommand>      (idSrc, idDst); break;  // case 3
		case ct_knot:		 upCmd = make_unique<PlugIoLineAnimation>        (idSrc, idDst); break;  // case 4/5
		case ct_ioConnector: upCmd = make_unique<PlugIoConnectorAnimation>   (idSrc, idDst); break;  // case 6
		default: assert(false);
	}
	m_pCmdStack->PushCommand(move(upCmd));
}

void NNetModelCommands::CopySelection()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	UPNobList list { CopySelectedNobs::Do(*m_pNMWI) };
	m_pCmdStack->PushCommand(make_unique<AddNobsCommand>(list));
}

void NNetModelCommands::CreateInitialNobs()
{ 
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pNMWI->CreateInitialNobs();
	m_pDynamicModelObservable->NotifyAll(false);
}

void NNetModelCommands::DeleteSignal(SignalId const & id)
{ 
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteSignalCommand>(id));
}

void NNetModelCommands::DeleteSelection()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteSelectionCommand>());
}

void NNetModelCommands::DeleteNob(NobId const id)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << endl;
	m_pCmdStack->PushCommand(MakeDeleteCommand(*m_pNMWI, *m_pNMWI->GetNob(id)));
}

void NNetModelCommands::DeleteTrack(TrackNr const nr)
{ 
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << nr << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteTrackCommand>(nr));
}

void NNetModelCommands::DeleteBaseKnot(NobId const id)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << endl;
	Nob * pNob { m_pNMWI->GetNob(id) };
	if (! pNob)
		return;
	m_pCmdStack->PushCommand(make_unique<DeleteBaseKnotCmd>(*pNob));
}

void NNetModelCommands::DiscIoConnector(NobId const id)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << endl;
	Nob * pNob { m_pNMWI->GetNob(id) };
	if (! pNob)
		return;
	m_pCmdStack->PushCommand(make_unique<DiscIoConnectorCmd>(*pNob));
}

void NNetModelCommands::InsertTrack(TrackNr const nr)
{ 
	if (IsTraceOn())
	TraceStream() << source_location::current().function_name() << nr << endl;
	m_pCmdStack->PushCommand(make_unique<InsertTrackCommand>(nr));
}

bool NNetModelCommands::MakeIoConnector()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	unique_ptr<ConnAnimationCommand> upCmd = { make_unique<ConnAnimationCommand>() };
	bool bCmdOk = upCmd->IsCmdOk();
	m_pCmdStack->PushCommand(move(upCmd));
	return bCmdOk;
}

void NNetModelCommands::SplitNeuron(NobId const id)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << endl;
	m_pCmdStack->PushCommand(make_unique<SplitNeuronCmd>(id));
}

void NNetModelCommands::ToggleStopOnTrigger(NobId const id)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << endl;
	m_pCmdStack->PushCommand(make_unique<ToggleStopOnTriggerCmd>(id));
}

SensorId NNetModelCommands::SetHighlightedSensor(MicroMeterPnt const & umPos)
{
	UPSensorList & list { m_pNMWI->GetSensorList() };
	SensorId const id   { list.FindSensor(umPos) };
	list.SetActive(id);
	return id;
}

void NNetModelCommands::SetParameter(ParamType::Value const param, float const fNewValue)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << ParamType::GetName(param) << L" " << fNewValue << endl;
	m_pCmdStack->PushCommand(make_unique<SetParameterCommand>(m_pNMWI->GetParams(), param, fNewValue));
}

void NNetModelCommands::SetSigGenStaticData(SignalGenerator & dst, SigGenStaticData const &data)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<SetSigGenStaticDataCmd>(dst, data));
}

void NNetModelCommands::MoveNob(NobId const id, MicroMeterPnt const & delta)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << delta << endl;
	m_pCmdStack->PushCommand(make_unique<MoveNobCommand>(*m_pNMWI->GetNob(id), delta));
}

void NNetModelCommands::MoveSensor(SensorId const id, MicroMeterPnt const & delta)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << delta << endl;
	m_pCmdStack->PushCommand(make_unique<MoveSensorCmd>(id, delta));
}

void NNetModelCommands::MoveSignal(SignalId const & id, TrackNr const trackNr)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << trackNr << endl;
	m_pCmdStack->PushCommand(make_unique<MoveSignalCmd>(id, trackNr));
}

void NNetModelCommands::NewSignalGenerator()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<NewSigGenCmd>());
}

void NNetModelCommands::SetActiveSignalGenerator(SigGenId const id)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pCmdStack->PushCommand(make_unique<SetActiveSigGenCmd>(id));
}

void NNetModelCommands::SetHighlightedNob(NobId const id)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pCmdStack->PushCommand(make_unique<SetHighlightedNobCmd>(id));
}

void NNetModelCommands::SetTargetNob(NobId const id)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pCmdStack->PushCommand(make_unique<SetTargetNobCmd>(id));
}

void NNetModelCommands::DeleteSigGen()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteSigGenCmd>());
}

void NNetModelCommands::RenameSigGen(SigGenId const id, wstring const & name)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << name << endl;
	m_pCmdStack->PushCommand(make_unique<RenameSigGenCmd>(id, name));
}

void NNetModelCommands::SelectSigGenClients()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<SelSigGenClientsCmd>());
}

void NNetModelCommands::AddSigGen2Monitor(TrackNr const trackNr)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AddSigGen2MonitorCmd>(trackNr));
}

void NNetModelCommands::SizeSelection(float const fFactor)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << fFactor << endl;
	m_pCmdStack->PushCommand(make_unique<SizeSelectionCmd>(fFactor));
}

void NNetModelCommands::SizeSensor(SensorId const id, float const fFactor)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << fFactor << endl;
	m_pCmdStack->PushCommand(make_unique<SizeSensorCmd>(id, fFactor));
}

void NNetModelCommands::Rotate
(
	NobId         const   id, 
	MicroMeterPnt const & umPntOld, 
	MicroMeterPnt const & umPntNew 
)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << umPntOld << L" " << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateNobCommand>(*m_pNMWI->GetNob(id), umPntOld, umPntNew));
}

void NNetModelCommands::SetNob(NobId const id, MicroMeterPosDir const posDir)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << posDir << endl;
	m_pCmdStack->PushCommand(make_unique<SetNobCommand>(*m_pNMWI->GetNob(id), posDir));
}

void NNetModelCommands::MoveSelection(MicroMeterPnt const & delta)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << delta << endl;
	m_pCmdStack->PushCommand(make_unique<MoveSelectionCommand>(delta));
}

void NNetModelCommands::RotateSelection(MicroMeterPnt const & umPntOld, MicroMeterPnt const & umPntNew)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << umPntOld << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateSelectionCommand>(umPntOld, umPntNew));
}

void NNetModelCommands::RotateModel(MicroMeterPnt const & umPntOld, MicroMeterPnt const & umPntNew)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << umPntOld << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateModelCommand>(umPntOld, umPntNew));
}

void NNetModelCommands::InsertKnot(NobId const id, MicroMeterPnt const & pos)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<InsertBaseKnotCommand<Knot>>(id, pos));
}

void NNetModelCommands::InsertNeuron(NobId const id, MicroMeterPnt const & pos)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<InsertBaseKnotCommand<Neuron>>(id, pos));
}

void NNetModelCommands::NewIoLinePair(MicroMeterPnt const & pos)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<NewIoLinePairCmd>(* m_pNMWI, pos));
}

void NNetModelCommands::RestrictSelection(NobType::Value const val)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << val << endl;
	m_pCmdStack->PushCommand(make_unique<RestrictSelectionCommand>(val));
}

void NNetModelCommands::SelectNob(NobId const id, tBoolOp const op)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << op << endl;
	if (IsDefined(id))
		m_pCmdStack->PushCommand(make_unique<SelectNobCommand>(id, op));
}

void NNetModelCommands::SelectAll(bool const bOn)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << bOn << endl;
	m_pCmdStack->PushCommand(make_unique<SelectAllCommand>(bOn));
}

void NNetModelCommands::SelectSubtree(NobId const id, bool  const bOn)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << bOn << endl;
	m_pCmdStack->PushCommand(make_unique<SelectSubtreeCommand>(id, bOn));
}

void NNetModelCommands::SelectNobsInRect(MicroMeterRect const & rect)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << rect << endl;
	m_pCmdStack->PushCommand(make_unique<SelectNobsInRectCommand>(rect));
}

void NNetModelCommands::ToggleEmphMode(NobId const id)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<ToggleEmphModeCmd>(id));
}

void NNetModelCommands::StartStimulus()
{  
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pNMWI->GetSigGenSelected()->StartStimulus();
}
