// win32_MainWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <unordered_map>
#include "MoreTypes.h"
#include "Resource.h"
#include "Signal.h"
#include "MicroMeterPointVector.h"
#include "Neuron.h"
#include "Connector.h"
#include "NNetColors.h"
#include "NNetParameters.h"
#include "NNetModelCommands.h"
#include "ConnAnimationCommand.h"
#include "win32_callable.h"
#include "win32_Commands.h"
#include "win32_MonitorWindow.h"
#include "win32_MainWindow.h"

using std::unordered_map;
using std::unique_ptr;
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
	NNetModelCommands              & modelCommands,
	WinCommands                    & winCommands,  
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
	m_pWinCommands         = & winCommands;
	m_pModelCommands       = & modelCommands;
	m_pCursorPosObservable = & cursorObservable;
	m_pCoordObservable     = & coordObservable;
	m_scale.Initialize( & m_graphics, L"m" );

	m_upCoordAnimation = make_unique<Animation<PixelCoordsFp>>
	(
		[&](bool const bTargetReached)
		{ 
			GetCoord().Set( m_upCoordAnimation->GetActual() );
			Notify(false);
		}
	);
	m_upArrowAnimation = make_unique<Animation<MicroMeter>>
	(
		[&](bool const bTargetReached)
		{ 
			m_arrowSize = m_upArrowAnimation->GetActual();
			Notify(false);
		}
	);
}

void MainWindow::Stop()
{
	Reset();
	NNetWindow::Stop();
}

void MainWindow::Reset()
{ 
	m_shapeHighlighted = NO_SHAPE; 
	setNoTarget(); 
}

void appendMenu(HMENU const hPopupMenu, int const idCommand)
{
	static unordered_map <int, LPCWSTR const> mapCommands =
	{
		{ IDD_ADD_INCOMING2KNOT,   L"Add incoming dendrite"                 },
		{ IDD_ADD_INCOMING2PIPE,   L"Add incoming dendrite"                 },
		{ IDD_ADD_OUTGOING2KNOT,   L"Add outgoing dendrite"                 },
		{ IDD_ADD_OUTGOING2PIPE,   L"Add outgoing dendrite"                 },
		{ IDD_ADD_SIGNAL,          L"New EEG sensor" 					    },
		{ IDM_ALIGN_SHAPES,        L"Align selected objects"                },
		{ IDD_APPEND_INPUT_NEURON, L"Add input neuron"                      },
		{ IDD_APPEND_NEURON,       L"Add neuron"                            },
		{ IDD_ARROWS_OFF,          L"Arrows off"                            },
		{ IDD_ARROWS_ON,           L"Arrows on"                             },
		{ IDM_CLEAR_BEEPERS,       L"Clear selected trigger sounds"         },
		{ IDM_COPY_SELECTION,      L"Copy selection"                        },
		{ IDM_DELETE_SELECTION,    L"Delete selected objects"               },
		{ IDD_DELETE_SHAPE,        L"Delete"                                },
		{ IDM_DESELECT_ALL,        L"Deselect all"                          },
		{ IDM_DESELECT_SHAPE,      L"Deselect"                              },
		{ IDD_DISCONNECT,          L"Disconnect"                            },
		{ IDD_INSERT_KNOT,         L"Insert knot"                           },
		{ IDD_INSERT_NEURON,       L"Insert neuron"                         },
		{ IDM_MAKE_CONNECTOR,      L"Make connector"                        },
		{ IDD_NEW_INPUT_NEURON,    L"New input neuron" 					    },
		{ IDD_NEW_NEURON,          L"New neuron"                            },
		{ IDD_NEW_OUTPUT_NEURON,   L"New output neuron"					    },
		{ IDD_PULSE_RATE,          L"Pulse rate"                            },
		{ IDM_SELECT_ALL_BEEPERS,  L"Select all neurons with trigger sound" }, 
		{ IDM_SELECT_SHAPE,        L"Select"                                },
		{ IDM_SELECT_SUBTREE,      L"Select subtree"                        },
		{ IDD_STOP_ON_TRIGGER,     L"Stop on trigger on/off"                },
		{ IDD_TRIGGER_SOUND_DLG,   L"Trigger sound"                         }
	};
	AppendMenu( hPopupMenu, MF_STRING, idCommand, mapCommands.at(idCommand) );
}

