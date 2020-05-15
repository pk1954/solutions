// win32_WorkThreadInterface.cpp
//
// NNetWindows

#include "stdafx.h"
#include "assert.h"
#include <unordered_map>
#include "Resource.h"
#include "PixelTypes.h"
#include "win32_util.h"
#include "win32_script.h"
#include "win32_WorkThread.h"
#include "win32_WorkThreadInterface.h"

using std::wostream;
using std::wcout;
using std::endl;
using std::unordered_map;

WorkThreadInterface::WorkThreadInterface( ) :
	m_pTraceStream   ( nullptr ),
	m_bTrace         ( true ),
	m_pNNetWorkThread( nullptr )
{ }

WorkThreadInterface::~WorkThreadInterface( )
{
	m_pNNetWorkThread = nullptr;
	m_pTraceStream = nullptr;
}

void WorkThreadInterface::Initialize( wostream * pTraceStream ) 
{ 
	m_pTraceStream = pTraceStream;
}

void WorkThreadInterface::Start
( 
	HWND                 const hwndApplication,
	ActionTimer        * const pActionTimer,
    EventInterface     * const pEvent,
	ObserverInterface  * const pRedrawObserver,
	ObserverInterface  * const pChangeObserver,
	SlowMotionRatio    * const pSlowMotionRatio,
	NNetModel          * const pModel,
	Param              * const pParam,
	NNetModelStorage   * const pStorage,
	bool                 const bAsync
)
{
	m_pNNetWorkThread = new NNetWorkThread
	( 
		hwndApplication, 
		pActionTimer,
		pEvent, 
		pRedrawObserver,
		pChangeObserver,
		pSlowMotionRatio,
		this,
		pModel,
		pParam,
		pStorage,
		bAsync
	);
}

void WorkThreadInterface::Stop( )
{
	m_pNNetWorkThread->Terminate( );
	delete m_pNNetWorkThread;
	m_pNNetWorkThread = nullptr;
}

void WorkThreadInterface::postMsg( NNetWorkThreadMessage::Id msg, WPARAM const wParam, LPARAM const lParam )
{
	m_pNNetWorkThread->PostThreadMsg( static_cast<UINT>( msg ), wParam, lParam );
}

void WorkThreadInterface::PostResetTimer( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	postMsg( NNetWorkThreadMessage::Id::RESET_TIMER );
}

void WorkThreadInterface::PostConnect( ShapeId const idSrc, ShapeId const idDst )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << idSrc.GetValue() << L" " << idDst.GetValue() << endl;
	postMsg( NNetWorkThreadMessage::Id::CONNECT, idSrc.GetValue(), idDst.GetValue() );
}

void WorkThreadInterface::PostRemoveShape( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	postMsg( NNetWorkThreadMessage::Id::REMOVE_SHAPE, id.GetValue() );
}

void WorkThreadInterface::PostDisconnect( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	postMsg( NNetWorkThreadMessage::Id::DISCONNECT, id.GetValue() );
}

void WorkThreadInterface::PostConvert2Neuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	postMsg( NNetWorkThreadMessage::Id::CONVERT2NEURON, id.GetValue() );
}

void WorkThreadInterface::PostConvert2InputNeuron( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	postMsg( NNetWorkThreadMessage::Id::CONVERT2INPUT_NEURON, id.GetValue() );
}

void WorkThreadInterface::PostToggleStopOnTrigger( ShapeId const id )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << endl;
	postMsg( NNetWorkThreadMessage::Id::TOGGLE_STOP_ON_TRIGGER, id.GetValue() );
}

void WorkThreadInterface::PostSelectShape( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << GetBoolOpName( op )  << endl;
	postMsg( NNetWorkThreadMessage::Id::SELECT_SHAPE, id.GetValue(), static_cast<LPARAM>(op) );
}

void WorkThreadInterface::PostSelectAll( tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;
	postMsg( NNetWorkThreadMessage::Id::SELECT_ALL, 0, static_cast<LPARAM>(op) );
}

