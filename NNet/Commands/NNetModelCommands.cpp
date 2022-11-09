// NNetModelCommands.cpp
//
// Commands

module;

#include <cassert>
#include <iostream>
#include <memory>
#include <source_location>

module NNetModelCommands;

import Uniform2D;
import SoundInterface;
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
import InsertPosNobCommand;
import DeleteSignalCommand;
import RotateNobCommand;
import SelectNobCommand;
import RotateModelCommand;
import RotateSelectionCommand;
import AttachSigGen2SelCmd;
import RenameSigGenCmd;
import ConnectCreateSynapseCmd;
import ConnectCreateForkCmd;
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
import AttachSigGen2LineCmd;
import SelectSubtreeCommand;
import SelSigGenClientsCmd;
import PlugIoConnectorAnimation;
import PlugIoLineAnimation;
import ConnAnimationCommand;
import CommandFunctions;
import Commands;
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
	Sound        * const pSound,
	CommandStack * const pCmdStack
) 
{ 
	m_pModelIO                = pModelIO;
	m_pDynamicModelObservable = pDynamicModelObservable;
	m_pSound                  = pSound;
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
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	if (! m_pCmdStack->UndoCommand())
		m_pSound->Warning();
}

void NNetModelCommands::RedoCommand()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	if (! m_pCmdStack->RedoCommand())
		m_pSound->Warning();
}

void NNetModelCommands::PushCommand(unique_ptr<NNetCommand> upCmd)
{
	m_pCmdStack->PushCommand(move(upCmd));
}

void NNetModelCommands::ResetModel()
{ 
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pNMWI->ResetModel();
	m_pCmdStack->Clear();
	m_pDynamicModelObservable->NotifyAll(false);
}

//void NNetModelCommands::ExtendInputLine(NobId const id, MicroMeterPnt const& pos) // case 10
//{
//	ExtendInputLineCmd::Push(*m_pCmdStack, id, pos);
//	//if (m_bTrace)
//	//	TraceStream() << source_location::current().function_name() << L" " << pos << endl;
//	//m_pCmdStack->PushCommand(make_unique<ExtendInputLineCmd>(id, pos - STD_OFFSET));
//}

void NNetModelCommands::ExtendOutputLine(NobId const id, MicroMeterPnt const& pos) // case 11
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<ExtendOutputLineCmd>(id, pos + STD_OFFSET));
}

void NNetModelCommands::AddIncoming2Neuron(NobId const id, MicroMeterPnt const& pos) // case 9
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<AddPipe2NeuronCmd>(id, pos - STD_OFFSET));
}

void NNetModelCommands::AddIncoming2Pipe(NobId const id, MicroMeterPnt const & pos)  // case 8
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<CreateSynapseCommand>(id, pos));
}

void NNetModelCommands::AddModel()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() 
		              << L" \"" << m_pModelIO->GetModelFileName() << L"\" " << endl;
	unique_ptr<Model> upImportedModel { m_pModelIO->GetImportedModel() };
	m_pCmdStack->PushCommand(make_unique<AddNobsCommand>(upImportedModel->GetUPNobs()));
}

void NNetModelCommands::AddOutgoing2Pipe(NobId const id, MicroMeterPnt const & pos)  // case 7
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<CreateForkCommand>(pos, id));
}

void NNetModelCommands::AddSensor
(
	MicroMeterCircle const & umCircle,
	TrackNr          const   trackNr
)
{ 
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << umCircle << L" " << trackNr << endl;
	m_pCmdStack->PushCommand(make_unique<AddSensorSignalCmd>(umCircle, trackNr));
}

void NNetModelCommands::AnalyzeAnomalies()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AnalyzeCommand>(ModelAnalyzer::FindAnomaly(*m_pNMWI)));
}

void NNetModelCommands::AnalyzeLoops()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AnalyzeCommand>(ModelAnalyzer::FindLoop(*m_pNMWI)));
}

void NNetModelCommands::AnimateCoord
(
	Uniform2D<MicroMeter>       & actual, 
	Uniform2D<MicroMeter> const & target
)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<CoordAnimation>(actual, target));
}

void NNetModelCommands::AnimateArrows
(
	MicroMeter     & umActual, 
	MicroMeter const umTarget
)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<ArrowAnimation>(umActual, umTarget));
}

void NNetModelCommands::AttachSigGen2Line(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2LineCmd>(id));
}

void NNetModelCommands::AttachSigGen2Sel()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2SelCmd>());
}

