// NNetModelWriterInterface.cpp
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include <unordered_map>
#include "Resource.h"
#include "PixelTypes.h"
#include "Analyzer.h"
#include "win32_util.h"
#include "win32_script.h"
#include "NNetModel.h"
#include "BaseKnot.h"
#include "NNetModelWriterInterface.h"

using std::wostream;
using std::wcout;
using std::endl;
using std::unordered_map;

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
	class ResetTimerCommand : public Command
	{
	public:
		virtual void Do  ( NNetModel * const pModel ) { m_simuTime = pModel->SetSimulationTime(); }
		virtual void Undo( NNetModel * const pModel ) { pModel->SetSimulationTime( m_simuTime ); }

	private:
		fMicroSecs m_simuTime {};
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_CmdStack.NewCommand( new ResetTimerCommand() );
}

void NNetModelWriterInterface::Connect( ShapeId const idSrc, ShapeId const idDst )
{
	class Connect2PipeCommand : public Command
	{
	public:
		Connect2PipeCommand( BaseKnot * const pSrc, Pipe * const pDst )
		  :	m_pBaseKnot ( pSrc ),
			m_pPipe     ( pDst ),
			m_pNewPipe  ( nullptr ),
			m_pStartKnot( nullptr )
		{ }

		~Connect2PipeCommand( )
		{ 
			delete m_pNewPipe;
		}

		virtual void Do( NNetModel * const pModel )
		{
			m_pStartKnot = m_pPipe->GetStartKnotPtr( );

			m_pNewPipe = new Pipe( m_pStartKnot, m_pBaseKnot );
			m_pNewPipe->Recalc();
			pModel->Add2ShapeList( m_pNewPipe );

			m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
			m_pBaseKnot->m_connections.AddOutgoing( m_pPipe );
			
			m_pPipe->SetStartKnot( m_pBaseKnot );
			m_pPipe->Recalc();
		}

		virtual void Undo( NNetModel * const pModel )
		{
			m_pPipe->SetStartKnot( m_pStartKnot );
			m_pStartKnot->m_connections.RemoveOutgoing( m_pNewPipe );
			m_pStartKnot->m_connections.AddOutgoing( m_pPipe );
			pModel->RemoveFromShapeList( m_pNewPipe );
		}

		virtual void Redo( NNetModel * const pModel )
		{
			m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
			m_pPipe->SetStartKnot( m_pBaseKnot );
			pModel->RestoreToShapeList( m_pNewPipe );
		}

	private:
		BaseKnot * m_pBaseKnot;
		Pipe     * m_pPipe;
		Pipe     * m_pNewPipe;
		BaseKnot * m_pStartKnot;
	};

	class Connect2BaseKnotCommand : public Command
	{
	public:
		Connect2BaseKnotCommand( BaseKnot * const pSrc, BaseKnot * const pDst )
		  :	m_pBaseKnotSrc( pSrc ),
			m_pBaseKnotDst( pDst ),
			m_pDstConnections( nullptr )
		{ }

		~Connect2BaseKnotCommand( )
		{
			delete m_pBaseKnotSrc;
			delete m_pDstConnections;
		}

		virtual void Do( NNetModel * const pModel )
		{
			m_pDstConnections = m_pBaseKnotDst->m_connections.Clone();
			m_pBaseKnotDst->AddConnections( m_pBaseKnotSrc );
			pModel->RemoveFromShapeList( m_pBaseKnotSrc ); // m_pBaseKnotSrc disconnected, but unchanged. Do not delete, will be reused in Undo
		}

		virtual void Undo( NNetModel * const pModel )
		{
			m_pBaseKnotDst->SetConnections( m_pDstConnections );  // restore dst connections
			m_pBaseKnotSrc->RestoreConnections( );
			pModel->RestoreToShapeList( m_pBaseKnotSrc );           // reconnect src  
		}

		virtual void Redo( NNetModel * const pModel )
		{
			m_pBaseKnotDst->AddConnections( m_pBaseKnotSrc );
			pModel->RemoveFromShapeList( m_pBaseKnotSrc ); 
		}

	private:
		BaseKnot    * m_pBaseKnotSrc;
		BaseKnot    * m_pBaseKnotDst;
		Connections * m_pDstConnections;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << idSrc.GetValue() << L" " << idDst.GetValue() << endl;
	BaseKnot * pSrc { m_pModel->GetShapePtr<BaseKnot *>( idSrc ) };
	Shape    * pDst { m_pModel->GetShapePtr<Shape    *>( idDst ) };
	Command  * pCommand;
	if ( pDst->IsPipe() )   // connect baseknot to pipe
		pCommand = new Connect2PipeCommand( pSrc, m_pModel->GetShapePtr<Pipe *>( idDst ) );
	else
		pCommand = new Connect2BaseKnotCommand( pSrc, m_pModel->GetShapePtr<BaseKnot *>( idDst ) );
	m_CmdStack.NewCommand( pCommand );
}

void NNetModelWriterInterface::RemoveShape( ShapeId const id )
{
	class RemoveBaseKnotCommand : public Command
	{
	public:
		RemoveBaseKnotCommand( BaseKnot * const pBaseKnot )
			: m_pBaseKnot( pBaseKnot )
		{ }

		~RemoveBaseKnotCommand( )
		{
			delete m_pBaseKnot;
		}

		virtual void Do( NNetModel * const pModel )
		{
			MicroMeterPoint umPos { m_pBaseKnot->GetPosition() };
			m_pBaseKnot->m_connections.Apply2AllInPipes
			( 
				[&]( Pipe & pipe ) // every incoming Pipe needs a new end knot
				{ 
					Knot * pKnotNew { pModel->NewShape<Knot>( umPos ) };
					pKnotNew->m_connections.AddIncoming( & pipe );
					pipe.SetEndKnot( pKnotNew );
					pipe.DislocateEndPoint( );
					pipe.Recalc();
				} 
			);
			m_pBaseKnot->m_connections.ClearIncoming();
			m_pBaseKnot->m_connections.Apply2AllOutPipes
			( 
				[&]( Pipe & pipe ) // every outgoing Pipe needs a new start knot
				{ 
					Knot * pKnotNew { pModel->NewShape<Knot>( umPos ) };
					pKnotNew->m_connections.AddOutgoing( & pipe );
					pipe.SetStartKnot( pKnotNew );
					pipe.DislocateStartPoint( );
					pipe.Recalc();
				} 
			);
			m_pBaseKnot->m_connections.ClearOutgoing();
			assert( m_pBaseKnot->m_connections.IsOrphan( ) );

			pModel->RemoveFromShapeList( m_pBaseKnot );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			MicroMeterPoint umPos { m_pBaseKnot->GetPosition() };
			m_pBaseKnot->m_connections.Apply2AllInPipes
			( 
				[&]( Pipe & pipe ) // reconnect to original end knot
				{ 
					BaseKnot * pEndPoint { pipe.GetEndKnotPtr() };
					pipe.SetEndKnot( m_pBaseKnot );
					pipe.Recalc();
					delete pEndPoint;
				} 
			);
			m_pBaseKnot->m_connections.Apply2AllOutPipes
			( 
				[&]( Pipe & pipe ) // reconnect to original start knot
				{ 
					BaseKnot * pStartPoint { pipe.GetStartKnotPtr() };
					pipe.SetStartKnot( m_pBaseKnot );
					pipe.Recalc();
					delete pStartPoint;
				} 
			);
			pModel->RestoreToShapeList( m_pBaseKnot );
		}

	private:
		BaseKnot * m_pBaseKnot;
	};

	class RemovePipeCommand : public Command
	{
	public:
		RemovePipeCommand( Pipe * const pPipe )
		  : m_pPipe     ( pPipe ),
			m_pStartKnot( pPipe->GetStartKnotPtr() ),
			m_pEndKnot  ( pPipe->GetEndKnotPtr() )
		{ }

		~RemovePipeCommand( )
		{
			delete m_pStartKnot;                       
			delete m_pEndKnot;
			delete m_pPipe;
		}

		virtual void Do( NNetModel * const pModel )
		{
			m_pStartKnot->m_connections.RemoveOutgoing( m_pPipe );
			if ( m_pStartKnot->IsOrphanedKnot( ) && ( m_pStartKnot->IsKnot() ) )
				pModel->RemoveFromShapeList( m_pStartKnot );

			m_pEndKnot->m_connections.RemoveIncoming( m_pPipe );
			if ( m_pEndKnot->IsOrphanedKnot( ) && ( m_pEndKnot->IsKnot() ) )
				pModel->RemoveFromShapeList( m_pEndKnot );

			pModel->RemoveFromShapeList( m_pPipe );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			m_pStartKnot->m_connections.AddOutgoing( m_pPipe );
			m_pEndKnot  ->m_connections.AddIncoming( m_pPipe );
			pModel->RestoreToShapeList( m_pStartKnot );
			pModel->RestoreToShapeList( m_pEndKnot );
			pModel->RestoreToShapeList( m_pPipe );
		}

	private:
		Pipe     * m_pPipe;
		BaseKnot * m_pStartKnot;
		BaseKnot * m_pEndKnot;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;

	Shape   * pShape { m_pModel->GetShapePtr<Shape *>( id ) };
	Command * pCommand;
	if ( pShape->IsPipe() )   // connect baseknot to pipe
		pCommand = new RemovePipeCommand( static_cast< Pipe * const >( pShape ) );
	else 
		pCommand = new RemoveBaseKnotCommand( static_cast< BaseKnot * const >( pShape ) );
	m_CmdStack.NewCommand( pCommand );
}

void NNetModelWriterInterface::Disconnect( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pModel->Disconnect( id );
}

void NNetModelWriterInterface::Convert2Neuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pModel->Convert2Neuron( id );
}

void NNetModelWriterInterface::Convert2InputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pModel->Convert2InputNeuron( id );
}

