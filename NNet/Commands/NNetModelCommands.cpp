// NNetModelCommands.cpp
//
// Commands

module;

#include <cassert>
#include <iostream>
#include <memory>
#include <source_location>

module NNetModelCommands;

import AddNobsCommand;
import AddPipe2NeuronCmd;
import AddSensorSignalCmd;
import AddSigGen2MonitorCmd;
import ArrowAnimation;
import AttachSigGen2ConCmd;
import AttachSigGen2LineCmd;
import AttachSigGen2SelCmd;
import Commands;
import CommandFunctions;
import ConnAnimationCommand;
import CoordAnimation;
import ConnectCreateForkCmd;
import ConnectCreateSynapseCmd;
import Connect2NeuronCommand;
import CreateForkCommand;
import CreateSynapseCommand;
import DeleteSelectionCommand;
import DeleteSensorCommand;
import DeleteSigGenCmd;
import DeleteSignalCommand;
import DeleteTrackCommand;
import DiscIoConnectorCmd;
import InsertPosNobCommand;
import InsertTrackCommand;
import MoveSelectionCommand;
import MoveSensorCmd;
import MoveSignalCmd;
import NewIoLinePairCmd;
import NewSigGenCmd;
import NNetModel;
import NNetCommandStack;
import NNetModelCommands;
import NNetModelIO;
import PlugIoConnectors;
import PlugIoLines;
import RenameSigGenCmd;
import RestrictSelectionCommand;
import RotateModelCommand;
import RotateNobCommand;
import RotateSelectionCommand;
import SelectAllCommand;
import SelectionCommand;
import SelectAllConnectedCmd;
import SetActiveSigGenCmd;
import SelSigGenClientsCmd;
import SetNobCommand;
import SetParameterCommand;
import SetSigGenStaticDataCmd;
import SizeSelectionCmd;
import SizeSensorCmd;
import SoundInterface;
import SplitNeuronCmd;
import ToggleEmphModeCmd;
import ToggleStopOnTriggerCmd;
import Uniform2D;

using std::wstring;
using std::wcout;
using std::endl;
using std::move;
using std::make_unique;
using std::unique_ptr;
using std::source_location;

void NNetModelCommands::Initialize
(
	NNetModelIO      * const pModelIO,
	Observable       * const pDynamicModelObservable,
	Sound            * const pSound,
	NNetCommandStack * const pCmdStack
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

void NNetModelCommands::AddIncoming2Neuron(NobId const id, MicroMeterPnt const& pos) // case 9
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << pos << endl;
	m_pCmdStack->PushCommand(make_unique<AddPipe2NeuronCmd>(id, pos - STD_OFFSET));
}

void NNetModelCommands::AddSynapse(NobId const id, MicroMeterPnt const & pos)  // case 8
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << pos << endl;
	m_pCmdStack->PushCommand(make_unique<CreateSynapseCommand>(id, pos));
}

void NNetModelCommands::AddModel()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() 
		              << L" \"" << m_pModelIO->GetModelFileName() << L"\" " << endl;
	unique_ptr<Model> upImportedModel { m_pModelIO->GetImportedModel() };
	m_pCmdStack->PushCommand(make_unique<AddNobsCommand>(upImportedModel->MoveUPNobs()));
}

void NNetModelCommands::AddSensor
(
	MicroMeterCircle const & umCircle,
	TrackNr          const   trackNr
)
{ 
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << umCircle << trackNr << endl;
	m_pCmdStack->PushCommand(make_unique<AddSensorSignalCmd>(umCircle, trackNr));
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
		TraceStream() << source_location::current().function_name() << id << endl;
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
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pCmdStack->PushCommand(make_unique<AttachSigGen2ConCmd>(id));
}

void NNetModelCommands::Connect(NobId const idSrc, NobId const idDst)
{ 
	using enum ConnectionType;

	unique_ptr<Command> upCmd;
	ConnectionType connType { m_pNMWI->ConnectionResult(idSrc, idDst) };
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() 
		              << idSrc << idDst
		              << L" type " << static_cast<int>(connType) << endl;
	switch (connType)
	{
		case ct_fork: ConnectCreateForkCmd::Push(idSrc, idDst);          // case 1 
			return;
		case ct_synapse:	 
			upCmd = make_unique<ConnectCreateSynapseCmd>(idSrc, idDst);  // case 2
			break;  
		case ct_neuron:      
			upCmd = make_unique<Connect2NeuronCommand>(idSrc, idDst);    // case 3
			break;  
		case ct_knot:		 
			upCmd = make_unique<PlugIoLines>(idSrc, idDst);              // case 4/5
			break;
		case ct_connector:
			upCmd = make_unique<ConnAnimationCommand>(idSrc, idDst);     // case 12/13
			break;
		case ct_plugConnectors:
			upCmd = make_unique<PlugIoConnectors>(idSrc, idDst);         // case 6
			break;
		default: assert(false);
	}
	m_pCmdStack->PushCommand(move(upCmd));
}

