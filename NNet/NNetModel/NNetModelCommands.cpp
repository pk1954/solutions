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
#include "Attach2MonitorCommand.h"
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
#include "NNetModelReaderInterface.h"
#include "NNetModelWriterInterface.h"
#include "NNetParameters.h"
#include "NNetModelCommands.h"

using std::wcout;
using std::endl;

void NNetModelCommands::Initialize
( 
	wostream                 * const pTraceStream,
	NNetModelReaderInterface * const pReaderInterface,
	NNetModelWriterInterface * const pWriterInterface,
	Param                    * const pParam,
	CommandStack             * const pCmdStack,
	NNetModelStorage         * const pStorage,
	Observable               * const pDynamicModelObservable
) 
{ 
	m_pTraceStream            = pTraceStream;
	m_pModelReaderInterface   = pReaderInterface;
	m_pModelWriterInterface   = pWriterInterface;
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
	BaseKnot * pSrc { m_pModelWriterInterface->GetShapePtr<BaseKnot *>( idSrc ) };
	Shape    * pDst { m_pModelWriterInterface->GetShapePtr<Shape    *>( idDst ) };
	Command  * pCommand;
	if ( pDst->IsPipe() )   // connect baseknot to pipe
		pCommand = new Connect2PipeCommand( m_pModelWriterInterface, pSrc, m_pModelWriterInterface->GetShapePtr<Pipe *>( idDst ) );
	else
		pCommand = new Connect2BaseKnotCommand( pSrc, m_pModelWriterInterface->GetShapePtr<BaseKnot *>( idDst ) );
	m_pCmdStack->NewCommand( pCommand );
}

void NNetModelCommands::deleteShape( Shape* const pShape )
{
	Command * pCommand;
	if ( pShape->IsPipe() ) 
		pCommand = new DeletePipeCommand( static_cast<Pipe *>( pShape ) );
	else 
		pCommand = new DisconnectBaseKnotCommand( m_pModelWriterInterface, static_cast<BaseKnot *>( pShape ), true );
	m_pCmdStack->NewCommand( pCommand );
}

void NNetModelCommands::deleteSelection( )
{
	ShapeList list;                                       // detour with secondary list is neccessary!
	m_pModelWriterInterface->GetShapeList                 // cannot delete shapes directly in Apply2All
	(                                                        
		list, 
		[&]( Shape const & s ) { return s.IsSelected(); } // first construct list
	); 
	list.Apply2All                                        // then run through list 
	( 
		[&]( Shape * pShape ) {	deleteShape( pShape ); }  // and delete shapes in model
	);                                                    // using pointers from list
}

void NNetModelCommands::Attach2Monitor( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	if ( m_pModelReaderInterface->IsOfType<Neuron>( id ) )
	{
		m_pCmdStack->NewCommand
		( 
			new Attach2MonitorCommand
			( 
				id,
				m_pModelWriterInterface->GetMonitorData(), 
				m_pModelReaderInterface, 
				m_pParam,
				m_pDynamicModelObservable
			) 
		);
	}
}

void NNetModelCommands::DeleteShape( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	deleteShape( m_pModelWriterInterface->GetShapePtr<Shape *>( id ) );
}

void NNetModelCommands::Disconnect( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	m_pCmdStack->NewCommand
	( 
		new DisconnectBaseKnotCommand
		( 
			m_pModelWriterInterface, 
			m_pModelWriterInterface->GetShapePtr<BaseKnot *>(id), 
			false 
		) 
	);
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
	m_pCmdStack->NewCommand( new SetPulseRateCommand( m_pModelWriterInterface->GetShapePtr<InputNeuron *>( id ), fNewValue ) );
}

void NNetModelCommands::ResetModel( )
{ 
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModelWriterInterface->ResetModel( );
	m_pCmdStack->Clear();
	m_pStorage->ResetModelPath( );
	m_pModelWriterInterface->CreateInitialShapes();
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
	m_pCmdStack->NewCommand( new SetTriggerSoundCommand( m_pModelWriterInterface, id, sound ) );
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
	Shape * const pShape { m_pModelWriterInterface->GetShape( id ) };
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
	m_pCmdStack->NewCommand( new AddOutgoing2KnotCommand( m_pModelWriterInterface, id, pos + STD_OFFSET) );
}

