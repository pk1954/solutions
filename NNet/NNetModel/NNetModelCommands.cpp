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
#include "AppendInputNeuronCommand.h"
#include "ClearBeepersCommand.h"
#include "CommandStack.h"
#include "CommandFunctions.h"
#include "CopySelectionCommand.h"
#include "CreateConnectorCommand.h"
#include "DeleteSelectionCommand.h"
#include "DeleteSignalCommand.h"
#include "DeleteTrackCommand.h"
#include "InsertBaseKnotCommand.h"
#include "InsertTrackCommand.h"
#include "MoveShapeCommand.h"
#include "MoveSelectionCommand.h"
#include "NewNeuronCommandT.h"
#include "NNetModelImporter.h"
#include "NNetModelStorage.h"
#include "RestrictSelectionCommand.h"
#include "RotateConnectorCommand.h"
#include "SelectAllBeepersCommand.h"
#include "SelectAllCommand.h"
#include "SelectionCommand.h"
#include "SelectShapeCommand.h"
#include "SelectShapesInRectCommand.h"
#include "SelectSubtreeCommand.h"
#include "SetConnectionNeuronsCommand.h"
#include "SetParameterCommand.h"
#include "SetPulseRateCommand.h"
#include "SetShapeCommand.h"
#include "SetTriggerSoundCommand.h"
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

void NNetModelCommands::CreateInitialShapes()
{ 
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pNMWI->CreateInitialShapes();
	m_pDynamicModelObservable->NotifyAll( false );
}

void NNetModelCommands::DeleteShape( ShapeId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	if ( Shape * pShape { m_pNMWI->GetShape(id) } )
	{
		m_pCmdStack->PushCommand( move( MakeDeleteCommand(*pShape) ) );
	}
}

void NNetModelCommands::DeleteSelection()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << endl;
	m_pCmdStack->PushCommand( make_unique<DeleteSelectionCommand>() );
}

void NNetModelCommands::Connect( ShapeId const idSrc, ShapeId const idDst )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << idSrc << L" " << idDst << endl;
	Shape & shapeDst { * m_pNMWI->GetShapePtr<Shape *>(idDst) };

	if (shapeDst.IsPipe())
		m_pCmdStack->PushCommand
		(
			MakeConnectCommand
			(
				* m_pNMWI->GetShapePtr<BaseKnot *>(idSrc), 
				shapeDst
			) 
		);
	else if (shapeDst.IsBaseKnot())
		m_pCmdStack->PushCommand
		(
			MakeConnectCommand
			(
				* m_pNMWI->GetShapePtr<BaseKnot *>(idSrc), 
				shapeDst
			) 
		);
	else if ( shapeDst.IsConnector( ) )
	{

	}
	else
	{ 
		assert(false);
	}
}

void NNetModelCommands::Disconnect( ShapeId const id )
{
	unique_ptr<Command> pCmd;
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand( move( MakeDisconnectCommand(*m_pNMWI->GetShape(id)) ) );
}

void NNetModelCommands::ToggleStopOnTrigger( ShapeId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand( make_unique<ToggleStopOnTriggerCommand>( id ) );
}

void NNetModelCommands::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << fNewValue << endl;
	m_pCmdStack->PushCommand( make_unique<SetPulseRateCommand>( id, fNewValue ) );
}

void NNetModelCommands::SetTriggerSound( ShapeId const id, SoundDescr const & sound )
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

void NNetModelCommands::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << delta << endl;
	m_pCmdStack->PushCommand( make_unique<MoveShapeCommand>(id, delta) );
}

void NNetModelCommands::RotateConnector
( 
	ShapeId         const   id, 
	MicroMeterPoint const & umPntOld, 
	MicroMeterPoint const & umPntNew 
)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << umPntOld << L" " << umPntNew << endl;
	m_pCmdStack->PushCommand( make_unique<RotateConnectorCommand>(id, umPntOld, umPntNew) );
}

void NNetModelCommands::SetShape( ShapeId const id, MicroMeterPosDir const posDir )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << posDir << endl;
	m_pCmdStack->PushCommand( make_unique<SetShapeCommand>(id, posDir) );
}

