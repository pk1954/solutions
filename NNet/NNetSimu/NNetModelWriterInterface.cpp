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

class ResetTimerCommand : public Command
{
public:
	virtual void Do  ( NNetModel * const pModel ) { m_simuTime = pModel->SetSimulationTime(); }
	virtual void Undo( NNetModel * const pModel ) { pModel->SetSimulationTime( m_simuTime ); }

private:
	fMicroSecs m_simuTime {};
};

void NNetModelWriterInterface::ResetTimer( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_CmdStack.NewCommand( new ResetTimerCommand() );
}

class Connect2PipeCommand : public Command
{
public:
	Connect2PipeCommand( ShapeId idSrc, ShapeId idDst )
	:	m_idBaseKnot( idSrc ),
		m_idPipe    ( idDst )
	{ }

	virtual void Do( NNetModel * const pModel )
	{
		BaseKnot * const pBaseKnot2Insert { pModel->GetShapePtr<BaseKnot *>( m_idBaseKnot ) };
		Pipe     * const pPipe2Split      { pModel->GetShapePtr<Pipe     *>( m_idPipe     ) };
		BaseKnot * const pStartKnot { pPipe2Split->GetStartKnotPtr( ) };

		m_idNewPipe = pModel->NewPipe( pStartKnot, pBaseKnot2Insert );
		pStartKnot->RemoveOutgoing( pPipe2Split );
		ConnectOutgoing( pPipe2Split, pBaseKnot2Insert );
	}

	virtual void Undo( NNetModel * const pModel )
	{
		BaseKnot      * const pBaseKnot2Disconnect { pModel->GetShapePtr<BaseKnot *>( m_idBaseKnot ) };
		Pipe          * const pPipe2Remove         { pModel->GetShapePtr<Pipe     *>( m_idNewPipe  ) };
		Pipe          * const pPipe2Survive        { pModel->GetShapePtr<Pipe     *>( m_idPipe  ) };
		MicroMeterPoint const umStartPos           { pPipe2Remove->GetStartKnotPtr( )->GetPosition() };

		pModel->RemoveShape( pBaseKnot2Disconnect );
		pModel->RemoveShape( pPipe2Remove );
		pPipe2Survive->GetStartKnotPtr()->SetPosition( umStartPos );
	}

private:
	ShapeId m_idBaseKnot { NO_SHAPE };
	ShapeId m_idPipe     { NO_SHAPE };
	ShapeId m_idNewPipe  { NO_SHAPE };
};

class Connect2BaseKnotCommand : public Command
{
public:
	Connect2BaseKnotCommand( BaseKnot * pSrc, BaseKnot * pDst )
	  :	m_pBaseKnotSrc( pSrc ),
		m_pBaseKnotDst( pDst )
	{ }

	virtual void Do( NNetModel * const pModel )
	{
		if ( m_pBaseKnotSrc && m_pBaseKnotDst )
		{
			m_pBaseKnotSrc->Apply2AllInPipes ( [&]( Pipe & pipe ) { ConnectIncoming( & pipe, m_pBaseKnotDst ); } );
			m_pBaseKnotSrc->Apply2AllOutPipes( [&]( Pipe & pipe ) { ConnectOutgoing( & pipe, m_pBaseKnotDst ); } );
			pModel->RemoveFromShapeList( m_pBaseKnotSrc );
		}
	}

	virtual void Undo( NNetModel * const pModel )
	{
		pModel->RestoreToShapeList( m_pBaseKnotSrc );
		m_pBaseKnotSrc->Apply2AllInPipes ( [&]( Pipe & pipe ) {	ReplaceEndKnot  ( & pipe, m_pBaseKnotSrc ); } );
		m_pBaseKnotSrc->Apply2AllOutPipes( [&]( Pipe & pipe ) { ReplaceStartKnot( & pipe, m_pBaseKnotSrc ); } );
	}

private:
	BaseKnot * m_pBaseKnotSrc { nullptr };
	BaseKnot * m_pBaseKnotDst { nullptr };
};

void NNetModelWriterInterface::Connect( ShapeId const idSrc, ShapeId const idDst )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << idSrc.GetValue() << L" " << idDst.GetValue() << endl;
	BaseKnot * pSrc { m_pModel->GetShapePtr<BaseKnot *>( idSrc ) };
	Shape    * pDst { m_pModel->GetShapePtr<Shape    *>( idDst ) };
	Command  * pCommand;
	if ( pDst->IsPipe() )   // connect baseknot to pipe
		pCommand = new Connect2PipeCommand    ( idSrc, idDst );
	else
		pCommand = new Connect2BaseKnotCommand( pSrc, m_pModel->GetShapePtr<BaseKnot *>( idDst ) );
	m_CmdStack.NewCommand( pCommand );
}

class RemoveShapeCommand : public Command
{
public:
	RemoveShapeCommand( ShapeId const id )
		:	m_id( id )
	{ }

	virtual void Do( NNetModel * const pModel )
	{
		pModel->RemoveShape( m_id );
	}

	virtual void Undo( NNetModel * const pModel )
	{
		// TODO  xxxxxxxxxx
	}

private:
	ShapeId const m_id;
};

void NNetModelWriterInterface::RemoveShape( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pModel->RemoveShape( id );
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

void NNetModelWriterInterface::ToggleStopOnTrigger( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pModel->ToggleStopOnTrigger( id );
}

void NNetModelWriterInterface::SelectShape( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << GetBoolOpName( op )  << endl;
	m_pModel->SelectShape( id, op );
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

void NNetModelWriterInterface::SetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << fNewValue << endl;
	m_pModel->SetPulseRate( id, fNewValue ); 
	m_pModel->ClearModel( );
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
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetParameterName( param ) << L" " << fNewValue << endl;
	m_pModel->SetParameter( param, fNewValue );
}

class MoveShapeCommand : public Command
{
public:
	MoveShapeCommand( ShapeId const id, MicroMeterPoint const & delta )
		:	m_id( id ),
		    m_delta( delta )
	{ }

	virtual void Do( NNetModel * const pModel )
	{
		pModel->MoveShape( m_id, m_delta );
	}

	virtual void Undo( NNetModel * const pModel )
	{
		pModel->MoveShape( m_id, - m_delta );
	}

private:
	ShapeId         const m_id;
	MicroMeterPoint const m_delta;
};

void NNetModelWriterInterface::MoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << delta << endl;
	m_pModel->MoveShape( id, delta );
}

void NNetModelWriterInterface::MoveSelection( MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << delta << endl;
	m_pModel->MoveSelection( delta );
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
		{ IDM_REMOVE_BEEPERS,      L"REMOVE_BEEPERS"      },
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
		{ L"REMOVE_BEEPERS",      IDM_REMOVE_BEEPERS      },
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
			pModel->RemoveShape( m_id );
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

void NNetModelWriterInterface::RemoveBeepers( )
{
	m_pModel->RemoveBeepers( );
}

void NNetModelWriterInterface::MarkSelection( tBoolOp const op )
{
	m_pModel->MarkSelection( op );
}

void NNetModelWriterInterface::CopySelection( )
{
	m_pModel->CopySelection();
}
