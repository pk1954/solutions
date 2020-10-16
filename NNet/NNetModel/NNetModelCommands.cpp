// NNetModelCommands.cpp
//
// NNetModel

#include "stdafx.h"
#include "CommandStack.h"
#include "AddIncoming2KnotCommand.h"
#include "AddIncoming2PipeCommand.h"
#include "AddOutgoing2KnotCommand.h"
#include "AddOutgoing2PipeCommand.h"
#include "AnalyzeAnomaliesCommand.h"
#include "AnalyzeLoopsCommand.h"
#include "ClearBeepersCommand.h"
#include "Connect2BaseKnotCommand.h"
#include "Connect2PipeCommand.h"
#include "CopySelectionCommand.h"
#include "DeletePipeCommand.h"
#include "DisconnectBaseKnotCommand.h"
#include "InsertNeuronCommand.h"
#include "InsertTrackCommand.h"
#include "MarkSelectionCommand.h"
#include "MoveBaseKnotCommand.h"
#include "MovePipeCommand.h"
#include "MoveSelectionCommand.h"
#include "NewInputNeuronCommand.h"
#include "NewNeuronCommand.h"
#include "NNetModelStorage.h"
#include "SelectAllBeepersCommand.h"
#include "SelectAllCommand.h"
#include "SelectionCommand.h"
#include "SelectShapeCommand.h"
#include "SelectShapesInRectCommand.h"
#include "SelectSubtreeCommand.h"
#include "SetParameterCommand.h"
#include "SetPulseRateCommand.h"
#include "SetTriggerSoundCommand.h"
#include "ToggleStopOnTriggerCommand.h"
#include "NNetModelReaderInterface.h"
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
	NNetModelReaderInterface * const pReaderInterface,
	NNetModelWriterInterface * const pWriterInterface,
	Param                    * const pParam,
	CommandStack             * const pCmdStack,
	NNetModelStorage         * const pStorage,
	Observable               * const pDynamicModelObservable
) 
{ 
	m_pMRI   = pReaderInterface;
	m_pMWI   = pWriterInterface;
	m_pCmdStack               = pCmdStack;
	m_pStorage                = pStorage;
	m_pParam                  = pParam;
	m_pDynamicModelObservable = pDynamicModelObservable;
}

void NNetModelCommands::UndoCommand( )
{
	if ( ! m_pCmdStack->UndoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelCommands::RedoCommand( )
{
	if ( ! m_pCmdStack->RedoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelCommands::Connect( ShapeId const idSrc, ShapeId const idDst )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << idSrc << idDst << endl;
	unique_ptr<Command> pCmd;
	if ( m_pMWI->IsPipe( idDst ) ) 
		pCmd = make_unique<Connect2PipeCommand>( m_pMWI, idSrc, idDst );
	else
		pCmd = make_unique<Connect2BaseKnotCommand>( m_pMWI, idSrc, idDst );
	m_pCmdStack->NewCommand( move( pCmd ) );
}

void NNetModelCommands::deleteShape( Shape * const pShape )
{
	unique_ptr<Command> pCmd;
	if ( pShape->IsPipe() ) 
		pCmd = make_unique<DeletePipeCommand>( static_cast<Pipe *>( pShape ) ) ;
	else 
		pCmd = make_unique<DisconnectBaseKnotCommand>( m_pMWI, static_cast<BaseKnot *>(pShape), true );
	m_pCmdStack->NewCommand( move( pCmd ) );
}

void NNetModelCommands::Attach2Monitor( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	if ( m_pMRI->IsOfType<Neuron>( id ) )
	{
		MonitorData * const pMonitorData { m_pMWI->GetMonitorData() }; 
		pMonitorData->InsertTrack( TrackNr(0) );
		pMonitorData->AddSignal( id, TrackNr(0) );
	}
}

void NNetModelCommands::InsertTrack( TrackNr const trackNr )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << trackNr << endl;
	m_pCmdStack->NewCommand( make_unique<InsertTrackCommand>( trackNr ) );
}

void NNetModelCommands::DeleteShape( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	deleteShape( m_pMWI->GetShapePtr<Shape *>( id ) );
}

void NNetModelCommands::DeleteSignal( SignalId const & id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	m_pMWI->GetMonitorData()->DeleteSignal( id );
}

void NNetModelCommands::Disconnect( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	m_pCmdStack->NewCommand( make_unique<DisconnectBaseKnotCommand>(m_pMWI, m_pMWI->GetShapePtr<BaseKnot *>(id), false) );
}

void NNetModelCommands::ToggleStopOnTrigger( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	m_pCmdStack->NewCommand( make_unique<ToggleStopOnTriggerCommand>( id ) );
}

void NNetModelCommands::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << fNewValue << endl;
	m_pCmdStack->NewCommand( make_unique<SetPulseRateCommand>( m_pMWI->GetShapePtr<InputNeuron *>(id), fNewValue ) );
}

void NNetModelCommands::ResetModel( )
{ 
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pMWI->ResetModel( );
	m_pCmdStack->Clear();
	m_pStorage->ResetModelPath( );
	m_pMWI->CreateInitialShapes();
}

void NNetModelCommands::ReadModel( bool bConcurrently, wstring const wstrPath )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << bConcurrently << L" " << wstrPath << endl;
	m_pStorage->Read( bConcurrently, wstrPath );
	m_pCmdStack->Clear();
}

void NNetModelCommands::SetTriggerSound( ShapeId const id, SoundDescr const & sound )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << L" " << sound.m_bOn << sound.m_frequency << L" " << sound.m_duration << endl;
	m_pCmdStack->NewCommand( make_unique<SetTriggerSoundCommand>( m_pMWI, id, sound ) );
}