void NNetModelCommands::MoveSelection( MicroMeterPoint const & delta )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << delta << endl;
	m_pCmdStack->PushCommand( make_unique<MoveSelectionCommand>( delta ) );
}

void NNetModelCommands::SetConnectionNeurons
( 
	MicroMeterPointVector const & umPntVectorRun,
	unique_ptr<ShapeIdList>       upShapeIds 
)
{
	if ( IsTraceOn() )
		TraceStream() << __func__<< * upShapeIds << umPntVectorRun << endl ;
	m_pCmdStack->PushCommand( make_unique<SetConnectionNeuronsCommand>(umPntVectorRun, move(upShapeIds)) );
}

void NNetModelCommands::CreateConnector(unique_ptr<ShapePtrList<ConnNeuron>> upShapes)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << upShapes << endl;
	m_pCmdStack->PushCommand( make_unique<CreateConnectorCommand>(move(upShapes)) );
}

void NNetModelCommands::CreateConnector(unique_ptr<ShapeIdList> upList)  // only used in wrapper function
{
	unique_ptr<ShapePtrList<ConnNeuron>> upShapes { make_unique<ShapePtrList<ConnNeuron>>() };
	upList->Apply2All([&](ShapeId const id) { upShapes->Add(m_pNMWI->GetShapePtr<ConnNeuron*>(id)); });
	CreateConnector( move(upShapes) );
}

void NNetModelCommands::AddModel()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<AddModelCommand>( m_pModelImporter->GetImportedModel()->GetUPShapes() ) );
}

void NNetModelCommands::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<AddOutgoing2KnotCommand>( id, pos + STD_OFFSET ) );
}

void NNetModelCommands::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<AddIncoming2KnotCommand>( id, pos - STD_OFFSET ) );
}

void NNetModelCommands::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<AddOutgoing2PipeCommand>( id, pos ) );
}

void NNetModelCommands::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<AddIncoming2PipeCommand>( id, pos ) );
}

void NNetModelCommands::InsertKnot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<InsertBaseKnotCommand<Knot>>( id, pos ) );
}

void NNetModelCommands::InsertNeuron( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<InsertBaseKnotCommand<Neuron>>( id, pos ) );
}

void NNetModelCommands::NewNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<NewNeuronCommand>( pos ) );
}

void NNetModelCommands::NewInputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<NewInputNeuronCommand>( pos ) );
}

void NNetModelCommands::NewOutputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << pos << endl;
	m_pCmdStack->PushCommand( make_unique<NewOutputNeuronCommand>( pos ) );
}

void NNetModelCommands::AppendNeuron( ShapeId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand( make_unique<AppendNeuronCommand>( id ) );
}

void NNetModelCommands::AppendInputNeuron( ShapeId const id )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << endl;
	m_pCmdStack->PushCommand( make_unique<AppendInputNeuronCommand>( id ) );
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
	m_pCmdStack->PushCommand( make_unique<CopySelectionCommand>() );
}

void NNetModelCommands::RestrictSelection( ShapeType::Value const val )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << val << endl;
	m_pCmdStack->PushCommand( make_unique<RestrictSelectionCommand>( val ) );
}

void NNetModelCommands::SelectAllBeepers()
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << endl;
	m_pCmdStack->PushCommand( make_unique<SelectAllBeepersCommand>() );
}

void NNetModelCommands::SelectShape( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << op << endl;
	m_pCmdStack->PushCommand( make_unique<SelectShapeCommand>( id, op ) );
}

void NNetModelCommands::SelectAll(bool const bOn)
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << bOn << endl;
	m_pCmdStack->PushCommand( make_unique<SelectAllCommand>(bOn) );
}

void NNetModelCommands::SelectSubtree( ShapeId const id, bool  const bOn )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << id << L" " << bOn << endl;
	m_pCmdStack->PushCommand( make_unique<SelectSubtreeCommand>( id, bOn ) );
}

void NNetModelCommands::SelectShapesInRect( MicroMeterRect const & rect, bool const bAdd2Selection )
{
	if ( IsTraceOn() )
		TraceStream() << __func__ << L" " << rect << bAdd2Selection << endl;
	m_pCmdStack->PushCommand( make_unique<SelectShapesInRectCommand>(rect, bAdd2Selection) );
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
