// win32_MainWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "MoreTypes.h"
#include "Resource.h"
#include "SumSignal.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelCommands.h"
#include "win32_MonitorWindow.h"
#include "win32_MainWindow.h"

void MainWindow::Start
(
	HWND                       const hwndApp, 
	DWORD                      const dwStyle,
	bool                       const bShowRefreshRateDialog,
	NNetController           * const pController,
	NNetModelReaderInterface * const pModelReaderInterface,
	NNetModelCommands        * const pNNetCommands,
	Observable               * const pCursorObservable,
	Observable               * const pCoordObservable,
	BeaconAnimation          * const pBeaconAnimation,
	MonitorData              * const pMonitorData
)
{
	NNetWindow::Start
	( 
		hwndApp, 
		dwStyle,
		bShowRefreshRateDialog,
		pController,
		pModelReaderInterface,
		pBeaconAnimation,
		pMonitorData
	);
	ShowRefreshRateDlg( bShowRefreshRateDialog );
	m_pNNetCommands        = pNNetCommands;
	m_pCursorPosObservable = pCursorObservable;
	m_pCoordObservable     = pCoordObservable;
}

void MainWindow::Stop( )
{
	Reset( );
	NNetWindow::Stop();
}

void MainWindow::Reset( )
{ 
	m_shapeHighlighted      = NO_SHAPE; 
	m_shapeSuperHighlighted = NO_SHAPE; 
}

long MainWindow::AddContextMenuEntries( HMENU const hPopupMenu )
{
	if ( m_pMRI->AnyShapesSelected( ) )
	{
		AppendMenu( hPopupMenu, MF_STRING, IDM_DESELECT_ALL,     L"Deselect all" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_COPY_SELECTION,   L"Copy selection" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_DELETE_SELECTION, L"Delete selected objects" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_CLEAR_BEEPERS,    L"Clear selected trigger sounds" );
	}
	else if ( IsUndefined( m_shapeHighlighted ) )  // no shape highlighted, cursor on background
	{
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_NEURON,         L"New neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_INPUT_NEURON,   L"New input neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_SENSOR,         L"New EEG sensor" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_SELECT_ALL_BEEPERS, L"Select all neurons with trigger sound" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_CLEAR_BEEPERS,      L"Clear all trigger sounds" );
	}
	else switch ( m_pMRI->GetShapeType( m_shapeHighlighted ).GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		if ( ! m_pMRI->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_PULSE_RATE,            L"Pulse rate" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SHAPE,          L"Delete" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DISCONNECT,            L"Disconnect" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_SINGLE_SIGNAL,     L"Monitor" );
		break;

	case ShapeType::Value::neuron:
		if ( ! m_pMRI->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_INCOMING2KNOT,     L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SHAPE,          L"Delete" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DISCONNECT,            L"Disconnect" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_TRIGGER_SOUND_DLG,     L"Trigger sound" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_SELECT_SUBTREE,        L"Select subtree" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_STOP_ON_TRIGGER,       L"Stop on trigger on/off" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_SINGLE_SIGNAL,     L"Monitor" );
		break;

	case ShapeType::Value::knot:  
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_INCOMING2KNOT, L"Add incoming dendrite" );
		if ( 
			(! m_pMRI->HasOutgoing( m_shapeHighlighted )) || 
			(
				! m_pMRI->HasIncoming( m_shapeHighlighted ) && 
				( m_pMRI->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 )  
				) 
			)
			AppendMenu( hPopupMenu, MF_STRING, IDD_APPEND_NEURON, L"Add neuron" );
		if ( 
			( m_pMRI->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 ) && 
			(! m_pMRI->HasIncoming( m_shapeHighlighted )) 
			)
			AppendMenu( hPopupMenu, MF_STRING, IDD_APPEND_INPUT_NEURON, L"Add input neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DISCONNECT, L"Disconnect" );
		break;

	case ShapeType::Value::pipe:
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2PIPE, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_INCOMING2PIPE, L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_INSERT_NEURON,     L"Insert neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SHAPE,      L"Delete" );
		if ( Pipe::GetArrowSize( ) > 0.0_MicroMeter )
			AppendMenu( hPopupMenu, MF_STRING, IDD_ARROWS_OFF,    L"Arrows off" );
		else
			AppendMenu( hPopupMenu, MF_STRING, IDD_ARROWS_ON,     L"Arrows on" );
		break;

	default:
		assert( false );
	}

	if ( IsDefined( m_shapeHighlighted ) )
	{
		if ( m_pMRI->IsSelected( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, MF_STRING, IDM_DESELECT_SHAPE, L"Deselect" );
		else
			AppendMenu( hPopupMenu, MF_STRING, IDM_SELECT_SHAPE, L"Select" );
	}	

	return m_shapeHighlighted.GetValue(); // will be forwarded to HandleContextMenuCommand
}

