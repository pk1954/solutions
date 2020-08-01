// NNetModelWriterInterface.cpp
//
// NNetModel

#include "stdafx.h"
#include "AddIncoming2KnotCommand.h"
#include "AddIncoming2PipeCommand.h"
#include "AddOutgoing2KnotCommand.h"
#include "AddOutgoing2PipeCommand.h"
#include "AnalyzeAnomaliesCommand.h"
#include "AnalyzeLoopsCommand.h"
#include "AppendInputNeuronCommand.h"
#include "AppendNeuronCommand.h"
#include "ClearBeepersCommand.h"
#include "Connect2BaseKnotCommand.h"
#include "Connect2PipeCommand.h"
#include "Convert2InputNeuronCommand.h"
#include "Convert2NeuronCommand.h"
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
#include "ResetModelCommand.h"
#include "ResetTimerCommand.h"
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

void NNetModelWriterInterface::Initialize( wostream * pTraceStream ) 
{ 
	m_pTraceStream = pTraceStream;
}

void NNetModelWriterInterface::Start( NNetModel * const pModel )
{
	m_pModel = pModel;
	m_CmdStack.Inizialize( m_pModel );
}

void NNetModelWriterInterface::Stop( )
{
	m_pModel = nullptr;
}

void NNetModelWriterInterface::UndoCommand( )
{
	if ( ! m_CmdStack.UndoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelWriterInterface::RedoCommand( )
{
	if ( ! m_CmdStack.RedoCommand() )
		MessageBeep( MB_ICONWARNING );
}

void NNetModelWriterInterface::ResetTimer( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_CmdStack.NewCommand( new ResetTimerCommand() );
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
	m_CmdStack.NewCommand( pCommand );
}

void NNetModelWriterInterface::DeleteShape( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;

	Shape   * pShape { m_pModel->GetShapePtr<Shape *>( id ) };
	Command * pCommand;
	if ( pShape->IsPipe() )   // connect baseknot to pipe
		pCommand = new DeletePipeCommand( m_pModel, id );
	else 
		pCommand = new DisconnectBaseKnotCommand( m_pModel, id, true );
	m_CmdStack.NewCommand( pCommand );
}

void NNetModelWriterInterface::Disconnect( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new DisconnectBaseKnotCommand( m_pModel, id, false ) );
}

void NNetModelWriterInterface::Convert2Neuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new Convert2NeuronCommand( m_pModel, id ) );
}

void NNetModelWriterInterface::Convert2InputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new Convert2InputNeuronCommand( m_pModel, id ) );
}

void NNetModelWriterInterface::ToggleStopOnTrigger( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_CmdStack.NewCommand( new ToggleStopOnTriggerCommand( id ) );
}

void NNetModelWriterInterface::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << fNewValue << endl;
	m_CmdStack.NewCommand( new SetPulseRateCommand( m_pModel->GetShapePtr<InputNeuron *>( id ), fNewValue ) );
}

void NNetModelWriterInterface::ResetModel( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_CmdStack.NewCommand( new ResetModelCommand( m_pModel ) );
}

void NNetModelWriterInterface::SetTriggerSound( ShapeId const id, SoundDescr const & sound )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << sound.m_bOn << L" " << sound.m_frequency << L" " << sound.m_duration << endl;

	m_CmdStack.NewCommand( new SetTriggerSoundCommand( m_pModel, id, sound ) );
}

void NNetModelWriterInterface::SetParameter( tParameter const param, float const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetParameterName( param ) << L" " << fNewValue << endl;

	m_CmdStack.NewCommand( new SetParameterCommand( param, fNewValue ) );
}

void NNetModelWriterInterface::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << delta << endl;

	Shape * const pShape { m_pModel->GetShape( id ) };
	if ( pShape->IsPipe( ) )
		m_CmdStack.NewCommand( new MovePipeCommand    ( static_cast<Pipe     *>( pShape ), delta ) );
	else
		m_CmdStack.NewCommand( new MoveBaseKnotCommand( static_cast<BaseKnot *>( pShape ), delta ) );
}

void NNetModelWriterInterface::MoveSelection( MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << delta << endl;

	m_CmdStack.NewCommand( new MoveSelectionCommand( delta ) );
}

void NNetModelWriterInterface::AddOutgoing2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;

	m_CmdStack.NewCommand( new AddOutgoing2KnotCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;

	m_CmdStack.NewCommand( new AddIncoming2KnotCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;

	m_CmdStack.NewCommand( new AddOutgoing2PipeCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << pos << endl;

	m_CmdStack.NewCommand( new AddIncoming2PipeCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::InsertNeuron( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue( ) << L" "<< pos << endl;

	m_CmdStack.NewCommand( new InsertNeuronCommand( m_pModel, id, pos ) );
}

void NNetModelWriterInterface::NewNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << pos << endl;

	m_CmdStack.NewCommand( new NewNeuronCommand( m_pModel, pos ) );
}

void NNetModelWriterInterface::NewInputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << pos << endl;

	m_CmdStack.NewCommand( new NewInputNeuronCommand( m_pModel, pos ) );
}

void NNetModelWriterInterface::AppendNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;

	m_CmdStack.NewCommand( new AppendNeuronCommand( m_pModel, id ) );
}

void NNetModelWriterInterface::AppendInputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;

	m_CmdStack.NewCommand( new AppendInputNeuronCommand( m_pModel, id ) );
}

void NNetModelWriterInterface::ClearBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new ClearBeepersCommand( m_pModel ) );
}

void NNetModelWriterInterface::MarkSelection( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new MarkSelectionCommand( m_pModel, op ) );
}

void NNetModelWriterInterface::DeleteSelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	vector<Shape *> m_selection { m_pModel->GetShapeList( [&]( Shape const & s ){ return s.IsSelected(); } ) };
	for( Shape * const pShape : m_selection )
		DeleteShape( pShape->GetId( ) );
}

///////////////////// selection commands /////////////////////////////

void NNetModelWriterInterface::CopySelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new CopySelectionCommand( m_pModel ) );
}

void NNetModelWriterInterface::SelectAllBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << endl;

	m_CmdStack.NewCommand( new SelectAllBeepersCommand( m_pModel ) );
}

void NNetModelWriterInterface::SelectShape( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << GetBoolOpName( op ) << endl;

	m_CmdStack.NewCommand( new SelectShapeCommand( m_pModel, id, op ) );
}

void NNetModelWriterInterface::SelectAll( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;

	m_CmdStack.NewCommand( new SelectAllCommand( m_pModel, op ) );
}

void NNetModelWriterInterface::SelectSubtree( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;

	m_CmdStack.NewCommand( new SelectSubtreeCommand( m_pModel, id, op ) );
}

void NNetModelWriterInterface::SelectShapesInRect( MicroMeterRect const & rect )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << rect << endl;

	m_CmdStack.NewCommand( new SelectShapesInRectCommand( m_pModel, rect ) );
}

void NNetModelWriterInterface::AnalyzeLoops( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new AnalyzeLoopsCommand( m_pModel ) );
}

void NNetModelWriterInterface::AnalyzeAnomalies( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;

	m_CmdStack.NewCommand( new AnalyzeAnomaliesCommand( m_pModel ) );
}