void NNetModelCommands::AddIncoming2Knot( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( new AddIncoming2KnotCommand( m_pModelWriterInterface, id, pos - STD_OFFSET ) );
}

void NNetModelCommands::AddOutgoing2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( new AddOutgoing2PipeCommand( m_pModelWriterInterface, id, pos ) );
}

void NNetModelCommands::AddIncoming2Pipe( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( new AddIncoming2PipeCommand( m_pModelWriterInterface, id, pos ) );
}

void NNetModelCommands::InsertNeuron( ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << pos << endl;
	m_pCmdStack->NewCommand( new InsertNeuronCommand( m_pModelWriterInterface, id, pos ) );
}

void NNetModelCommands::NewNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << pos << endl;
	m_pCmdStack->NewCommand( new NewNeuronCommand( m_pModelWriterInterface, pos ) );
}

void NNetModelCommands::NewInputNeuron( MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << pos << endl;
	m_pCmdStack->NewCommand( new NewInputNeuronCommand( m_pModelWriterInterface, pos ) );
}

void NNetModelCommands::AppendNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	auto * pCmdNewNeuron 
	{ 
		new NewNeuronCommand
		( 
			m_pModelWriterInterface, 
			m_pModelReaderInterface->GetShapePos(id) 
		) 
	};
	m_pCmdStack->NewCommand( pCmdNewNeuron );
	auto * pCmdNewBaseKnot
	{
		new Connect2BaseKnotCommand
		( 
			m_pModelWriterInterface->GetShapePtr<Knot *>(id), 
			pCmdNewNeuron->GetNeuron() 
		) 
	};
	m_pCmdStack->NewCommand( pCmdNewBaseKnot );
}

void NNetModelCommands::AppendInputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << endl;
	auto * pCmdNewInputNeuron 
	{ 
		new NewInputNeuronCommand
		( 
			m_pModelWriterInterface, 
			m_pModelReaderInterface->GetShapePos(id) 
		) 
	};
	m_pCmdStack->NewCommand( pCmdNewInputNeuron );
	auto * pCmdNewBaseKnot 
	{ 
		new Connect2BaseKnotCommand
		( 
			m_pModelWriterInterface->GetShapePtr<Knot *>(id), 
			pCmdNewInputNeuron->GetInputNeuron() 
		) 
	};
	m_pCmdStack->NewCommand( pCmdNewBaseKnot );
}

void NNetModelCommands::ClearBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new ClearBeepersCommand( m_pModelWriterInterface ) );
}

void NNetModelCommands::MarkSelection( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << op << endl;
	m_pCmdStack->NewCommand( new MarkSelectionCommand( m_pModelWriterInterface, op ) );
}

void NNetModelCommands::DeleteSelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->StartSeries();
	deleteSelection();
	m_pCmdStack->StopSeries();
}

///////////////////// selection commands /////////////////////////////

void NNetModelCommands::CopySelection( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new CopySelectionCommand( m_pModelWriterInterface ) );
}

void NNetModelCommands::SelectAllBeepers( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new SelectAllBeepersCommand( m_pModelWriterInterface ) );
}

void NNetModelCommands::SelectShape( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id  << op << endl;
	m_pCmdStack->NewCommand( new SelectShapeCommand( m_pModelWriterInterface, id, op ) );
}

void NNetModelCommands::SelectAll( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << op << endl;
	m_pCmdStack->NewCommand( new SelectAllCommand( m_pModelWriterInterface, op ) );
}

void NNetModelCommands::SelectSubtree( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << id << op << endl;
	m_pCmdStack->NewCommand( new SelectSubtreeCommand( m_pModelWriterInterface, id, op ) );
}

void NNetModelCommands::SelectShapesInRect( MicroMeterRect const & rect )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << rect << endl;
	m_pCmdStack->NewCommand( new SelectShapesInRectCommand( m_pModelWriterInterface, rect ) );
}

void NNetModelCommands::AnalyzeLoops( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new AnalyzeLoopsCommand( m_pModelWriterInterface ) );
}

void NNetModelCommands::AnalyzeAnomalies( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pCmdStack->NewCommand( new AnalyzeAnomaliesCommand( m_pModelWriterInterface ) );
}