long MainWindow::AddContextMenuEntries( HMENU const hPopupMenu )
{
	if ( m_pNMRI->AnyShapesSelected() )
	{
		appendMenu( hPopupMenu, IDM_DESELECT_ALL     );
		appendMenu( hPopupMenu, IDM_COPY_SELECTION   );
		appendMenu( hPopupMenu, IDM_DELETE_SELECTION );
		appendMenu( hPopupMenu, IDM_MAKE_CONNECTOR   );
		appendMenu( hPopupMenu, IDM_ALIGN_SHAPES     );
		appendMenu( hPopupMenu, IDM_CLEAR_BEEPERS    );
	}
	else if ( IsUndefined(m_shapeHighlighted) )  // no shape selected, cursor on background
	{
		appendMenu( hPopupMenu, IDD_NEW_NEURON         );
		appendMenu( hPopupMenu, IDD_NEW_INPUT_NEURON   );
		appendMenu( hPopupMenu, IDD_NEW_OUTPUT_NEURON  );
		appendMenu( hPopupMenu, IDD_ADD_SIGNAL         );
		appendMenu( hPopupMenu, IDM_SELECT_ALL_BEEPERS );
		appendMenu( hPopupMenu, IDM_CLEAR_BEEPERS      );
	}
	else switch ( m_pNMRI->GetShapeType( m_shapeHighlighted ).GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		if ( ! m_pNMRI->HasOutgoing( m_shapeHighlighted ) )
			appendMenu( hPopupMenu, IDD_ADD_OUTGOING2KNOT );
		appendMenu( hPopupMenu, IDD_PULSE_RATE );         
		appendMenu( hPopupMenu, IDD_DELETE_SHAPE );
		appendMenu( hPopupMenu, IDD_DISCONNECT );        
		break;

	case ShapeType::Value::outputNeuron:
		appendMenu( hPopupMenu, IDD_ADD_INCOMING2KNOT );
		appendMenu( hPopupMenu, IDD_DELETE_SHAPE );   
		appendMenu( hPopupMenu, IDD_DISCONNECT );        
		appendMenu( hPopupMenu, IDD_TRIGGER_SOUND_DLG );
		appendMenu( hPopupMenu, IDD_STOP_ON_TRIGGER );   
		break;

	case ShapeType::Value::neuron:
		if ( ! m_pNMRI->HasOutgoing( m_shapeHighlighted ) )
			appendMenu( hPopupMenu, IDD_ADD_OUTGOING2KNOT );
		appendMenu( hPopupMenu, IDD_ADD_INCOMING2KNOT );
		appendMenu( hPopupMenu, IDD_DELETE_SHAPE );   
		appendMenu( hPopupMenu, IDD_DISCONNECT );        
		appendMenu( hPopupMenu, IDD_TRIGGER_SOUND_DLG );
		appendMenu( hPopupMenu, IDM_SELECT_SUBTREE );   
		appendMenu( hPopupMenu, IDD_STOP_ON_TRIGGER );      
		break;

	case ShapeType::Value::connector:
		appendMenu( hPopupMenu, IDD_DELETE_SHAPE );     
		appendMenu( hPopupMenu, IDD_DISCONNECT );        
		break;

	case ShapeType::Value::closedConnector:
		//appendMenu( hPopupMenu, IDD_DELETE_SHAPE );     
		//appendMenu( hPopupMenu, IDD_DISCONNECT );        
		break;

	case ShapeType::Value::knot:  
		appendMenu( hPopupMenu, IDD_ADD_OUTGOING2KNOT );
		appendMenu( hPopupMenu, IDD_ADD_INCOMING2KNOT );
		if ( 
			  (! m_pNMRI->HasOutgoing( m_shapeHighlighted )) || 
			  (
				 ( ! m_pNMRI->HasIncoming( m_shapeHighlighted ) ) && 
				 ( m_pNMRI->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 )  
			  ) 
		   )
		   appendMenu( hPopupMenu, IDD_APPEND_NEURON );
		if ( 
			  ( ! m_pNMRI->HasIncoming( m_shapeHighlighted ) ) &&
			  ( m_pNMRI->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 )
		   )
		   appendMenu( hPopupMenu, IDD_APPEND_INPUT_NEURON );
		appendMenu( hPopupMenu, IDD_DISCONNECT );
		break;

	case ShapeType::Value::pipe:
		appendMenu( hPopupMenu, IDD_ADD_OUTGOING2PIPE );
		appendMenu( hPopupMenu, IDD_ADD_INCOMING2PIPE );
		appendMenu( hPopupMenu, IDD_INSERT_NEURON );
		appendMenu( hPopupMenu, IDD_INSERT_KNOT );   
		appendMenu( hPopupMenu, IDD_DELETE_SHAPE );     
		if ( ArrowsVisible() )
			appendMenu( hPopupMenu, IDD_ARROWS_OFF );    
		else
			appendMenu( hPopupMenu, IDD_ARROWS_ON );  
		break;

	default:
		assert( false );
	}

	if ( IsDefined(m_shapeHighlighted) )
	{
		if ( m_pNMRI->IsSelected( m_shapeHighlighted ) )
			appendMenu( hPopupMenu, IDM_DESELECT_SHAPE );
		else
			appendMenu( hPopupMenu, IDM_SELECT_SHAPE );
	}	

	return m_shapeHighlighted.GetValue(); // will be forwarded to HandleContextMenuCommand
}

