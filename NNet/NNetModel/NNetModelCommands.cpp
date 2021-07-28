// NNetModelCommands.cpp
//
// NNetModel

#include "stdafx.h"
#include "AddModelCommand.h"
#include "AddIncoming2KnotCommand.h"
#include "AddIncoming2PipeCommand.h"
#include "AddOutgoing2KnotCommand.h"
#include "AddOutgoing2PipeCommand.h"
#include "AddSignalCommand.h"
#include "AnalyzeCommand.h"
#include "Analyzer.h"
#include "AppendNeuronCommand.h"
#include "ClearBeepersCommand.h"
#include "CommandStack.h"
#include "CommandFunctions.h"
#include "CopySelectionCommand.h"
#include "DeleteSelectionCommand.h"
#include "DeleteSignalCommand.h"
#include "DeleteTrackCommand.h"
#include "DiscBaseKnotCmd.h"
#include "DiscClosedConnCmd.h"
#include "DiscIoConnectorCmd.h"
#include "InsertBaseKnotCommand.h"
#include "InsertTrackCommand.h"
#include "MoveNobCommand.h"
#include "MoveSelectionCommand.h"
#include "NewNeuronCommandT.h"
#include "NNetModelImporter.h"
#include "NNetModelStorage.h"
#include "RestrictSelectionCommand.h"
#include "RotateCommand.h"
#include "SelectAllBeepersCommand.h"
#include "SelectAllCommand.h"
#include "SelectionCommand.h"
#include "SelectNobCommand.h"
#include "SelectNobsInRectCommand.h"
#include "SelectSubtreeCommand.h"
#include "SetIoNeuronsCommand.h"
#include "SetParameterCommand.h"
#include "SetPulseRateCommand.h"
#include "SetNobCommand.h"
#include "SetTriggerSoundCommand.h"
#include "SplitClosedConnCmd.h"
#include "SplitNeuronCmd.h"
#include "ToggleStopOnTriggerCommand.h"
#include "NNetModelWriterInterface.h"
#include "NNetParameters.h"
#include "NNetModelCommands.h"

using std::wcout;
using std::endl;
using std::move;
using std::make_unique;
using std::unique_ptr;

void NNetModelCommands::Initialize
( 
	NNetModelReaderInterface * const pNMRI,
	NNetModelWriterInterface * const pNMWI,
	NNetModelImporter        * const pModelImporter,
	Observable               * const pDynamicModelObservable,
	CommandStack             * const pCmdStack
) 
{ 
	m_pNMRI                   = pNMRI;
	m_pNMWI                   = pNMWI;
	m_pModelImporter          = pModelImporter;
	m_pDynamicModelObservable = pDynamicModelObservable;
	m_pCmdStack               = pCmdStack;
}

void NNetModelCommands::UndoCommand()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	if ( ! m_pCmdStack->UndoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelCommands::RedoCommand()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	if ( ! m_pCmdStack->RedoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelCommands::PushCommand( unique_ptr<Command> upCmd )
{
	m_pCmdStack->PushCommand( move(upCmd) );
}

void NNetModelCommands::ResetModel()
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pNMWI->ResetModel();
	m_pCmdStack->Clear();
	m_pDynamicModelObservable->NotifyAll( false );
}

void NNetModelCommands::AddSignal
( 
	MicroMeterCircle const & umCircle,
	TrackNr          const   trackNr
)
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << umCircle << L" " << trackNr << endl;
	m_pCmdStack->PushCommand( make_unique<AddSignalCommand>(umCircle, trackNr) );
}

void NNetModelCommands::DeleteSignal( SignalId const id )
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << id << endl;
	m_pCmdStack->PushCommand( make_unique<DeleteSignalCommand>(id) );
}

void NNetModelCommands::DeleteTrack( TrackNr const nr )
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << nr << endl;
	m_pCmdStack->PushCommand( make_unique<DeleteTrackCommand>(nr) );
}

