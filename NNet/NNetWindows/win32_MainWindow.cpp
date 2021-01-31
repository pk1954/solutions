// win32_MainWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "MoreTypes.h"
#include "Resource.h"
#include "Signal.h"
#include "Neuron.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelCommands.h"
#include "win32_MonitorWindow.h"
#include "win32_MainWindow.h"

using std::make_unique;

void MainWindow::Start
(
	HWND                     const   hwndApp, 
	DWORD                    const   dwStyle,
	bool                     const   bShowRefreshRateDialog,
	fPixel                   const   fPixBeaconLimit,
	NNetModelReaderInterface const & modelReaderInterface,
	MonitorWindow            const & monitorWindow,
	NNetController                 & controller,
	NNetModelCommands              & commands,
	Observable                     & cursorObservable,
	Observable                     & coordObservable
)
{
	NNetWindow::Start
	( 
		hwndApp, 
		dwStyle,
		bShowRefreshRateDialog,
		fPixBeaconLimit,
		modelReaderInterface,
		monitorWindow,
		controller
	);
	ShowRefreshRateDlg( bShowRefreshRateDialog );
	m_pNNetCommands        = & commands;
	m_pCursorPosObservable = & cursorObservable;
	m_pCoordObservable     = & coordObservable;
	m_upArrowAnimation     = make_unique<Animation<MicroMeter>>
	( 
		GetWindowHandle(), 
		m_arrowSize, 
		[]( HWND hwnd, UINT msgTimer, UINT_PTR idTimer, DWORD msSinceStart )
		{
			auto pMainWin { GetWinPtr<MainWindow>( hwnd ) };
			pMainWin->m_upArrowAnimation->Next();
		}
	);

	m_upCoordAnimation = make_unique<Animation<PixelCoordsFp>>
	(
		GetWindowHandle(), 
		GetCoord(), 
		[]( HWND hwnd, UINT msgTimer, UINT_PTR idTimer, DWORD msSinceStart )
		{
			auto pMainWin { GetWinPtr<MainWindow>( hwnd ) };
			pMainWin->m_upCoordAnimation->Next();
			pMainWin->m_pCoordObservable->NotifyAll( false );
		}
	);

}

void MainWindow::Stop( )
{
	Reset( );
	NNetWindow::Stop();
}

void MainWindow::Reset( )
{ 
	m_shapeHighlighted = NO_SHAPE; 
	m_shapeTarget      = NO_SHAPE; 
}

long MainWindow::AddContextMenuEntries( HMENU const hPopupMenu )
{
	if ( m_pNMRI->AnyShapesSelected( ) )
	{
		AppendMenu( hPopupMenu, MF_STRING, IDM_DESELECT_ALL,     L"Deselect all" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_COPY_SELECTION,   L"Copy selection" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_DELETE_SELECTION, L"Delete selected objects" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_ALIGN_SELECTION,  L"Align selected objects" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_CLEAR_BEEPERS,    L"Clear selected trigger sounds" );
	}
	else if ( IsUndefined(m_shapeHighlighted) )  // no shape selected, cursor on background
	{
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_NEURON,         L"New neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_INPUT_NEURON,   L"New input neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_NEW_SENSOR,         L"New EEG sensor" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_SELECT_ALL_BEEPERS, L"Select all neurons with trigger sound" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_CLEAR_BEEPERS,      L"Clear all trigger sounds" );
	}
	else switch ( m_pNMRI->GetShapeType( m_shapeHighlighted ).GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		if ( ! m_pNMRI->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_PULSE_RATE,            L"Pulse rate" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SHAPE,          L"Delete" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DISCONNECT,            L"Disconnect" );
		break;

	case ShapeType::Value::neuron:
		if ( ! m_pNMRI->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_INCOMING2KNOT,     L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SHAPE,          L"Delete" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DISCONNECT,            L"Disconnect" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_TRIGGER_SOUND_DLG,     L"Trigger sound" );
		AppendMenu( hPopupMenu, MF_STRING, IDM_SELECT_SUBTREE,        L"Select subtree" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_STOP_ON_TRIGGER,       L"Stop on trigger on/off" );
		break;

	case ShapeType::Value::knot:  
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_INCOMING2KNOT, L"Add incoming dendrite" );
		if ( 
			(! m_pNMRI->HasOutgoing( m_shapeHighlighted )) || 
			(
				! m_pNMRI->HasIncoming( m_shapeHighlighted ) && 
				( m_pNMRI->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 )  
				) 
			)
			AppendMenu( hPopupMenu, MF_STRING, IDD_APPEND_NEURON, L"Add neuron" );
		if ( 
			( m_pNMRI->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 ) && 
			(! m_pNMRI->HasIncoming( m_shapeHighlighted )) 
			)
			AppendMenu( hPopupMenu, MF_STRING, IDD_APPEND_INPUT_NEURON, L"Add input neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DISCONNECT, L"Disconnect" );
		break;

	case ShapeType::Value::pipe:
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_OUTGOING2PIPE, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_ADD_INCOMING2PIPE, L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_INSERT_NEURON,     L"Insert neuron" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_INSERT_KNOT,       L"Insert knot" );
		AppendMenu( hPopupMenu, MF_STRING, IDD_DELETE_SHAPE,      L"Delete" );
		if ( m_arrowSizeTarget > 0.0_MicroMeter )
			AppendMenu( hPopupMenu, MF_STRING, IDD_ARROWS_OFF,    L"Arrows off" );
		else
			AppendMenu( hPopupMenu, MF_STRING, IDD_ARROWS_ON,     L"Arrows on" );
		break;

	default:
		assert( false );
	}

	if ( IsDefined(m_shapeHighlighted) )
	{
		if ( m_pNMRI->IsSelected( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, MF_STRING, IDM_DESELECT_SHAPE, L"Deselect" );
		else
			AppendMenu( hPopupMenu, MF_STRING, IDM_SELECT_SHAPE, L"Select" );
	}	

	return m_shapeHighlighted.GetValue(); // will be forwarded to HandleContextMenuCommand
}

