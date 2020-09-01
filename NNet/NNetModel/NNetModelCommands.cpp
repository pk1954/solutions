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
#include "NNetModelWriterInterface.h"
#include "NNetModelCommands.h"

using std::wcout;
using std::endl;

void NNetModelCommands::Initialize
( 
	wostream                 * const pTraceStream,
	NNetModelWriterInterface * const pWriterInterface,
	CommandStack             * const pCmdStack,
	NNetModelStorage         * const pStorage
) 
{ 
	m_pTraceStream = pTraceStream;
	m_pModel       = pWriterInterface;
	m_pCmdStack    = pCmdStack;
	m_pStorage     = pStorage;
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
	BaseKnot * pSrc { m_pModel->GetShapePtr<BaseKnot *>( idSrc ) };
	Shape    * pDst { m_pModel->GetShapePtr<Shape    *>( idDst ) };
	Command  * pCommand;
	if ( pDst->IsPipe() )   // connect baseknot to pipe
		pCommand = new Connect2PipeCommand( m_pModel, pSrc, m_pModel->GetShapePtr<Pipe *>( idDst ) );
	else
		pCommand = new Connect2BaseKnotCommand( pSrc, m_pModel->GetShapePtr<BaseKnot *>( idDst ) );
	m_pCmdStack->NewCommand( pCommand );
}

void NNetModelCommands::deleteShape( Shape* const pShape )
{
	Command * pCommand;
	if ( pShape->IsPipe() ) 
		pCommand = new DeletePipeCommand( static_cast<Pipe *>( pShape ) );
	else 
		pCommand = new DisconnectBaseKnotCommand( m_pModel, static_cast<BaseKnot *>( pShape ), true );
	m_pCmdStack->NewCommand( pCommand );
}

void NNetModelCommands::DeleteShape( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	deleteShape( m_pModel->GetShapePtr<Shape *>( id ) );
}

void NNetModelCommands::Disconnect( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	m_pCmdStack->NewCommand( new DisconnectBaseKnotCommand( m_pModel, m_pModel->GetShapePtr<BaseKnot *>(id), false ) );
}

void NNetModelCommands::ToggleStopOnTrigger( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	m_pCmdStack->NewCommand( new ToggleStopOnTriggerCommand( id ) );
}

void NNetModelCommands::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << fNewValue << endl;
	m_pCmdStack->NewCommand( new SetPulseRateCommand( m_pModel->GetShapePtr<InputNeuron *>( id ), fNewValue ) );
}

void NNetModelCommands::ResetModel( )
{ 
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModel->ResetModel( );
	m_pCmdStack->Clear();
	m_pStorage->ResetModelPath( );
	m_pModel->CreateInitialShapes();
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
	m_pCmdStack->NewCommand( new SetTriggerSoundCommand( m_pModel, id, sound ) );
}

void NNetModelCommands::SetParameter( tParameter const param, float const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << GetParameterName( param ) << L" " << fNewValue << endl;
	m_pCmdStack->NewCommand( new SetParameterCommand( param, fNewValue ) );
}

void NNetModelCommands::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << delta << endl;
	Shape * const pShape { m_pModel->GetShape( id ) };
	if ( pShape->IsPipe( ) )
		m_pCmdStack->NewCommand( new MovePipeCommand    ( static_cast<Pipe     *>( pShape ), delta ) );
	else
		m_pCmdStack->NewCommand( new MoveBaseKnotCommand( static_cast<BaseKnot *>( pShape ), delta ) );
}

void NNetModelCommands::MoveSelection( MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << delta << endl;
	m_pCmdStack->NewCommand( new MoveSelectionCommand( delta ) );
}

void NNetModelCommands::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( new AddOutgoing2KnotCommand( m_pModel, id, pos + STD_OFFSET) );
}

void NNetModelCommands::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( new AddIncoming2KnotCommand( m_pModel, id, pos - STD_OFFSET ) );
}

void NNetModelCommands::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( new AddOutgoing2PipeCommand( m_pModel, id, pos ) );
}

void NNetModelCommands::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( new AddIncoming2PipeCommand( m_pModel, id, pos ) );
}

void NNetModelCommands::InsertNeuron( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( new InsertNeuronCommand( m_pModel, id, pos ) );
}

void NNetModelCommands::NewNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << pos << endl;
	m_pCmdStack->NewCommand( new NewNeuronCommand( m_pModel, pos ) );
}

void NNetModelCommands::NewInputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << pos << endl;
	m_pCmdStack->NewCommand( new NewInputNeuronCommand( m_pModel, pos ) );
}

void NNetModelCommands::AppendNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	auto * pCmdNewNeuron { new NewNeuronCommand( m_pModel, m_pModel->GetModel().GetShapePos(id) ) };
	m_pCmdStack->NewCommand( pCmdNewNeuron );
	m_pCmdStack->NewCommand( new Connect2BaseKnotCommand( m_pModel->GetShapePtr<Knot *>(id), pCmdNewNeuron->GetNeuron() ) );
}

void NNetModelCommands::AppendInputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	auto * pCmdNewInputNeuron { new NewInputNeuronCommand( m_pModel, m_pModel->GetModel().GetShapePos(id) ) };
	m_pCmdStack->NewCommand( pCmdNewInputNeuron );
	m_pCmdStack->NewCommand( new Connect2BaseKnotCommand( m_pModel->GetShapePtr<Knot *>(id), pCmdNewInputNeuron->GetInputNeuron() ) );
}

void NNetModelCommands::ClearBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new ClearBeepersCommand( m_pModel ) );
}

void NNetModelCommands::MarkSelection( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << op << endl;
	m_pCmdStack->NewCommand( new MarkSelectionCommand( m_pModel, op ) );
}

void NNetModelCommands::DeleteSelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->StartSeries();
	for ( Shape * const pShape : m_pModel->GetShapeList( [&]( Shape const & s ){ return s.IsSelected(); } ) )
		deleteShape( pShape );
	m_pCmdStack->StopSeries();
}

///////////////////// selection commands /////////////////////////////

void NNetModelCommands::CopySelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new CopySelectionCommand( m_pModel ) );
}

void NNetModelCommands::SelectAllBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new SelectAllBeepersCommand( m_pModel ) );
}

void NNetModelCommands::SelectShape( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id  << op << endl;
	m_pCmdStack->NewCommand( new SelectShapeCommand( m_pModel, id, op ) );
}

void NNetModelCommands::SelectAll( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << op << endl;
	m_pCmdStack->NewCommand( new SelectAllCommand( m_pModel, op ) );
}

void NNetModelCommands::SelectSubtree( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << op << endl;
	m_pCmdStack->NewCommand( new SelectSubtreeCommand( m_pModel, id, op ) );
}

void NNetModelCommands::SelectShapesInRect( MicroMeterRect const & rect )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << rect << endl;
	m_pCmdStack->NewCommand( new SelectShapesInRectCommand( m_pModel, rect ) );
}

void NNetModelCommands::AnalyzeLoops( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new AnalyzeLoopsCommand( m_pModel ) );
}

void NNetModelCommands::AnalyzeAnomalies( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new AnalyzeAnomaliesCommand( m_pModel ) );
}
