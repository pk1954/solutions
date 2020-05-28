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
#include "NNetModelWriterInterface.h"

using std::wostream;
using std::wcout;
using std::endl;
using std::unordered_map;

void NNetModelWriterInterface::Initialize( wostream * pTraceStream ) 
{ 
	m_pTraceStream = pTraceStream;
}

void NNetModelWriterInterface::Start( NNetModel * const pModel, Param * const pParam )
{
	m_pModel = pModel;
	m_pParam = pParam;
}

void NNetModelWriterInterface::Stop( )
{
	m_pModel = nullptr;
}

void NNetModelWriterInterface::ResetTimer( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pModel->ResetSimulationTime( );
}

void NNetModelWriterInterface::Connect( ShapeId const idSrc, ShapeId const idDst )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << idSrc.GetValue() << L" " << idDst.GetValue() << endl;
	m_pModel->Connect( idSrc, idDst );
}

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

void NNetModelWriterInterface::ActionCommand( int const idMsg, ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetActionCommandName( idMsg ) << L" " << id.GetValue( ) << pos << endl;
	switch ( idMsg )
	{
	case IDM_ANALYZE_LOOPS:
	case IDM_ANALYZE_ANOMALIES:
	{
		m_pModel->SelectAll( tBoolOp::opFalse );
		auto func { (idMsg == IDM_ANALYZE_LOOPS) ? ModelAnalyzer::FindLoop : ModelAnalyzer::FindAnomaly };
		bool bFound { func( * m_pModel ) };
		if ( bFound )
			ModelAnalyzer::SelectLoopShapes( * m_pModel );
		m_pParam->SetEmphasizeMode( bFound );
	}
	break;

	case IDD_NEW_NEURON:
		m_pModel->NewShape<Neuron>( pos );
		break;

	case IDD_NEW_INPUT_NEURON:
		m_pModel->NewShape<InputNeuron>( pos );
		break;

	case IDD_APPEND_NEURON:
		m_pModel->AppendShape<Neuron>( id );
		break;

	case IDD_APPEND_INPUT_NEURON:
		m_pModel->AppendShape<InputNeuron>( id );
		break;

	case IDD_INSERT_NEURON:
		m_pModel->InsertNeuron( id, pos );
		break;

	case IDD_ADD_OUTGOING2KNOT:
		m_pModel->AddOutgoing2Knot( id, pos );
		break;

	case IDD_ADD_INCOMING2KNOT:
		m_pModel->AddIncoming2Knot( id, pos );
		break;

	case IDD_ADD_OUTGOING2PIPE:
		m_pModel->AddOutgoing2Pipe( id, pos );
		break;

	case IDD_ADD_INCOMING2PIPE:
		m_pModel->AddIncoming2Pipe( id, pos );
		break;

	default:
		break;
	} 
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
