// NNetModelWriterInterface.cpp
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
#include "NNetModelWriterInterface.h"
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

using std::wcout;
using std::endl;

void NNetModelWriterInterface::Initialize
( 
	wostream         * const pTraceStream,
	CommandStack     * const pCmdStack,
	NNetModelStorage * const pStorage
) 
{ 
	m_pTraceStream = pTraceStream;
	m_pCmdStack    = pCmdStack;
	m_pStorage     = pStorage;
}

void NNetModelWriterInterface::Start( NNetModel * const pModel )
{
	m_pModel = pModel;
}

void NNetModelWriterInterface::Stop( )
{
	m_pModel = nullptr;
}

void NNetModelWriterInterface::UndoCommand( )
{
	if ( ! m_pCmdStack->UndoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelWriterInterface::RedoCommand( )
{
	if ( ! m_pCmdStack->RedoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelWriterInterface::Connect( ShapeId const idSrc, ShapeId const idDst )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << idSrc.GetValue() << L" " << idDst.GetValue() << endl;
	BaseKnot * pSrc { m_pModel->GetShapePtr<BaseKnot *>( idSrc ) };
	Shape    * pDst { m_pModel->GetShapePtr<Shape    *>( idDst ) };
	Command  * pCommand;
	if ( pDst->IsPipe() )   // connect baseknot to pipe
		pCommand = new Connect2PipeCommand( m_pModel, pSrc, m_pModel->GetShapePtr<Pipe *>( idDst ) );
	else
		pCommand = new Connect2BaseKnotCommand( pSrc, m_pModel->GetShapePtr<BaseKnot *>( idDst ) );
	m_pCmdStack->NewCommand( pCommand );
}

void NNetModelWriterInterface::deleteShape( Shape* const pShape )
{
	Command * pCommand;
	if ( pShape->IsPipe() ) 
		pCommand = new DeletePipeCommand( static_cast<Pipe *>( pShape ) );
	else 
		pCommand = new DisconnectBaseKnotCommand( m_pModel, static_cast<BaseKnot *>( pShape ), true );
	m_pCmdStack->NewCommand( pCommand );
}

void NNetModelWriterInterface::DeleteShape( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	deleteShape( m_pModel->GetShapePtr<Shape *>( id ) );
}

void NNetModelWriterInterface::Disconnect( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pCmdStack->NewCommand( new DisconnectBaseKnotCommand( m_pModel, m_pModel->GetShapePtr<BaseKnot *>(id), false ) );
}

void NNetModelWriterInterface::ToggleStopOnTrigger( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pCmdStack->NewCommand( new ToggleStopOnTriggerCommand( id ) );
}

void NNetModelWriterInterface::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << fNewValue << endl;
	m_pCmdStack->NewCommand( new SetPulseRateCommand( m_pModel->GetShapePtr<InputNeuron *>( id ), fNewValue ) );
}

void NNetModelWriterInterface::ResetModel( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModel->ResetModel( );
	m_pCmdStack->Clear();
	m_pStorage->ResetModelPath( );
	m_pModel->CreateInitialShapes();
}

void NNetModelWriterInterface::ReadModel( bool bConcurrently, wstring const wstrPath )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << bConcurrently << L" " << wstrPath << endl;
	m_pStorage->Read( bConcurrently, wstrPath );
	m_pCmdStack->Clear();
}

void NNetModelWriterInterface::SetTriggerSound( ShapeId const id, SoundDescr const & sound )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << sound.m_bOn << L" " << sound.m_frequency << L" " << sound.m_duration << endl;
	m_pCmdStack->NewCommand( new SetTriggerSoundCommand( m_pModel, id, sound ) );
}

void NNetModelWriterInterface::SetParameter( tParameter const param, float const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetParameterName( param ) << L" " << fNewValue << endl;
	m_pCmdStack->NewCommand( new SetParameterCommand( param, fNewValue ) );
}

void NNetModelWriterInterface::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << delta << endl;
	Shape * const pShape { m_pModel->GetShape( id ) };
	if ( pShape->IsPipe( ) )
		m_pCmdStack->NewCommand( new MovePipeCommand    ( static_cast<Pipe     *>( pShape ), delta ) );
	else
		m_pCmdStack->NewCommand( new MoveBaseKnotCommand( static_cast<BaseKnot *>( pShape ), delta ) );
}