void NNetModelWriterInterface::ToggleStopOnTrigger( ShapeId const id )
{
	class ToggleStopOnTriggerCommand : public Command
	{
	public:
		ToggleStopOnTriggerCommand( ShapeId const id )
			:	m_id( id )
		{ }

		virtual void Do( NNetModel * const pModel )
		{
			pModel->ToggleStopOnTrigger( m_id );
		}

		virtual void Undo( NNetModel * const pModel )
		{
			pModel->ToggleStopOnTrigger( m_id );
		}

	private:
		ShapeId const m_id;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pModel->ToggleStopOnTrigger( id );
}

void NNetModelWriterInterface::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	class SetPulseRateCommand : public Command
	{
	public:
		SetPulseRateCommand( ShapeId const id, fHertz const fNewValue )
		  :	m_id( id ),
			m_fHertz( fNewValue )
		{ }

		virtual void Do  ( NNetModel * const pModel ) { m_fHertz = pModel->SetPulseRate( m_id, m_fHertz ); }
		virtual void Undo( NNetModel * const pModel ) {	Do( pModel ); }

	private:
		ShapeId const m_id;
		fHertz        m_fHertz;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << fNewValue << endl;
	m_CmdStack.NewCommand( new SetPulseRateCommand( id, fNewValue ) );
}

void NNetModelWriterInterface::SelectShape( ShapeId const id, tBoolOp const op )
{
	class SelectShapeCommand : public Command
	{
	public:
		SelectShapeCommand( ShapeId const id, tBoolOp const op )
		  :	m_id( id ),
			m_op( op )
		{ }

		virtual void Do  ( NNetModel * const pModel ) { pModel->SelectShape( m_id, m_op ); }
		virtual void Undo( NNetModel * const pModel ) {	pModel->SelectShape( m_id, Reverse( m_op ) ); }

	private:
		ShapeId const m_id;
		tBoolOp const m_op;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << GetBoolOpName( op ) << endl;

	m_CmdStack.NewCommand( new SelectShapeCommand( id, op ) );
}

void NNetModelWriterInterface::SelectAll( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;
	m_pModel->SelectAll( op );
}

void NNetModelWriterInterface::SelectSubtree( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;
	m_pModel->SelectSubtree( id, op );
}

void NNetModelWriterInterface::ResetModel( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModel->ResetModel( );
	m_pModel->CreateInitialShapes();
}

void NNetModelWriterInterface::SetTriggerSound( ShapeId const id, bool const bActive, Hertz const freq, MilliSecs const ms )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << bActive << L" " << freq << L" " << ms << endl;
	m_pModel->SetTriggerSound( id, freq, ms );
	m_pModel->ClearModel( );
}

void NNetModelWriterInterface::SetParameter( tParameter const param, float const fNewValue )
{
	class SetParameterCommand : public Command
	{
	public:
		SetParameterCommand( tParameter const param, float const fNewValue )
		  : m_param( param ),
			m_fValue( fNewValue )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			m_fValue = pModel->SetParameter( m_param, m_fValue ); 
		}

	private:
		tParameter const m_param;
		float            m_fValue;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetParameterName( param ) << L" " << fNewValue << endl;
	m_pModel->SetParameter( param, fNewValue );
}

void NNetModelWriterInterface::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	class MoveShapeCommand : public Command
	{
	public:
		MoveShapeCommand( ShapeId const id, MicroMeterPoint const & delta )
		  : m_id( id ),
			m_delta( -delta )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->MoveShape( m_id, m_delta = -m_delta); 
		}

	private:
		ShapeId   const m_id;
		MicroMeterPoint m_delta;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << delta << endl;
	m_CmdStack.NewCommand( new MoveShapeCommand( id, delta ) );
}