MicroMeterPoint const MainWindow::GetCursorPos() const
{
	PixelPoint const pixPoint { GetRelativeCrsrPosition() };
	return IsInClientRect( pixPoint )
		? GetCoordC().Transform2MicroMeterPointPos( pixPoint )
		: NP_ZERO;
}

bool MainWindow::Zoom( MicroMeter const newSize, PixelPoint const * const pPixPntCenter )
{
	PixelPoint      const pixPntCenter    { pPixPntCenter ? * pPixPntCenter : GetClRectCenter() };
	fPixelPoint     const fPixPointCenter { Convert2fPixelPoint( pixPntCenter ) };                         // compute center
	MicroMeterPoint const umPointcenter   { GetCoordC().Transform2MicroMeterPointPos( fPixPointCenter ) }; // ** BEFORE ** zooming!
	if ( GetDrawContext().Zoom( newSize )  )
	{
		GetDrawContext().Center( umPointcenter, fPixPointCenter ); 
		Notify( false ); 
		if ( m_pCoordObservable )
			m_pCoordObservable->NotifyAll( false );
		m_scale.SetHorzPixelSize( newSize.GetValue() );
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

void MainWindow::CenterModel()
{
	centerAndZoomRect( UPShapeList::SelMode::allShapes, 1.2f ); // give 20% more space (looks better)
}

void MainWindow::CenterSelection()
{
	centerAndZoomRect( UPShapeList::SelMode::selectedShapes, 2.0f );
}

bool const MainWindow::ArrowsVisible() const
{
	return m_arrowSize > 0._MicroMeter;
}

void MainWindow::ShowArrows( bool const op )
{
	MicroMeter oldVal { m_arrowSize };
	m_arrowSizeTarget = op ? STD_ARROW_SIZE : 0._MicroMeter;
	if ( m_arrowSizeTarget != oldVal )
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
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_scale.SetClientRectSize( PIXEL(width), PIXEL(height) );
	return true;
}

void MainWindow::setNoTarget()
{
	m_shapeTarget = NO_SHAPE;
	m_bTargetFits = false;
}

void MainWindow::setTargetShape()
{
	m_shapeTarget = m_pNMRI->FindShapeAt
	(
		m_pNMRI->GetShapePos( m_shapeHighlighted ),
		[&](Shape const & s) { return m_pNMRI->IsConnectionCandidate(s.GetId(), m_shapeHighlighted); }
	);
	m_bTargetFits = IsDefined(m_shapeTarget) && m_pNMRI->CanConnectTo(m_shapeHighlighted, m_shapeTarget); 
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
			MicroMeterPoint umDelta = umCrsrPos - umLastPos;
			if ( IsDefined(m_shapeHighlighted) )    // move highligthted shape
			{
				if (umDelta.IsNotZero())
				{
					if ((wParam & MK_CONTROL) && m_pNMRI->GetConstShape(m_shapeHighlighted)->IsAnyConnector())
					{
						m_pModelCommands->Rotate(m_shapeHighlighted, umLastPos, umCrsrPos);
					}
					else
					{
						m_pModelCommands->MoveShape(m_shapeHighlighted, umDelta);
						setTargetShape();
					}
				}
			}
			else if (Signal * const pSignal { m_pNMRI->GetMonitorData().FindSensor(umCrsrPos) })
			{                                // move signal
				pSignal->Move( umDelta );
				Notify( false ); 
			}
			else if (m_pNMRI->AnyShapesSelected())   // move selected shapes 
			{
				m_pModelCommands->MoveSelection( umDelta );
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
		m_pModelCommands->SelectShape( m_shapeHighlighted, tBoolOp::opToggle );
	}
}

void MainWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	if ( IsDefined(m_shapeHighlighted) && IsDefined(m_shapeTarget) && m_bTargetFits )
	{ 
		SendCommand2Application( IDD_CONNECT, 0	);
	}
	setNoTarget();
}

bool MainWindow::OnRButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	bool const bSelection { m_rectSelection.IsNotEmpty() };
	if ( bSelection )
	{
		m_pModelCommands->SelectShapesInRect(m_rectSelection, wParam & MK_CONTROL);
		m_rectSelection.SetZero();
	}
	return bSelection; // let base class handle other cases
}

