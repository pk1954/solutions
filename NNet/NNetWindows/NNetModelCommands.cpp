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
import NNetModelCommands;
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

void NNetModelCommands::ExtendInputLine(MicroMeterPnt const& pos) // case 10
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<ExtendInputLineCmd>(m_nobHighlighted, pos - STD_OFFSET));
}

void NNetModelCommands::ExtendOutputLine(MicroMeterPnt const& pos) // case 11
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<ExtendOutputLineCmd>(m_nobHighlighted, pos + STD_OFFSET));
}

void NNetModelCommands::AddIncoming2Neuron(MicroMeterPnt const& pos) // case 9
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<AddPipe2NeuronCmd>(m_nobHighlighted, pos - STD_OFFSET));
}

void NNetModelCommands::AddIncoming2Pipe(MicroMeterPnt const & pos)  // case 8
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<CreateSynapseCommand>(m_nobHighlighted, pos));
}

void NNetModelCommands::AddModel()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() 
		              << L" \"" << m_pModelIO->GetModelFileName() << L"\" " << endl;
	unique_ptr<Model> upImportedModel { m_pModelIO->GetImportedModel() };
	m_pCmdStack->PushCommand(make_unique<AddNobsCommand>(upImportedModel->GetUPNobs()));
}

void NNetModelCommands::AddOutgoing2Pipe(MicroMeterPnt const & pos)  // case 7
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<CreateForkCommand>(pos, m_nobHighlighted));
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

void NNetModelCommands::AttachSigGen2Line()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2LineCmd>(m_nobHighlighted));
}

void NNetModelCommands::AttachSigGen2Sel()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2SelCmd>());
}

void NNetModelCommands::AttachSigGen2Conn()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2ConCmd>(m_nobHighlighted));
}

void NNetModelCommands::Connect()
{ 
	using enum ConnectionType;

	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;

	unique_ptr<Command> upCmd;
	switch (m_pNMWI->ConnectionResult(m_nobHighlighted, m_nobTarget))
	{
		case ct_fork:        upCmd = make_unique<ConnectCreateForkCommand>   (m_nobHighlighted, m_nobTarget); break;  // case 1                                                           // case 1
		case ct_synapse:	 upCmd = make_unique<ConnectCreateSynapseCommand>(m_nobHighlighted, m_nobTarget); break;  // case 2
		case ct_neuron:      upCmd = make_unique<Connect2NeuronCommand>      (m_nobHighlighted, m_nobTarget); break;  // case 3
		case ct_knot:		 upCmd = make_unique<PlugIoLineAnimation>        (m_nobHighlighted, m_nobTarget); break;  // case 4/5
		case ct_ioConnector: upCmd = make_unique<PlugIoConnectorAnimation>   (m_nobHighlighted, m_nobTarget); break;  // case 6
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

void NNetModelCommands::DeleteNob()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(MakeDeleteCommand(*m_pNMWI, nobHigh()));
}

void NNetModelCommands::DeleteTrack(TrackNr const nr)
{ 
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << nr << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteTrackCommand>(nr));
}

void NNetModelCommands::DeleteBaseKnot()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	if (nobHighPtr())
		m_pCmdStack->PushCommand(make_unique<DeleteBaseKnotCmd>(nobHigh()));
}

void NNetModelCommands::DiscIoConnector()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	if (nobHighPtr())
	    m_pCmdStack->PushCommand(make_unique<DiscIoConnectorCmd>(nobHigh()));
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

void NNetModelCommands::SplitNeuron()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<SplitNeuronCmd>(m_nobHighlighted));
}

void NNetModelCommands::ToggleStopOnTrigger()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<ToggleStopOnTriggerCmd>(m_nobHighlighted));
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

void NNetModelCommands::MoveNob(MicroMeterPnt const & delta)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << delta << endl;
	m_pCmdStack->PushCommand(make_unique<MoveNobCommand>(nobHigh(), delta));
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
	MicroMeterPnt const & umPntOld, 
	MicroMeterPnt const & umPntNew 
)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << umPntOld << L" " << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateNobCommand>(nobHigh(), umPntOld, umPntNew));
}

void NNetModelCommands::SetNob(MicroMeterPosDir const posDir)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << posDir << endl;
	m_pCmdStack->PushCommand(make_unique<SetNobCommand>(nobHigh(), posDir));
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

void NNetModelCommands::InsertKnot(MicroMeterPnt const & pos)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<InsertBaseKnotCommand<Knot>>(m_nobHighlighted, pos));
}

void NNetModelCommands::InsertNeuron(MicroMeterPnt const & pos)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<InsertBaseKnotCommand<Neuron>>(m_nobHighlighted, pos));
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

void NNetModelCommands::SelectNob(tBoolOp const op)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << op << endl;
	if (IsDefined(m_nobHighlighted))
		m_pCmdStack->PushCommand(make_unique<SelectNobCommand>(m_nobHighlighted, op));
}

void NNetModelCommands::SelectAll(bool const bOn)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << bOn << endl;
	m_pCmdStack->PushCommand(make_unique<SelectAllCommand>(bOn));
}

void NNetModelCommands::SelectSubtree(bool const bOn)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << bOn << endl;
	m_pCmdStack->PushCommand(make_unique<SelectSubtreeCommand>(m_nobHighlighted, bOn));
}

void NNetModelCommands::SelectNobsInRect(MicroMeterRect const & rect)
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << L" " << rect << endl;
	m_pCmdStack->PushCommand(make_unique<SelectNobsInRectCommand>(rect));
}

void NNetModelCommands::ToggleEmphMode()
{
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<ToggleEmphModeCmd>(m_nobHighlighted));
}

void NNetModelCommands::StartStimulus()
{  
	if (IsTraceOn())
		TraceStream() << source_location::current().function_name() << endl;
	m_pNMWI->GetSigGenSelected()->StartStimulus();
}