void WorkThreadInterface::PostSelectSubtree( ShapeId const id, tBoolOp const op )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetBoolOpName( op ) << endl;
	postMsg( NNetWorkThreadMessage::Id::SELECT_SUBTREE, id.GetValue(), static_cast<LPARAM>(op) );
}

void WorkThreadInterface::PostResetModel( )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << endl;
	postMsg( NNetWorkThreadMessage::Id::RESET_MODEL );
}

void WorkThreadInterface::PostSetPulseRate( ShapeId const id, fHertz const fNewValue )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << fNewValue << endl;
	postMsg( NNetWorkThreadMessage::Id::PULSE_RATE, id.GetValue(), (LPARAM &)fNewValue );
}

void WorkThreadInterface::PostSetTriggerSound( ShapeId const id, bool const bActive, Hertz const freq, MilliSecs const ms )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << bActive << L" " << freq << L" " << ms << endl;
	postMsg( NNetWorkThreadMessage::Id::TRIGGER_SOUND, id.GetValue(), Util::Pack2UINT64( freq.GetValue(), ms.GetValue() ) );
}

void WorkThreadInterface::PostSetParameter( tParameter const param, float const fNewValue )
{
	static unordered_map < tParameter, NNetWorkThreadMessage::Id const > mapParam =
	{
		{ tParameter::pulseSpeed,     NNetWorkThreadMessage::Id::PULSE_SPEED       },
		{ tParameter::pulseWidth,     NNetWorkThreadMessage::Id::PULSE_WIDTH       },
		{ tParameter::threshold,      NNetWorkThreadMessage::Id::THRESHOLD         },
		{ tParameter::peakVoltage,    NNetWorkThreadMessage::Id::PEAK_VOLTAGE      },
    	{ tParameter::refractPeriod,  NNetWorkThreadMessage::Id::REFRACTORY_PERIOD },
	    { tParameter::timeResolution, NNetWorkThreadMessage::Id::TIME_RESOLUTION   }
	};				  

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetParameterName( param ) << L" " << fNewValue << endl;
	postMsg( mapParam.at( param ), 0, (LPARAM &)fNewValue );
}

void WorkThreadInterface::PostMoveShape( ShapeId const id, MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << id.GetValue() << L" " << delta << endl;
	postMsg( NNetWorkThreadMessage::Id::MOVE_SHAPE, id.GetValue(), Util::Pack2UINT64(delta) );
}

void WorkThreadInterface::PostMoveSelection( MicroMeterPoint const & delta )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << delta << endl;
	postMsg( NNetWorkThreadMessage::Id::MOVE_SELECTION, 0, Util::Pack2UINT64(delta) );
}

void WorkThreadInterface::PostSelectShapesInRect( MicroMeterRect const & rect )
{
	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << L" " << rect << endl;
	m_pNNetWorkThread->PostThreadMsg
	( 
		static_cast<UINT>( NNetWorkThreadMessage::Id::SELECT_SHAPES_IN_RECT ), 
		Util::Pack2UINT64( rect.GetStartPoint() ), 
		Util::Pack2UINT64( rect.GetEndPoint() ) 
	);
}