MicroMeterPoint const MainWindow::GetCursorPos( ) const
{
	PixelPoint const pixPoint { GetRelativeCrsrPosition( ) };
	return IsInClientRect( pixPoint )
		? GetCoordC().Transform2MicroMeterPointPos( pixPoint )
		: NP_ZERO;
}

bool MainWindow::Zoom( MicroMeter const newSize, PixelPoint const * const pPixPntCenter )
{
	PixelPoint      const pixPntCenter    { pPixPntCenter ? * pPixPntCenter : GetClRectCenter() };
	fPixelPoint     const fPixPointCenter { Convert2fPixelPoint( pixPntCenter ) };                      // compute center
	MicroMeterPoint const umPointcenter   { GetCoordC().Transform2MicroMeterPointPos( fPixPointCenter ) }; // ** BEFORE ** zooming!
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
	Zoom( GetCoordC().ComputeNewPixelSize( bZoomIn ), pPixPntCenter );
}

void MainWindow::CenterModel( )
{
	centerAndZoomRect( ShapeList::SelMode::allShapes, 1.2f ); // give 20% more space (looks better)
}

void MainWindow::CenterSelection( )
{
	centerAndZoomRect( ShapeList::SelMode::selectedShapes, 2.0f );
}

bool const MainWindow::ArrowsVisible( ) const
{
	return m_arrowSizeTarget > 0._MicroMeter;
}