void NNetModelWriterInterface::MoveSelection( MicroMeterPoint const & delta )
{
	class MoveSelectionCommand : public Command
	{
	public:
		MoveSelectionCommand(  MicroMeterPoint const & delta )
			: m_delta( -delta )
		{ }

		virtual void Do( NNetModel * const pModel ) 
		{ 
			pModel->MoveSelection( m_delta = - m_delta); 
		}

	private:
		MicroMeterPoint m_delta;
	};

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << delta << endl;

	m_CmdStack.NewCommand( new MoveSelectionCommand( delta ) );
}

void NNetModelWriterInterface::SelectShapesInRect( MicroMeterRect const & rect )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << rect << endl;
	m_pModel->SelectShapesInRect( rect );
}

wchar_t const * NNetModelWriterInterface::GetActionCommandName( int const iMsgId ) const 
{
	static unordered_map < int, wchar_t const * > mapMsgName =
	{
		{ IDM_ANALYZE_LOOPS,       L"ANALYZE_LOOPS"       },
		{ IDM_ANALYZE_ANOMALIES,   L"ANALYZE_ANOMALIES"   },
		{ IDM_REMOVE_SELECTION,    L"REMOVE_SELECTION"    },
		{ IDM_CLEAR_BEEPERS,       L"CLEAR_BEEPERS"       },
		{ IDM_SELECT_ALL_BEEPERS,  L"SELECT_ALL_BEEPERS"  },
		{ IDM_MARK_SELECTION,      L"MARK_SELECTION"      },
		{ IDM_COPY_SELECTION,      L"COPY_SELECTION"      },
		{ IDD_INSERT_NEURON,       L"INSERT_NEURON"       },
		{ IDD_NEW_NEURON,          L"NEW_NEURON"          },
		{ IDD_NEW_INPUT_NEURON,    L"NEW_INPUT_NEURON"    },
		{ IDD_APPEND_NEURON,       L"APPEND_NEURON"       },
		{ IDD_APPEND_INPUT_NEURON, L"APPEND_INPUT_NEURON" },
		{ IDD_ADD_OUTGOING2KNOT,   L"ADD_OUTGOING2KNOT"   },
		{ IDD_ADD_INCOMING2KNOT,   L"ADD_INCOMING2KNOT"   },
		{ IDD_ADD_OUTGOING2PIPE,   L"ADD_OUTGOING2PIPE"   },
		{ IDD_ADD_INCOMING2PIPE,   L"ADD_INCOMING2PIPE"   }
	};

	return mapMsgName.at( iMsgId );
}