void NNetModelCommands::AttachSigGen2Conn(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2ConCmd>(id));
}

void NNetModelCommands::Connect(NobId const idSrc, NobId const idDst)
{ 
	using enum ConnectionType;

	unique_ptr<Command> upCmd;
	switch (m_pNMWI->ConnectionResult(idSrc, idDst))
	{
		case ct_fork:    ConnectCreateForkCmd::Push(idSrc, idDst); return; // case 1 
		case ct_synapse:	 
			if (m_bTrace)
				TraceStream() << source_location::current().function_name() << endl;
			upCmd = make_unique<ConnectCreateSynapseCmd>(idSrc, idDst); // case 2
			break;  
		case ct_neuron:      
			if (m_bTrace)
				TraceStream() << source_location::current().function_name() << endl;
			upCmd = make_unique<Connect2NeuronCommand>      (idSrc, idDst); // case 3
			break;  
		case ct_knot:		 
			if (m_bTrace)
				TraceStream() << source_location::current().function_name() << endl;
			upCmd = make_unique<PlugIoLineAnimation>        (idSrc, idDst); // case 4/5
			break;  
		case ct_ioConnector: 
			if (m_bTrace)
				TraceStream() << source_location::current().function_name() << endl;
			upCmd = make_unique<PlugIoConnectorAnimation>   (idSrc, idDst); // case 6
			break;  
		default: assert(false);
	}
	m_pCmdStack->PushCommand(move(upCmd));
}

void NNetModelCommands::CopySelection()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	UPNobList list { CopySelectedNobs::Do(*m_pNMWI) };
	m_pCmdStack->PushCommand(make_unique<AddNobsCommand>(list));
}

void NNetModelCommands::CreateInitialNobs()
{ 
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pNMWI->CreateInitialNobs();
	m_pDynamicModelObservable->NotifyAll(false);
}

void NNetModelCommands::DeleteSignal(SignalId const & id)
{ 
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteSignalCommand>(id));
}

void NNetModelCommands::DeleteSelection()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteSelectionCommand>());
}

void NNetModelCommands::DeleteNob(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pSound->Play(L"DISAPPEAR_SOUND");
	m_pCmdStack->PushCommand(MakeDeleteCommand(*m_pNMWI, *m_pNMWI->GetNob(id)));
}

void NNetModelCommands::DeleteTrack(TrackNr const nr)
{ 
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << nr << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteTrackCommand>(nr));
}

void NNetModelCommands::DeleteBaseKnot(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	if (m_pNMWI->GetNob(id))
		m_pCmdStack->PushCommand(make_unique<DeleteBaseKnotCmd>(*m_pNMWI->GetNob(id)));
}

void NNetModelCommands::DiscIoConnector(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	if (m_pNMWI->GetNob(id))
	{
		m_pSound->Play(L"UNLOCK_SOUND");
		m_pCmdStack->PushCommand(make_unique<DiscIoConnectorCmd>(*m_pNMWI->GetNob(id)));
	}
}

void NNetModelCommands::InsertTrack(TrackNr const nr)
{ 
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << nr << endl;
	m_pCmdStack->PushCommand(make_unique<InsertTrackCommand>(nr));
}

bool NNetModelCommands::MakeIoConnector()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	unique_ptr<ConnAnimationCommand> upCmd = { make_unique<ConnAnimationCommand>() };
	bool bCmdOk = upCmd->IsCmdOk();
	m_pCmdStack->PushCommand(move(upCmd));
	return bCmdOk;
}

void NNetModelCommands::SplitNeuron(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pSound->Play(L"UNLOCK_SOUND");
	m_pCmdStack->PushCommand(make_unique<SplitNeuronCmd>(id));
}

void NNetModelCommands::ToggleStopOnTrigger(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pSound->Play(L"SNAP_IN_SOUND");
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
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << ParamType::GetName(param) << L" " << fNewValue << endl;
	m_pCmdStack->PushCommand(make_unique<SetParameterCommand>(m_pNMWI->GetParams(), param, fNewValue));
}

void NNetModelCommands::SetSigGenStaticData(SignalGenerator & dst, SigGenStaticData const &data)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<SetSigGenStaticDataCmd>(dst, data));
}

void NNetModelCommands::MoveNob(NobId const id, MicroMeterPnt const & delta)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << delta << endl;
	m_pCmdStack->PushCommand(make_unique<MoveNobCommand>(*m_pNMWI->GetNob(id), delta));
}