void WorkThreadInterface::PostSlowMotionChanged( )
{
	postMsg( NNetWorkThreadMessage::Id::SLOW_MOTION_CHANGED );
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

void WorkThreadInterface::PostActionCommand( int const idMsg, ShapeId const idShape, MicroMeterPoint const & umPos )
{
	static unordered_map < int, NNetWorkThreadMessage::Id const > mapMsg =
	{
		{ IDM_ANALYZE_LOOPS,       NNetWorkThreadMessage::Id::ANALYZE_LOOPS       },
		{ IDM_ANALYZE_ANOMALIES,   NNetWorkThreadMessage::Id::ANALYZE_ANOMALIES   },
		{ IDM_REMOVE_SELECTION,    NNetWorkThreadMessage::Id::REMOVE_SELECTION    },
		{ IDM_REMOVE_BEEPERS,      NNetWorkThreadMessage::Id::REMOVE_BEEPERS      },
		{ IDM_SELECT_ALL_BEEPERS,  NNetWorkThreadMessage::Id::SELECT_ALL_BEEPERS  },
		{ IDM_MARK_SELECTION,      NNetWorkThreadMessage::Id::MARK_SELECTION      },
		{ IDM_COPY_SELECTION,      NNetWorkThreadMessage::Id::COPY_SELECTION      },
		{ IDD_INSERT_NEURON,       NNetWorkThreadMessage::Id::INSERT_NEURON       },
		{ IDD_NEW_NEURON,          NNetWorkThreadMessage::Id::NEW_NEURON          },
		{ IDD_NEW_INPUT_NEURON,    NNetWorkThreadMessage::Id::NEW_INPUT_NEURON    },
		{ IDD_APPEND_NEURON,       NNetWorkThreadMessage::Id::APPEND_NEURON       },
		{ IDD_APPEND_INPUT_NEURON, NNetWorkThreadMessage::Id::APPEND_INPUT_NEURON },
		{ IDD_ADD_OUTGOING2KNOT,   NNetWorkThreadMessage::Id::ADD_OUTGOING2KNOT   },
		{ IDD_ADD_INCOMING2KNOT,   NNetWorkThreadMessage::Id::ADD_INCOMING2KNOT   },
		{ IDD_ADD_OUTGOING2PIPE,   NNetWorkThreadMessage::Id::ADD_OUTGOING2PIPE   },
		{ IDD_ADD_INCOMING2PIPE,   NNetWorkThreadMessage::Id::ADD_INCOMING2PIPE   }
	};				  

	if ( IsTraceOn( ) )
		TraceStream( ) << __func__ << L" " << GetActionCommandName( idMsg ) << L" " << idShape.GetValue( ) << umPos << endl;
	postMsg( mapMsg.at( idMsg ), idShape.GetValue( ), Util::Pack2UINT64( umPos ) );
}

void WorkThreadInterface::PostGenerationStep( )
{
	m_pNNetWorkThread->Continue( );     // trigger worker thread if waiting on POI event
	postMsg( NNetWorkThreadMessage::Id::NEXT_GENERATION );
}

void WorkThreadInterface::PostRunGenerations( bool const bFirst )
{
	postMsg( NNetWorkThreadMessage::Id::GENERATION_RUN, 0, bFirst );
}

void WorkThreadInterface::PostRepeatGenerationStep( )
{
	postMsg( NNetWorkThreadMessage::Id::REPEAT_NEXT_GENERATION );
}

void WorkThreadInterface::PostStopComputation( )
{
	postMsg( NNetWorkThreadMessage::Id::STOP );
}

void WorkThreadInterface::PostSendBack( int const iMsg )
{
	postMsg( NNetWorkThreadMessage::Id::SEND_BACK, iMsg );
}

void WorkThreadInterface::PostDeleteSelection()
{
	postMsg( NNetWorkThreadMessage::Id::REMOVE_SELECTION );
}

void WorkThreadInterface::PostSelectAllBeepers( )
{
	postMsg( NNetWorkThreadMessage::Id::SELECT_ALL_BEEPERS );
}

void WorkThreadInterface::PostRemoveBeepers( )
{
	postMsg( NNetWorkThreadMessage::Id::REMOVE_BEEPERS );
}

void WorkThreadInterface::PostMarkSelection( tBoolOp const op )
{
	postMsg( NNetWorkThreadMessage::Id::MARK_SELECTION, 0, static_cast<LPARAM>(op) );
}

void WorkThreadInterface::PostCopySelection( )
{
	postMsg( NNetWorkThreadMessage::Id::COPY_SELECTION );
}