int const NNetModelWriterInterface::GetActionCommandFromName( wchar_t const * const name ) const 
{
	static unordered_map < wchar_t const *, int > mapMsg =
	{
		{ L"ANALYZE_LOOPS",       IDM_ANALYZE_LOOPS       },
		{ L"ANALYZE_ANOMALIES",   IDM_ANALYZE_ANOMALIES   },
		{ L"REMOVE_SELECTION",    IDM_REMOVE_SELECTION    },
		{ L"CLEAR_BEEPERS",       IDM_CLEAR_BEEPERS       },
		{ L"SELECT_ALL_BEEPERS",  IDM_SELECT_ALL_BEEPERS  },
		{ L"MARK_SELECTION",      IDM_MARK_SELECTION      },
		{ L"COPY_SELECTION",      IDM_COPY_SELECTION      },
		{ L"INSERT_NEURON",       IDD_INSERT_NEURON       },
		{ L"NEW_NEURON",          IDD_NEW_NEURON          },
		{ L"NEW_INPUT_NEURON",    IDD_NEW_INPUT_NEURON    },
		{ L"APPEND_NEURON",       IDD_APPEND_NEURON       },
		{ L"APPEND_INPUT_NEURON", IDD_APPEND_INPUT_NEURON },
		{ L"ADD_OUTGOING2KNOT",   IDD_ADD_OUTGOING2KNOT   },
		{ L"ADD_INCOMING2KNOT",   IDD_ADD_INCOMING2KNOT   },
		{ L"ADD_OUTGOING2PIPE",   IDD_ADD_OUTGOING2PIPE   },
		{ L"ADD_INCOMING2PIPE",   IDD_ADD_INCOMING2PIPE   }
	};

	return mapMsg.at( name );
}

class ActionCommand : public Command
{
public:
	ActionCommand( int const idMsg, ShapeId const id, MicroMeterPoint const & pos )
	  : m_idMsg( idMsg ),
		m_id   ( id ),
		m_pos  ( pos )
	{}