void NNetModelCommands::CopySelection()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<AddNobsCommand>(move(CopySelectedNobs::Do(*m_pNMWI))));
}

void NNetModelCommands::CreateInitialNobs()
{ 
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pNMWI->CreateInitialNobs();
	m_pDynamicModelObservable->NotifyAll(false);
}

void NNetModelCommands::DeleteSensor(SensorId const& id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteSensorCommand>(id));
}

void NNetModelCommands::DeleteSignal(SignalId const& id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteSignalCommand>(id));
}

void NNetModelCommands::DeleteSelection()
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteSelectionCommand>());
}

void NNetModelCommands::DeleteNob(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pSound->Play(L"DISAPPEAR_SOUND");
	m_pCmdStack->PushCommand(MakeDeleteCommand(*m_pNMWI, *m_pNMWI->GetNob(id)));
}

void NNetModelCommands::DeleteTrack(TrackNr const nr)
{ 
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << nr << endl;
	m_pCmdStack->PushCommand(make_unique<DeleteTrackCommand>(nr));
}

void NNetModelCommands::DiscIoConnector(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << endl;
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

void NNetModelCommands::SplitNeuron(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pSound->Play(L"UNLOCK_SOUND");
	m_pCmdStack->PushCommand(make_unique<SplitNeuronCmd>(id));
}

void NNetModelCommands::ToggleStopOnTrigger(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << endl;
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

void NNetModelCommands::MoveSensor(SensorId const id, MicroMeterPnt const & delta)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << delta << endl;
	m_pCmdStack->PushCommand(make_unique<MoveSensorCmd>(id, delta));
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
		TraceStream() << source_location::current().function_name() << id << L" " << name << endl;
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
		TraceStream() << source_location::current().function_name() << id << L" " << fFactor << endl;
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
		TraceStream() << source_location::current().function_name() << umPntOld << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateNobCommand>(*m_pNMWI->GetNob(id), umPntOld, umPntNew));
}

void NNetModelCommands::SetNob
(
	NobId            const id,
	MicroMeterPosDir const posDir
)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << L" " << posDir << endl;
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
		TraceStream() << source_location::current().function_name() << umPntOld << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateSelectionCommand>(umPntOld, umPntNew));
}

void NNetModelCommands::RotateModel(MicroMeterPnt const & umPntOld, MicroMeterPnt const & umPntNew)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << umPntOld << umPntNew << endl;
	m_pCmdStack->PushCommand(make_unique<RotateModelCommand>(umPntOld, umPntNew));
}

void NNetModelCommands::InsertKnot(NobId const id, MicroMeterPnt const & pos)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << pos << endl;
	m_pCmdStack->PushCommand(make_unique<InsertPosNobCommand<Knot>>(id, pos));
}

void NNetModelCommands::InsertNeuron(NobId const id, MicroMeterPnt const & pos)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << pos << endl;
	m_pCmdStack->PushCommand(make_unique<InsertPosNobCommand<Neuron>>(id, pos));
}

void NNetModelCommands::RestrictSelection(NobType::Value const val)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << val << endl;
	m_pCmdStack->PushCommand(make_unique<RestrictSelectionCommand>(val));
}

void NNetModelCommands::SelectAll(bool const bOn)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << L" " << bOn << endl;
	m_pCmdStack->PushCommand(make_unique<SelectAllCommand>(bOn));
}

void NNetModelCommands::ToggleEmphMode(NobId const id)
{
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << id << endl;
	m_pCmdStack->PushCommand(make_unique<ToggleEmphModeCmd>(id));
}

void NNetModelCommands::StartStimulus()
{  
	if (m_bTrace)
		TraceStream() << source_location::current().function_name() << endl;
	m_pNMWI->GetSigGenSelected()->StartStimulus();
}