void NNetModelWriterInterface::MoveSelection( MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << delta << endl;
	m_pCmdStack->NewCommand( new MoveSelectionCommand( delta ) );
}

void NNetModelWriterInterface::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;
	m_pCmdStack->NewCommand( new AddOutgoing2KnotCommand( m_pModel, id, pos + STD_OFFSET) );
}

void NNetModelWriterInterface::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;
	m_pCmdStack->NewCommand( new AddIncoming2KnotCommand( m_pModel, id, pos - STD_OFFSET ) );
}

void NNetModelWriterInterface::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;
	m_pCmdStack->NewCommand( new AddOutgoing2PipeCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;
	m_pCmdStack->NewCommand( new AddIncoming2PipeCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::InsertNeuron( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue( ) << L" "<< pos << endl;
	m_pCmdStack->NewCommand( new InsertNeuronCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::NewNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << pos << endl;
	m_pCmdStack->NewCommand( new NewNeuronCommand( m_pModel, pos ) );
}

void NNetModelWriterInterface::NewInputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << pos << endl;
	m_pCmdStack->NewCommand( new NewInputNeuronCommand( m_pModel, pos ) );
}

void NNetModelWriterInterface::AppendNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	auto * pCmdNewNeuron { new NewNeuronCommand( m_pModel, m_pModel->GetShapePos(id) ) };
	m_pCmdStack->NewCommand( pCmdNewNeuron );
	m_pCmdStack->NewCommand( new Connect2BaseKnotCommand( m_pModel->GetShapePtr<Knot *>(id), pCmdNewNeuron->GetNeuron() ) );
}

void NNetModelWriterInterface::AppendInputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	auto * pCmdNewInputNeuron { new NewInputNeuronCommand( m_pModel, m_pModel->GetShapePos(id) ) };
	m_pCmdStack->NewCommand( pCmdNewInputNeuron );
	m_pCmdStack->NewCommand( new Connect2BaseKnotCommand( m_pModel->GetShapePtr<Knot *>(id), pCmdNewInputNeuron->GetInputNeuron() ) );
}

void NNetModelWriterInterface::ClearBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new ClearBeepersCommand( m_pModel ) );
}

void NNetModelWriterInterface::MarkSelection( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new MarkSelectionCommand( m_pModel, op ) );
}

void NNetModelWriterInterface::DeleteSelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->StartSeries();
	for ( Shape * const pShape : m_pModel->GetShapeList( [&]( Shape const & s ){ return s.IsSelected(); } ) )
		deleteShape( pShape );
	m_pCmdStack->StopSeries();
}

///////////////////// selection commands /////////////////////////////

void NNetModelWriterInterface::CopySelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new CopySelectionCommand( m_pModel ) );
}

void NNetModelWriterInterface::SelectAllBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << endl;
	m_pCmdStack->NewCommand( new SelectAllBeepersCommand( m_pModel ) );
}

void NNetModelWriterInterface::SelectShape( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << GetBoolOpName( op ) << endl;
	m_pCmdStack->NewCommand( new SelectShapeCommand( m_pModel, id, op ) );
}

void NNetModelWriterInterface::SelectAll( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;
	m_pCmdStack->NewCommand( new SelectAllCommand( m_pModel, op ) );
}

void NNetModelWriterInterface::SelectSubtree( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;
	m_pCmdStack->NewCommand( new SelectSubtreeCommand( m_pModel, id, op ) );
}

void NNetModelWriterInterface::SelectShapesInRect( MicroMeterRect const & rect )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << rect << endl;
	m_pCmdStack->NewCommand( new SelectShapesInRectCommand( m_pModel, rect ) );
}

void NNetModelWriterInterface::AnalyzeLoops( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new AnalyzeLoopsCommand( m_pModel ) );
}

void NNetModelWriterInterface::AnalyzeAnomalies( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new AnalyzeAnomaliesCommand( m_pModel ) );
}