	virtual void Do( NNetModel * const pModel )
	{
		switch ( m_idMsg )
		{
		case IDM_ANALYZE_LOOPS:
		case IDM_ANALYZE_ANOMALIES:
		{
			pModel->SelectAll( tBoolOp::opFalse );
			auto func { (m_idMsg == IDM_ANALYZE_LOOPS) ? ModelAnalyzer::FindLoop : ModelAnalyzer::FindAnomaly };
			bool bFound { func( * pModel ) };
			if ( bFound )
				ModelAnalyzer::SelectLoopShapes( * pModel );
		}
		break;

		case IDD_NEW_NEURON:
			pModel->NewShape<Neuron>( m_pos );
			break;

		case IDD_NEW_INPUT_NEURON:
			pModel->NewShape<InputNeuron>( m_pos );
			break;

		case IDD_APPEND_NEURON:
			pModel->AppendShape<Neuron>( m_id );
			break;

		case IDD_APPEND_INPUT_NEURON:
			pModel->AppendShape<InputNeuron>( m_id );
			break;

		case IDD_INSERT_NEURON:
			pModel->InsertNeuron( m_id, m_pos );
			break;

		case IDD_ADD_OUTGOING2KNOT:
			pModel->AddOutgoing2Knot( m_id, m_pos );
			break;

		case IDD_ADD_INCOMING2KNOT:
			pModel->AddIncoming2Knot( m_id, m_pos );
			break;

		case IDD_ADD_OUTGOING2PIPE:
			pModel->AddOutgoing2Pipe( m_id, m_pos );
			break;

		case IDD_ADD_INCOMING2PIPE:
			pModel->AddIncoming2Pipe( m_id, m_pos );
			break;

		default:
			break;
		} 
	}

	virtual void Undo( NNetModel * const pModel )
	{
		switch ( m_idMsg )
		{
		case IDM_ANALYZE_LOOPS:
		case IDM_ANALYZE_ANOMALIES:
		{
			//pModel->SelectAll( tBoolOp::opFalse );
			//auto func { (m_idMsg == IDM_ANALYZE_LOOPS) ? ModelAnalyzer::FindLoop : ModelAnalyzer::FindAnomaly };
			//bool bFound { func( * pModel ) };
			//if ( bFound )
			//	ModelAnalyzer::SelectLoopShapes( * pModel );
		}
		break;

		case IDD_NEW_NEURON:
		case IDD_NEW_INPUT_NEURON:
//			pModel->RemoveShape( pModel->GetShape( m_id ) ); 
			break;

		case IDD_APPEND_NEURON:
//			pModel->AppendShape<Neuron>( m_id );
			break;

		case IDD_APPEND_INPUT_NEURON:
//			pModel->AppendShape<InputNeuron>( m_id );
			break;

		case IDD_INSERT_NEURON:
//			pModel->InsertNeuron( m_id, m_pos );
			break;

		case IDD_ADD_OUTGOING2KNOT:
//			pModel->AddOutgoing2Knot( m_id, m_pos );
			break;

		case IDD_ADD_INCOMING2KNOT:
//			pModel->AddIncoming2Knot( m_id, m_pos );
			break;

		case IDD_ADD_OUTGOING2PIPE:
//			pModel->AddOutgoing2Pipe( m_id, m_pos );
			break;

		case IDD_ADD_INCOMING2PIPE:
//			pModel->AddIncoming2Pipe( m_id, m_pos );
			break;

		default:
			break;
		} 
	}

private:
	int             const m_idMsg;
	ShapeId         const m_id;
	MicroMeterPoint const m_pos;
	Shape               * m_pShape { nullptr };
};

void NNetModelWriterInterface::Action( int const idMsg, ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetActionCommandName( idMsg ) << L" " << id.GetValue( ) << pos << endl;

	Command * pCommand { new ActionCommand( idMsg, id, pos ) };
	pCommand->Do( m_pModel );
	delete pCommand;
}

void NNetModelWriterInterface::DeleteSelection()
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModel->DeleteSelection();
}

void NNetModelWriterInterface::SelectAllBeepers( )
{
	m_pModel->SelectBeepers();
}

void NNetModelWriterInterface::ClearBeepers( )
{
	m_pModel->ClearBeepers( );
}

void NNetModelWriterInterface::MarkSelection( tBoolOp const op )
{
	m_pModel->MarkSelection( op );
}

void NNetModelWriterInterface::CopySelection( )
{
	m_pModel->CopySelection();
}