bool MainWindow::OnRButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	SetFocus();
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
	UPShapeList::SelMode const mode, 
	float                const fRatioFactor 
)
{
	MicroMeterRect umRect { m_pNMRI->GetUPShapes().CalcEnclosingRect( mode ) };
	PixelCoordsFp  coordTarget;
	coordTarget.SetPixelSize  // do not change order!
	( 
		GetCoord().ComputeZoom( umRect.Scale( NEURON_RADIUS ), GetClRectSize(), fRatioFactor ) 
	);
	coordTarget.SetPixelOffset // do not change order! 
	( 
		coordTarget.Transform2fPixelSize( umRect.GetCenter() ) -  // SetPixelSize result is used here  
		Convert2fPixelPoint( GetClRectCenter() ) 
	);
	m_upCoordAnimation->Start( GetCoord(), coordTarget );
}

void MainWindow::OnPaint()
{
	m_pDisplayTimer->TimerStart();
	NNetWindow::OnPaint();
	m_pDisplayTimer->TimerStop();
}

bool MainWindow::changePulseRate( ShapeId const id, bool const bDirection )
{
	static fHertz const INCREMENT { 0.01_fHertz };
	fHertz const fOldValue { m_pNMRI->GetPulseFrequency( id ) };
	if ( fOldValue.IsNull() )
		return false;
	fHertz const fNewValue = fOldValue + ( bDirection ? INCREMENT : -INCREMENT );
	m_pModelCommands->SetPulseRate( id, fNewValue );
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

void MainWindow::doPaint() 
{
	PixelRect   const   pixRect { GetClPixelRect() };
	DrawContext const & context { GetDrawContext() };

	if ( m_rectSelection.IsNotEmpty() )
		context.DrawTranspRect( m_rectSelection, NNetColors::SELECTION_RECT );

	m_scale.DisplayStaticScale();

	if ( context.GetPixelSize() <= 5._MicroMeter )
	{
		DrawExteriorInRect( pixRect );
		if ( ArrowsVisible() )
			DrawArrowsInRect( pixRect, m_arrowSize );
	}

	DrawInteriorInRect( pixRect, [&](Shape const & s) { return s.IsPipe() && ! s.IsSelected(); } ); 
	DrawInteriorInRect( pixRect, [&](Shape const & s) { return s.IsPipe() &&   s.IsSelected(); } ); 
	DrawInteriorInRect( pixRect, [&](Shape const & s) { return s.IsBaseKnot (); } ); // draw BaseKnots OVER Pipes
	DrawInteriorInRect( pixRect, [&](Shape const & s) { return s.IsConnector(); } ); 
	DrawNeuronTextInRect( pixRect );

	if ( IsDefined(m_shapeTarget) ) // draw target shape again to be sure that it is visible
	{
		tHighlight type { m_bTargetFits ? tHighlight::targetFit : tHighlight::targetNoFit };
		m_pNMRI->DrawExterior( m_shapeTarget, context, type );
		m_pNMRI->DrawInterior( m_shapeTarget, context, type );
	}

	if ( IsDefined(m_shapeHighlighted) )  // draw highlighted shape again to be sure that it is in foreground
	{
		m_pNMRI->DrawExterior( m_shapeHighlighted, context, tHighlight::highlighted );
		m_pNMRI->DrawInterior( m_shapeHighlighted, context, tHighlight::highlighted );
		m_pNMRI->DrawNeuronText( m_shapeHighlighted, context );
	}

	DrawSensors();
	DrawBeacon();
}

void MainWindow::setHighlightedShape( MicroMeterPoint const & umCrsrPos )
{
	ShapeId const idHighlight { m_pNMRI->FindShapeAt( umCrsrPos ) };
	if ( idHighlight != m_shapeHighlighted )
	{
		m_shapeHighlighted = idHighlight; 
		Notify( false );
	}
}

bool MainWindow::UserProc
( 
	UINT   const uMsg, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	if ( uMsg == WM_APP_UI_CALL )
	{ 
		Callable::DoCall(lParam);
		return 0;
	}
	return NNetWindow::UserProc(uMsg, wParam, lParam); 
}

bool MainWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
	int const wmId = LOWORD( wParam );

	switch (wmId)
	{

	case IDM_MAKE_CONNECTOR:
		m_pWinCommands->MakeConnector(*this);
		break;

	default:
		break;
	}

	return NNetWindow::OnCommand( wParam, lParam, pixPoint );
}