void MainWindow::NNetMove( MicroMeterPoint const & umDelta )	
{ 
	GetDrawContext().Move( umDelta ); 
	Notify( true );  
	if ( m_pCoordObservable )
		m_pCoordObservable->NotifyAll( false );
}

void MainWindow::NNetMove( PixelPoint const & pixDelta ) 
{ 
	GetDrawContext().Move( pixDelta ); 
	Notify( true );  
	if ( m_pCoordObservable )
		m_pCoordObservable->NotifyAll( false );
}

MicroMeterPoint const MainWindow::GetCursorPos( ) const
{
	PixelPoint const pixPoint { GetRelativeCrsrPosition( ) };
	return IsInClientRect( pixPoint )
		? GetCoord().Convert2MicroMeterPointPos( pixPoint )
		: NP_ZERO;
}

bool MainWindow::Zoom( MicroMeter const newSize, PixelPoint const * const pPixPntCenter )
{
	PixelPoint      const pixPntCenter    { pPixPntCenter ? * pPixPntCenter : GetClRectCenter() };
	fPixelPoint     const fPixPointCenter { Convert2fPixelPoint( pixPntCenter ) };                      // compute center
	MicroMeterPoint const umPointcenter   { GetCoord().Convert2MicroMeterPointPos( fPixPointCenter ) }; // ** BEFORE ** zooming!
	if ( GetDrawContext().Zoom( newSize )  )
	{
		GetDrawContext().Center( umPointcenter, fPixPointCenter ); 
		Notify( false ); 
		if ( m_pCoordObservable )
			m_pCoordObservable->NotifyAll( false );
		return true;
	}
	else
	{
		MessageBeep( MB_ICONWARNING );
		return false;
	}
}

void MainWindow::ZoomStep( bool const bZoomIn, PixelPoint const * const pPixPntCenter )
{
	Zoom( GetCoord().ComputeNewPixelSize( bZoomIn ), pPixPntCenter );
}

void MainWindow::CenterModel( )
{
	CenterAndZoomRect( m_pMRI->GetEnclosingRect( ), 1.2f ); // give 20% more space (looks better)
}

//void MainWindow::OnSetCursor( WPARAM const wParam, LPARAM const lParam )
//{
//	bool    const keyDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
//	HCURSOR const hCrsr   = keyDown ? m_hCrsrMove : m_hCrsrArrow;
//	SetCursor( hCrsr );
//}

bool MainWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	NNetWindow::OnSize( wParam, lParam );
	m_pCoordObservable->NotifyAll( false );
	return true;
}

void MainWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint      const ptCrsr    { GetCrsrPosFromLparam( lParam ) };  // screen coordinates
	MicroMeterPoint const umCrsrPos { GetCoord().Convert2MicroMeterPointPos( ptCrsr   ) };
	MicroMeterPoint const umLastPos { GetCoord().Convert2MicroMeterPointPos( m_ptLast ) };

	if ( m_pCursorPosObservable )
		m_pCursorPosObservable->NotifyAll( false );

	if ( wParam & MK_RBUTTON )         // Right mouse button: selection
	{
		if ( m_ptLast.IsNotNull() )    // last cursor pos stored in m_ptLast
		{
			m_rectSelection = MicroMeterRect( umCrsrPos, umLastPos );
		}
		else                           // first time here after RBUTTON pressed
		{
			m_ptLast = ptCrsr;         // store current cursor pos
		}
	}
	else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
	{
		if ( m_ptLast.IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			m_shapeSuperHighlighted = NO_SHAPE;
			if ( IsDefined( m_shapeHighlighted ) ) //-V1051
			{
				setSuperHighlightedShape( m_pMRI->GetShapePos( m_shapeHighlighted ) );
				m_pNNetCommands->MoveShape( m_shapeHighlighted, umCrsrPos - umLastPos );
			}
			else if ( SignalInterface * const pSignal { m_pMonitorData->FindSensor( umCrsrPos ) } )
			{
				pSignal->Move( umCrsrPos - umLastPos );
			}
			else if ( m_pMRI->AnyShapesSelected( ) )   // move selected shapes 
			{
				m_pNNetCommands->MoveSelection( umCrsrPos - umLastPos );
			}
			else  // move view by manipulating coordinate system 
			{
				NNetMove( ptCrsr - m_ptLast );
			}
		}
		m_ptLast = ptCrsr;
	}
	else  // no mouse button pressed
	{                         
		setHighlightedShape( umCrsrPos );
		m_ptLast.Set2Null();   // make m_ptLast invalid
	}
}

void MainWindow::OnLeftButtonDblClick( WPARAM const wParam, LPARAM const lParam )
{
	if ( IsDefined( m_shapeHighlighted ) )
		m_pNNetCommands->SelectShape( m_shapeHighlighted, tBoolOp::opToggle );
}

void MainWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	if ( IsDefined( m_shapeHighlighted ) && IsDefined( m_shapeSuperHighlighted ) )
	{ 
		SendCommand2Application( IDD_CONNECT, 0	);
		m_shapeSuperHighlighted = NO_SHAPE;
	}
}

bool MainWindow::OnRButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	bool bMadeSelection { m_rectSelection.IsNotEmpty() };
	if ( bMadeSelection )
	{
		m_pNNetCommands->SelectShapesInRect( m_rectSelection );
		m_rectSelection.SetZero();
	}
	return bMadeSelection;
}

bool MainWindow::OnRButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	SetFocus( );
	return false;
}

void MainWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{  
	PixelPoint const ptCrsr     { GetRelativeCrsrPosition() };  // screen coordinates
	int        const iDelta     { GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA };
	bool       const bDirection { iDelta > 0 };

	MicroMeterPoint const umCrsrPos { GetCoord().Convert2MicroMeterPointPos( ptCrsr ) };
	if ( SignalInterface * const pSignal { m_pMonitorData->FindSensor( umCrsrPos ) } )
	{
		for ( int iSteps = abs( iDelta ); iSteps > 0; --iSteps )
		{
			pSignal->Size( bDirection ? 1.1f : 0.9f );
		}
	}
	else
	{
		for ( int iSteps = abs( iDelta ); iSteps > 0; --iSteps )
		{
			ZoomStep( bDirection, & ptCrsr );
		}
	}
}

void MainWindow::CenterAndZoomRect( MicroMeterRect const & umRect, float const fRatioFactor )
{
	MicroMeter      umPixelSizeTarget;
	MicroMeterPoint umPntCenterTarget { NP_ZERO };
	GetCoord().ComputeCenterAndZoom
	( 
		umRect.Scale( NEURON_RADIUS ), 
		fRatioFactor, 
		GetClRectSize(),
		umPixelSizeTarget, 
		umPntCenterTarget 
	);
	m_smoothMove.SetUp
	(
		GetCoord().GetPixelSize(),                                   // actual pixel size 
		GetCoord().Convert2MicroMeterPointPos( GetClRectCenter() ),  // actual center 
		umPixelSizeTarget,
		umPntCenterTarget
	);
	m_bFocusMode = true;
}