void NNetModelCommands::InsertTrack( TrackNr const nr )
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << nr << endl;
	m_pCmdStack->PushCommand( make_unique<InsertTrackCommand>(nr) );
}

void NNetModelCommands::CreateInitialNobs()
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pNMWI->CreateInitialNobs();
	m_pDynamicModelObservable->NotifyAll( false );
}

void NNetModelCommands::DeleteNob( NobId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand(move(MakeDeleteCommand(*m_pNMWI, id)));
}

void NNetModelCommands::DeleteSelection()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << endl;
	m_pCmdStack->PushCommand( make_unique<DeleteSelectionCommand>(*m_pNMWI) );
}

void NNetModelCommands::DiscIoConnector(NobId const id)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	Nob * pNob { m_pNMWI->GetNob(id) };
	if ( ! pNob )
		return;
	m_pCmdStack->PushCommand( make_unique<DiscIoConnectorCmd>(*m_pNMWI, *pNob, false) );
}

void NNetModelCommands::DiscBaseKnot(NobId const id)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	Nob * pNob { m_pNMWI->GetNob(id) };
	if ( ! pNob )
		return;
	m_pCmdStack->PushCommand( make_unique<DiscBaseKnotCmd>(*pNob, false) );
}

void NNetModelCommands::SplitNeuron( NobId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand( make_unique<SplitNeuronCmd>(*m_pNMWI, id) );
}

void NNetModelCommands::SplitClosedConnector( NobId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand( make_unique<SplitClosedConnCmd>(*m_pNMWI, id) );
}

void NNetModelCommands::DiscClosedConnector( NobId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
 	m_pCmdStack->PushCommand( make_unique<DiscClosedConnCmd>(*m_pNMWI, id) );
}

void NNetModelCommands::ToggleStopOnTrigger( NobId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand( make_unique<ToggleStopOnTriggerCommand>( id ) );
}

void NNetModelCommands::SetPulseRate( NobId const id, fHertz const fNewValue )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << fNewValue << endl;
	m_pCmdStack->PushCommand( make_unique<SetPulseRateCommand>( id, fNewValue ) );
}

void NNetModelCommands::SetTriggerSound( NobId const id, SoundDescr const & sound )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << sound.m_bOn << L" " << sound.m_frequency << L" " << sound.m_duration << endl;
	m_pCmdStack->PushCommand( make_unique<SetTriggerSoundCommand>( id, sound ) );
}

void NNetModelCommands::SetParameter( ParamType::Value const param, float const fNewValue )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << ParamType::GetName(param) << L" " << fNewValue << endl;
	m_pCmdStack->PushCommand( make_unique<SetParameterCommand>(SetParameterCommand(m_pNMWI->GetParams(), param, fNewValue)) );
}

void NNetModelCommands::MoveNob( NobId const id, MicroMeterPnt const & delta )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << delta << endl;
	m_pCmdStack->PushCommand( make_unique<MoveNobCommand>(*m_pNMWI->GetNob(id), delta) );
}

void NNetModelCommands::Rotate
( 
	NobId           const   id, 
	MicroMeterPnt const & umPntOld, 
	MicroMeterPnt const & umPntNew 
)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << umPntOld << L" " << umPntNew << endl;
	m_pCmdStack->PushCommand( make_unique<RotateCommand>(*m_pNMWI->GetNob(id), umPntOld, umPntNew) );
}

void NNetModelCommands::SetNob( NobId const id, MicroMeterPosDir const posDir )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << posDir << endl;
	m_pCmdStack->PushCommand( make_unique<SetNobCommand>(*m_pNMWI->GetNob(id), posDir) );
}

void NNetModelCommands::MoveSelection( MicroMeterPnt const & delta )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << delta << endl;
	m_pCmdStack->PushCommand( make_unique<MoveSelectionCommand>( delta ) );
}