void NNetModelCommands::MoveSensor(SensorId const id, MicroMeterPnt const & delta)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << delta << endl;
	m_pCmdStack->PushCommand(make_unique<MoveSensorCmd>(id, delta));
}

void NNetModelCommands::MoveSignal(SignalId const & id, TrackNr const trackNr)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << trackNr << endl;
	m_pCmdStack->PushCommand(make_unique<MoveSignalCmd>(id, trackNr));
}

void NNetModelCommands::NewSignalGenerator()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<NewSigGenCmd>());
}

void NNetModelCommands::SetActiveSignalGenerator(SigGenId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pCmdStack->PushCommand(make_unique<SetActiveSigGenCmd>(id));
}

void NNetModelCommands::DeleteSigGen()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteSigGenCmd>());
}

void NNetModelCommands::RenameSigGen(SigGenId const id, wstring const & name)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << id << L" " << name << endl;
	m_pCmdStack->PushCommand(make_unique<RenameSigGenCmd>(id, name));
}

void NNetModelCommands::SelectSigGenClients()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<SelSigGenClientsCmd>());
}

void NNetModelCommands::AddSigGen2Monitor(TrackNr const trackNr)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AddSigGen2MonitorCmd>(trackNr));
}

void NNetModelCommands::SizeSelection(float const fFactor)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << fFactor << endl;
	m_pCmdStack->PushCommand(make_unique<SizeSelectionCmd>(fFactor));
}

void NNetModelCommands::SizeSensor(SensorId const id, float const fFactor)
{
	if (m_bTrace)
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
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << umPntOld << L" " << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateNobCommand>(*m_pNMWI->GetNob(id), umPntOld, umPntNew));
}

void NNetModelCommands::SetNob
(
	NobId            const id,
	MicroMeterPosDir const posDir
)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << posDir << endl;
	m_pCmdStack->PushCommand(make_unique<SetNobCommand>(*m_pNMWI->GetNob(id), posDir));
}

void NNetModelCommands::MoveSelection(MicroMeterPnt const & delta)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << delta << endl;
	m_pCmdStack->PushCommand(make_unique<MoveSelectionCommand>(delta));
}

void NNetModelCommands::RotateSelection(MicroMeterPnt const & umPntOld, MicroMeterPnt const & umPntNew)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << umPntOld << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateSelectionCommand>(umPntOld, umPntNew));
}

void NNetModelCommands::RotateModel(MicroMeterPnt const & umPntOld, MicroMeterPnt const & umPntNew)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << umPntOld << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateModelCommand>(umPntOld, umPntNew));
}

void NNetModelCommands::InsertKnot(NobId const id, MicroMeterPnt const & pos)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<InsertPosNobCommand<Knot>>(id, pos));
}

void NNetModelCommands::InsertNeuron(NobId const id, MicroMeterPnt const & pos)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<InsertPosNobCommand<Neuron>>(id, pos));
}

void NNetModelCommands::NewIoLinePair(MicroMeterPnt const & pos)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << pos << endl;
	m_pCmdStack->PushCommand(make_unique<NewIoLinePairCmd>(* m_pNMWI, pos));
}

void NNetModelCommands::RestrictSelection(NobType::Value const val)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << val << endl;
	m_pCmdStack->PushCommand(make_unique<RestrictSelectionCommand>(val));
}

void NNetModelCommands::SelectNob(NobId const id, tBoolOp const op)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << op << endl;
	if (IsDefined(id))
		m_pCmdStack->PushCommand(make_unique<SelectNobCommand>(id, op));
}

void NNetModelCommands::SelectAll(bool const bOn)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << bOn << endl;
	m_pCmdStack->PushCommand(make_unique<SelectAllCommand>(bOn));
}

void NNetModelCommands::SelectSubtree(NobId const id, bool const bOn)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << bOn << endl;
	m_pCmdStack->PushCommand(make_unique<SelectSubtreeCommand>(id, bOn));
}

void NNetModelCommands::SelectNobsInRect(MicroMeterRect const & rect)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << rect << endl;
	m_pCmdStack->PushCommand(make_unique<SelectNobsInRectCommand>(rect));
}

void NNetModelCommands::ToggleEmphMode(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<ToggleEmphModeCmd>(id));
}

void NNetModelCommands::StartStimulus()
{  
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pNMWI->GetSigGenSelected()->StartStimulus();
}