void MainWindow::OnPaint( )
{
	m_pDisplayTimer->TimerStart( );
	NNetWindow::OnPaint( );
	if ( m_bFocusMode )
	{
		bool        const bTargetReached { m_smoothMove.Next() };
		fPixelPoint const fpCenter       { Convert2fPixelPoint( GetClRectCenter( ) ) };
		GetDrawContext().Zoom  ( m_smoothMove.GetNewSize() );
		GetDrawContext().Center( m_smoothMove.GetNewCenter(), fpCenter );
		Notify( true );                               // cause immediate repaint
		m_pCoordObservable->NotifyAll( false );
		if ( bTargetReached )
		{
			m_bFocusMode = false;
			SendCommand2Application( IDM_CENTERING_FINISHED, 0	);
		}
	}
	m_pDisplayTimer->TimerStop( );
}

bool MainWindow::changePulseRate( ShapeId const id, bool const bDirection )
{
	static fHertz const INCREMENT { 0.01_fHertz };
	fHertz const fOldValue { m_pMRI->GetPulseFrequency( id ) };
	if ( fOldValue.IsNull() )
		return false;
	fHertz const fNewValue = fOldValue + ( bDirection ? INCREMENT : -INCREMENT );
	m_pNNetCommands->SetPulseRate( id, fNewValue );
	return true;
}

void MainWindow::OnChar( WPARAM const wParam, LPARAM const lParam )
{
	if ( wParam == '+' )
		changePulseRate( m_shapeHighlighted, true );
	else if ( wParam == '-' )
		changePulseRate( m_shapeHighlighted, false );
}

/////////////////////// local functions ////////////////////////////////

void MainWindow::doPaint( ) 
{
	PixelRect const pixRect { GetClPixelRect( ) };
	DrawContext   & context { GetDrawContext() };

	if ( m_rectSelection.IsNotEmpty( ) )
		context.DrawTranspRect( m_rectSelection, NNetColors::SELECTION_RECT );

	if ( context.GetPixelSize() <= 5._MicroMeter )
		DrawExteriorInRect( pixRect );

	DrawInteriorInRect( pixRect, [&](Shape const & s) { return s.IsPipe    (); } );
	DrawInteriorInRect( pixRect, [&](Shape const & s) { return s.IsBaseKnot(); } );

	context.ShowScale( GetClRectSize() );

	if ( context.GetPixelSize() <= 2.5_MicroMeter )
		DrawNeuronTextInRect( pixRect );

	if ( IsDefined( m_shapeSuperHighlighted ) ) // draw super highlighted shape again to be sure that it is visible
	{
		m_pMRI->DrawExterior( m_shapeSuperHighlighted, context, tHighlightType::superHighlighted );
		m_pMRI->DrawInterior( m_shapeSuperHighlighted, context );
	}

	if ( IsDefined( m_shapeHighlighted ) )  // draw highlighted shape again to be sure that it is in foreground
	{
		m_pMRI->DrawExterior( m_shapeHighlighted, context, tHighlightType::highlighted );
		m_pMRI->DrawInterior( m_shapeHighlighted, context );
		if ( m_pMRI->IsOfType<Neuron>( m_shapeHighlighted ) )
			m_pMRI->DrawNeuronText( m_shapeHighlighted, context );
	}

	DrawSensors( );
	AnimateBeaconSingle( 30._fPIXEL);
	AnimateBeacon( 30._fPIXEL);
}

void MainWindow::setSuperHighlightedShape( MicroMeterPoint const & umCrsrPos )
{
	if ( m_pMRI->IsOfType<BaseKnot>( m_shapeHighlighted ) )
	{
		m_shapeSuperHighlighted = m_pMRI->FindShapeAt
		( 
			umCrsrPos,
			[&]( Shape const & shape ) { return m_pMRI->ConnectsTo( m_shapeHighlighted, shape.GetId() ); } 
		);
	}
}

void MainWindow::setHighlightedShape( MicroMeterPoint const & umCrsrPos )
{
	ShapeId const idHighlight { m_pMRI->FindShapeAt( umCrsrPos, ShapeCritAlwaysTrue ) };
	if ( idHighlight != m_shapeHighlighted )
	{
		m_shapeHighlighted = idHighlight; 
		Notify( true );     // cause immediate repaint
	}
}