void MainWindow::ShowArrows( bool const op )
{
	MicroMeter olVal { m_arrowSizeTarget };
	if ( op )
		m_arrowSizeTarget = STD_ARROW_SIZE;
	else 
		m_arrowSizeTarget = 0._MicroMeter;

	if ( m_arrowSizeTarget != olVal )
		m_upArrowAnimation->Start( m_arrowSize, m_arrowSizeTarget );
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

void MainWindow::setTargetShape( )
{
	if ( m_pNMRI->IsOfType<BaseKnot>( m_shapeHighlighted ) )
	{
		m_shapeTarget = m_pNMRI->FindShapeAt
		( 
			m_pNMRI->GetShapePos( m_shapeHighlighted ),
			[&](Shape const & shape) { return m_pNMRI->ConnectsTo( m_shapeHighlighted, shape.GetId() ); } 
		);
	}
}

void MainWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint      const ptCrsr    { GetCrsrPosFromLparam( lParam ) };  // screen coordinates
	MicroMeterPoint const umCrsrPos { GetCoordC().Transform2MicroMeterPointPos( ptCrsr   ) };
	MicroMeterPoint const umLastPos { GetCoordC().Transform2MicroMeterPointPos( m_ptLast ) };

	if ( m_pCursorPosObservable )
		m_pCursorPosObservable->NotifyAll( false );

	if ( wParam & MK_RBUTTON )         // Right mouse button: selection
	{
		if ( m_ptLast.IsNotNull() )    // last cursor pos stored in m_ptLast
		{
			m_rectSelection = MicroMeterRect( umCrsrPos, umLastPos );
			Notify( false ); 
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
			m_shapeTarget = NO_SHAPE;
			if ( IsDefined(m_shapeHighlighted) )
			{
				m_pNNetCommands->MoveShape( m_shapeHighlighted, umCrsrPos - umLastPos );
				setTargetShape( );
			}
			else if ( Signal * const pSignal { m_pNMRI->GetMonitorData().FindSensor( umCrsrPos ) } )
			{
				pSignal->Move( umCrsrPos - umLastPos );
				Notify( false ); 
			}
			else if ( m_pNMRI->AnyShapesSelected( ) )   // move selected shapes 
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
	{
		m_pNNetCommands->SelectShape( m_shapeHighlighted, tBoolOp::opToggle );
	}
}

void MainWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	if ( IsDefined( m_shapeHighlighted ) && IsDefined( m_shapeTarget ) )
	{ 
		SendCommand2Application( IDD_CONNECT, 0	);
		m_shapeTarget = NO_SHAPE;
	}
}

bool MainWindow::OnRButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	bool const bSelection { m_rectSelection.IsNotEmpty() };
	if ( bSelection )
	{
		m_pNNetCommands->SelectShapesInRect( m_rectSelection, !(wParam & MK_CONTROL) );
		m_rectSelection.SetZero();
	}
	return bSelection; // let base class handle other cases
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

	MicroMeterPoint const umCrsrPos { GetCoordC().Transform2MicroMeterPointPos( ptCrsr ) };
	if ( Signal * const pSignal { m_pNMRI->GetMonitorData().FindSensor( umCrsrPos ) } )
	{
		for ( int iSteps = abs( iDelta ); iSteps > 0; --iSteps )
		{
			pSignal->Size( bDirection ? 1.1f : 0.9f );
			Notify( false ); 
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

void MainWindow::centerAndZoomRect
( 
	ShapeList::SelMode const mode, 
	float              const fRatioFactor 
)
{
	MicroMeterRect umRect { m_pNMRI->GetShapes().CalcEnclosingRect( mode ) };
	PixelCoordsFp  coordTarget;
	coordTarget.SetPixelSize
	( 
		GetCoord().ComputeZoom( umRect.Scale( NEURON_RADIUS ), GetClRectSize(), fRatioFactor ) 
	);
	coordTarget.SetPixelOffset
	( 
		coordTarget.Transform2fPixelSize( umRect.GetCenter() ) - 
		Convert2fPixelPoint( GetClRectCenter() ) 
	);
	m_upCoordAnimation->Start( GetCoord(), coordTarget );
}

void MainWindow::OnPaint( )
{
	m_pDisplayTimer->TimerStart( );
	NNetWindow::OnPaint( );
	m_pDisplayTimer->TimerStop( );
}

bool MainWindow::changePulseRate( ShapeId const id, bool const bDirection )
{
	static fHertz const INCREMENT { 0.01_fHertz };
	fHertz const fOldValue { m_pNMRI->GetPulseFrequency( id ) };
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
	PixelRect   const   pixRect { GetClPixelRect( ) };
	DrawContext const & context { GetDrawContext() };

	if ( m_rectSelection.IsNotEmpty( ) )
		context.DrawTranspRect( m_rectSelection, NNetColors::SELECTION_RECT );

	context.ShowScale( GetClRectSize() );

	if ( context.GetPixelSize() <= 5._MicroMeter )
	{
		DrawExteriorInRect( pixRect );
		if ( m_arrowSize > 0.0_MicroMeter )
			DrawArrowsInRect( pixRect, m_arrowSize );
	}

	DrawInteriorInRect( pixRect, [&](Shape const & s) { return s.IsPipe    (); } ); 
	DrawInteriorInRect( pixRect, [&](Shape const & s) { return s.IsBaseKnot(); } ); // draw BaseKnots OVER Pipes
	DrawNeuronTextInRect( pixRect );

	if ( IsDefined(m_shapeTarget) ) // draw target shape again to be sure that it is visible
	{
		m_pNMRI->DrawExterior( m_shapeTarget, context, tHighlightType::target );
		m_pNMRI->DrawInterior( m_shapeTarget, context, tHighlightType::target );
	}

	if ( IsDefined(m_shapeHighlighted) )  // draw selected shape again to be sure that it is in foreground
	{
		m_pNMRI->DrawExterior( m_shapeHighlighted, context, tHighlightType::highlighted );
		m_pNMRI->DrawInterior( m_shapeHighlighted, context, tHighlightType::highlighted );
		m_pNMRI->DrawNeuronText( m_shapeHighlighted, context );
	}

	DrawSensors( );
	DrawBeacon( );
}

void MainWindow::setHighlightedShape( MicroMeterPoint const & umCrsrPos )
{
	ShapeId const idHighlight { m_pNMRI->FindShapeAt( umCrsrPos, ShapeCritAlwaysTrue ) };
	if ( idHighlight != m_shapeHighlighted )
	{
		m_shapeHighlighted = idHighlight; 
		Notify( true );     // cause immediate repaint
	}
}
