// win32_WorkThreadInterface.cpp
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
#include "win32_WorkThreadInterface.h"

using std::wostream;
using std::wcout;
using std::endl;
using std::unordered_map;

WorkThreadInterface::WorkThreadInterface( ) :
	m_pTraceStream   ( nullptr ),
	m_bTrace         ( true )
{ }

WorkThreadInterface::~WorkThreadInterface( )
{
	m_pTraceStream = nullptr;
}

void WorkThreadInterface::Initialize( wostream * pTraceStream ) 
{ 
	m_pTraceStream = pTraceStream;
}

void WorkThreadInterface::Start
( 
	HWND                const hwndApplication,
	ActionTimer       * const pActionTimer,
    EventInterface    * const pEvent,
	SlowMotionRatio   * const pSlowMotionRatio,
	NNetModel         * const pModel,
	Param             * const pParam,
	NNetModelStorage  * const pStorage
)
{
	m_pNNetModel = pModel;
	m_pParam     = pParam;
}

void WorkThreadInterface::Stop( )
{
	m_pNNetModel = nullptr;
}

void WorkThreadInterface::PostResetTimer( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pNNetModel->ResetSimulationTime( );
}

void WorkThreadInterface::PostConnect( ShapeId const idSrc, ShapeId const idDst )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << idSrc.GetValue() << L" " << idDst.GetValue() << endl;
	m_pNNetModel->Connect( idSrc, idDst );
}

void WorkThreadInterface::PostRemoveShape( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pNNetModel->RemoveShape( id );
}

void WorkThreadInterface::PostDisconnect( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pNNetModel->Disconnect( id );
}

void WorkThreadInterface::PostConvert2Neuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pNNetModel->Convert2Neuron( id );
}

void WorkThreadInterface::PostConvert2InputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pNNetModel->Convert2InputNeuron( id );
}

void WorkThreadInterface::PostToggleStopOnTrigger( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	m_pNNetModel->ToggleStopOnTrigger( id );
}

void WorkThreadInterface::PostSelectShape( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << GetBoolOpName( op )  << endl;
	m_pNNetModel->SelectShape( id, op );
}

void WorkThreadInterface::PostSelectAll( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;
	m_pNNetModel->SelectAll( op );
}

void WorkThreadInterface::PostSelectSubtree( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;
	m_pNNetModel->SelectSubtree( id, op );
}

void WorkThreadInterface::PostResetModel( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pNNetModel->ResetModel( );
	m_pNNetModel->CreateInitialShapes();
}

void WorkThreadInterface::PostSetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << fNewValue << endl;
	m_pNNetModel->SetPulseRate_Lock( id, fNewValue ); 
	m_pNNetModel->ClearModel( );
}

void WorkThreadInterface::PostSetTriggerSound( ShapeId const id, bool const bActive, Hertz const freq, MilliSecs const ms )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << bActive << L" " << freq << L" " << ms << endl;
	m_pNNetModel->SetTriggerSound( id, freq, ms );
	m_pNNetModel->ClearModel( );
}

void WorkThreadInterface::PostSetParameter( tParameter const param, float const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetParameterName( param ) << L" " << fNewValue << endl;
	m_pNNetModel->SetParameter( param, fNewValue );
}

void WorkThreadInterface::PostMoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << delta << endl;
	m_pNNetModel->MoveShape( id, delta );
}

void WorkThreadInterface::PostMoveSelection( MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << delta << endl;
	m_pNNetModel->MoveSelection( delta );
}

void WorkThreadInterface::PostSelectShapesInRect( MicroMeterRect const & rect )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << rect << endl;
	m_pNNetModel->SelectShapesInRect( rect );
}

wchar_t const * WorkThreadInterface::GetActionCommandName( int const iMsgId ) const 
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

int const WorkThreadInterface::GetActionCommandFromName( wchar_t const * const name ) const 
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

void WorkThreadInterface::PostActionCommand( int const idMsg, ShapeId const id, MicroMeterPoint const & pos )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetActionCommandName( idMsg ) << L" " << id.GetValue( ) << pos << endl;
	switch ( idMsg )
	{
	case IDM_ANALYZE_LOOPS:
	case IDM_ANALYZE_ANOMALIES:
	{
		m_pNNetModel->SelectAll( tBoolOp::opFalse );
		auto func { (idMsg == IDM_ANALYZE_LOOPS) ? ModelAnalyzer::FindLoop : ModelAnalyzer::FindAnomaly };
		bool bFound { func( * m_pNNetModel ) };
		if ( bFound )
			ModelAnalyzer::SelectLoopShapes( * m_pNNetModel );
		m_pParam->SetEmphasizeMode( bFound );
	}
	break;

	case IDD_NEW_NEURON:
		m_pNNetModel->NewShape<Neuron>( pos );
		break;

	case IDD_NEW_INPUT_NEURON:
		m_pNNetModel->NewShape<InputNeuron>( pos );
		break;

	case IDD_APPEND_NEURON:
		m_pNNetModel->AppendShape<Neuron>( id );
		break;

	case IDD_APPEND_INPUT_NEURON:
		m_pNNetModel->AppendShape<InputNeuron>( id );
		break;

	case IDD_INSERT_NEURON:
		m_pNNetModel->InsertNeuron( id, pos );
		break;

	case IDD_ADD_OUTGOING2KNOT:
		m_pNNetModel->AddOutgoing2Knot( id, pos );
		break;

	case IDD_ADD_INCOMING2KNOT:
		m_pNNetModel->AddIncoming2Knot( id, pos );
		break;

	case IDD_ADD_OUTGOING2PIPE:
		m_pNNetModel->AddOutgoing2Pipe( id, pos );
		break;

	case IDD_ADD_INCOMING2PIPE:
		m_pNNetModel->AddIncoming2Pipe( id, pos );
		break;

	default:
		break;
	} 
}

void WorkThreadInterface::PostDeleteSelection()
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	m_pNNetModel->DeleteSelection();
}

void WorkThreadInterface::PostSelectAllBeepers( )
{
	m_pNNetModel->SelectBeepers();
}

void WorkThreadInterface::PostRemoveBeepers( )
{
	m_pNNetModel->RemoveBeepers( );
}

void WorkThreadInterface::PostMarkSelection( tBoolOp const op )
{
	m_pNNetModel->MarkSelection( op );
}

void WorkThreadInterface::PostCopySelection( )
{
	m_pNNetModel->CopySelection();
}