void NNetModelCommands::SetIoNeurons
( 
	MicroMeterPntVector const & umPntVectorRun,
	unique_ptr<NobIdList>       upNobIds 
)
{
	if ( IsTraceOn() )
		TraceStream() << __func__<< * upNobIds << umPntVectorRun << endl ;
	m_pCmdStack->PushCommand( make_unique<SetIoNeuronsCommand>(umPntVectorRun, move(upNobIds)) );
}

void NNetModelCommands::AddModel()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AddModelCommand>( m_pModelImporter->GetImportedModel()->GetUPNobs() ) );
}

void NNetModelCommands::AddOutgoing2Knot( NobId const id, MicroMeterPnt const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<AddOutgoing2KnotCommand>( id, pos + STD_OFFSET ) );
}

void NNetModelCommands::AddIncoming2Knot( NobId const id, MicroMeterPnt const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<AddIncoming2KnotCommand>( id, pos - STD_OFFSET ) );
}

void NNetModelCommands::AddOutgoing2Pipe( NobId const id, MicroMeterPnt const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<AddOutgoing2PipeCommand>( id, pos ) );
}

void NNetModelCommands::AddIncoming2Pipe( NobId const id, MicroMeterPnt const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<AddIncoming2PipeCommand>( id, pos ) );
}

void NNetModelCommands::InsertKnot( NobId const id, MicroMeterPnt const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<InsertBaseKnotCommand<Knot>>( id, pos ) );
}

void NNetModelCommands::InsertNeuron( NobId const id, MicroMeterPnt const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<InsertBaseKnotCommand<Neuron>>( id, pos ) );
}

void NNetModelCommands::NewInputNeuron( MicroMeterPnt const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<NewInputNeuronCommand>( pos ) );
}

void NNetModelCommands::NewOutputNeuron( MicroMeterPnt const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<NewOutputNeuronCommand>( pos ) );
}

void NNetModelCommands::AppendInputNeuron( NobId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand( make_unique<AppendNeuronCommand<InputNeuron>>(*m_pNMWI, id) );
}

void NNetModelCommands::AppendOutputNeuron( NobId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand( make_unique<AppendNeuronCommand<OutputNeuron>>(*m_pNMWI, id) );
}

void NNetModelCommands::ClearBeepers()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << endl;
	m_pCmdStack->PushCommand( make_unique<ClearBeepersCommand>() );
}

///////////////////// selection commands /////////////////////////////

void NNetModelCommands::CopySelection()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<CopySelectionCommand>(*m_pNMWI) );
}

void NNetModelCommands::RestrictSelection(NobType::Value const val)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << val << endl;
	m_pCmdStack->PushCommand(make_unique<RestrictSelectionCommand>(val));
}

void NNetModelCommands::SelectAllBeepers()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand(make_unique<SelectAllBeepersCommand>());
}

void NNetModelCommands::SelectNob(NobId const id, tBoolOp const op)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << op << endl;
	if (IsDefined(id))
		m_pCmdStack->PushCommand(make_unique<SelectNobCommand>(id, op));
}

void NNetModelCommands::SelectAll(bool const bOn)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << bOn << endl;
	m_pCmdStack->PushCommand( make_unique<SelectAllCommand>(bOn) );
}

void NNetModelCommands::SelectSubtree( NobId const id, bool  const bOn )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << bOn << endl;
	m_pCmdStack->PushCommand( make_unique<SelectSubtreeCommand>( id, bOn ) );
}

void NNetModelCommands::SelectNobsInRect( MicroMeterRect const & rect )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << rect << endl;
	m_pCmdStack->PushCommand( make_unique<SelectNobsInRectCommand>(rect) );
}

void NNetModelCommands::AnalyzeLoops()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AnalyzeCommand>( ModelAnalyzer::FindLoop(*m_pNMRI) ) );
}

void NNetModelCommands::AnalyzeAnomalies()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AnalyzeCommand>( ModelAnalyzer::FindAnomaly(*m_pNMRI) ) );
}