void NNetModelCommands::SetParameter( tParameter const param, float const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << GetParameterName( param ) << L" " << fNewValue << endl;
	m_pCmdStack->NewCommand( make_unique<SetParameterCommand>( SetParameterCommand( m_pParam, param, fNewValue ) ) );
}

void NNetModelCommands::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << delta << endl;
	Shape * const pShape { m_pMWI->GetShape( id ) };
	if ( pShape->IsPipe( ) )
	{
		auto pCmd = make_unique<MovePipeCommand>( static_cast<Pipe *>( pShape ), delta );
		m_pCmdStack->NewCommand( move( pCmd ) );
	}
	else 
	{
		auto pCmd = make_unique<MoveBaseKnotCommand>( static_cast<BaseKnot *>( pShape ), delta );
		m_pCmdStack->NewCommand( move( pCmd ) );
	}
}

void NNetModelCommands::MoveSelection( MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << delta << endl;
	m_pCmdStack->NewCommand( make_unique<MoveSelectionCommand>( delta ) );
}

void NNetModelCommands::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( make_unique<AddOutgoing2KnotCommand>(m_pMWI, id, pos + STD_OFFSET ) );
}

void NNetModelCommands::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( make_unique<AddIncoming2KnotCommand>( m_pMWI, id, pos - STD_OFFSET ) );
}

void NNetModelCommands::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( make_unique<AddOutgoing2PipeCommand>(  m_pMWI, id, pos ) );
}

void NNetModelCommands::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( make_unique<AddIncoming2PipeCommand>( m_pMWI, id, pos ) );
}

void NNetModelCommands::InsertNeuron( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( make_unique<InsertNeuronCommand>( m_pMWI, id, pos ) );
}

void NNetModelCommands::NewNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << pos << endl;
	m_pCmdStack->NewCommand( make_unique<NewNeuronCommand>( m_pMWI, pos ) );
}

void NNetModelCommands::NewInputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << pos << endl;
	m_pCmdStack->NewCommand( make_unique<NewInputNeuronCommand>( m_pMWI, pos ) );
}

void NNetModelCommands::AppendNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	auto pCmdNewNeuron        { make_unique<NewNeuronCommand>       ( m_pMWI, m_pMRI->GetShapePos(id) ) };
	auto pCmdConnect2BaseKnot { make_unique<Connect2BaseKnotCommand>( m_pMWI, id, pCmdNewNeuron->GetNeuron()->GetId() ) };
	m_pCmdStack->NewCommand( move( pCmdNewNeuron ) );
	m_pCmdStack->NewCommand( move( pCmdConnect2BaseKnot ) );
}

void NNetModelCommands::AppendInputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	auto pCmdNewInputNeuron{ make_unique<NewInputNeuronCommand>  ( m_pMWI, m_pMRI->GetShapePos(id) ) };
	auto pCmdNewBaseKnot   { make_unique<Connect2BaseKnotCommand>( m_pMWI, id, pCmdNewInputNeuron->GetInputNeuron()->GetId() ) };
	m_pCmdStack->NewCommand( move( pCmdNewInputNeuron ) );
	m_pCmdStack->NewCommand( move( pCmdNewBaseKnot ) );
}

void NNetModelCommands::ClearBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( make_unique<ClearBeepersCommand>( m_pMWI ) );
}

void NNetModelCommands::MarkSelection( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << op << endl;
	m_pCmdStack->NewCommand( make_unique<MarkSelectionCommand>( m_pMWI, op ) );
}

void NNetModelCommands::DeleteSelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
//	m_pCmdStack->StartSeries();
	{
		ShapeList list;                                       // detour with secondary list is neccessary!
		m_pMWI->GetShapeList                                  // cannot delete shapes directly in Apply2All
		(                                                        
			list, 
			[&]( Shape const & s ) { return s.IsSelected(); } // first construct list
		); 
		list.Apply2All                                        // then run through list 
		( 
			[&]( Shape & shape ) { deleteShape( & shape ); }  // and delete shapes in model
		);                                                    // using pointers from list
	}

//	m_pCmdStack->StopSeries();
}

///////////////////// selection commands /////////////////////////////

void NNetModelCommands::CopySelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( make_unique<CopySelectionCommand>( m_pMWI ) );
}

void NNetModelCommands::SelectAllBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( make_unique<SelectAllBeepersCommand>( m_pMWI ) );
}

void NNetModelCommands::SelectShape( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id  << op << endl;
	m_pCmdStack->NewCommand( make_unique<SelectShapeCommand>( m_pMWI, id, op ) );
}

void NNetModelCommands::SelectAll( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << op << endl;
	m_pCmdStack->NewCommand( make_unique<SelectAllCommand>( m_pMWI, op  ) );
}

void NNetModelCommands::SelectSubtree( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << op << endl;
	m_pCmdStack->NewCommand( make_unique<SelectSubtreeCommand>( m_pMWI, id, op ) );
}

void NNetModelCommands::SelectShapesInRect( MicroMeterRect const & rect )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << rect << endl;
	m_pCmdStack->NewCommand( make_unique<SelectShapesInRectCommand>( m_pMWI, rect ) );
}

void NNetModelCommands::AnalyzeLoops( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( make_unique<AnalyzeLoopsCommand>( m_pMWI ) );
}

void NNetModelCommands::AnalyzeAnomalies( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( make_unique<AnalyzeAnomaliesCommand>( m_pMWI ) );
}
