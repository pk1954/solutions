// win32_MainWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <unordered_map>
#include "MoreTypes.h"
#include "Resource.h"
#include "Signal.h"
#include "MicroMeterPntVector.h"
#include "Knot.h"
#include "Neuron.h"
#include "IoConnector.h"
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
	m_nobHighlighted = NO_NOB; 
	setNoTarget(); 
}

void appendMenu(HMENU const hPopupMenu, int const idCommand)
{
	static unordered_map <int, LPCWSTR const> mapCommands =
	{
		{ IDD_ADD_INCOMING2KNOT,      L"Add incoming dendrite"       },
		{ IDD_ADD_INCOMING2PIPE,      L"Add incoming dendrite"       },
		{ IDD_ADD_OUTGOING2KNOT,      L"Add outgoing dendrite"       },
		{ IDD_ADD_OUTGOING2PIPE,      L"Add outgoing dendrite"       },
		{ IDD_ADD_SIGNAL,             L"New EEG sensor" 		     },
//		{ IDM_ALIGN_NOBS,             L"Align selected objects"      },
		{ IDD_APPEND_INPUT_NEURON,    L"Add input neuron"            },
		{ IDD_APPEND_OUTPUT_NEURON,   L"Add output neuron"           },
		{ IDD_ARROWS_OFF,             L"Arrows off"                  },
		{ IDD_ARROWS_ON,              L"Arrows on"                   },
		{ IDM_COPY_SELECTION,         L"Copy selection"              },
		{ IDM_DELETE_SELECTION,       L"Delete selected objects"     },
		{ IDD_DELETE_NOB,             L"Delete"                      },
		{ IDM_DESELECT_ALL,           L"Deselect all"                },
		{ IDM_DESELECT_NOB,           L"Deselect"                    },
		{ IDD_DISC_BASEKNOT,          L"Disconnect"                  },
		{ IDD_DISC_IOCONNECTOR,       L"Disconnect"                  },
		{ IDM_DISC_CLOSED_CONNECTOR,  L"Disconnect (make neurons)"   },
		{ IDD_SPLIT_NEURON,           L"Split (make I/O neurons)"    },
		{ IDM_SPLIT_CLOSED_CONNECTOR, L"Split (make I/O connectors)" },
		{ IDD_INSERT_KNOT,            L"Insert knot"                 },
		{ IDD_INSERT_NEURON,          L"Insert neuron"               },
		{ IDM_MAKE_CONNECTOR,         L"Make connector"              },
		{ IDD_NEW_INPUT_NEURON,       L"New input neuron" 		     },
		{ IDD_NEW_OUTPUT_NEURON,      L"New output neuron"		     },
		{ IDD_PULSE_RATE,             L"Pulse rate"                  },
		{ IDM_SELECT_NOB,             L"Select"                      },
		{ IDM_SELECT_SUBTREE,         L"Select subtree"              },
		{ IDD_STOP_ON_TRIGGER,        L"Stop on trigger on/off"      },
		{ IDD_TRIGGER_SOUND_DLG,      L"Trigger sound"               }
	};
	AppendMenu( hPopupMenu, MF_STRING, idCommand, mapCommands.at(idCommand) );
}

LPARAM MainWindow::AddContextMenuEntries(HMENU const hPopupMenu)
{
	if ( m_pNMRI->AnyNobsSelected() )
	{
		HMENU hSelectionMenu { Util::PopupMenu(hPopupMenu, L"Selection") };
		appendMenu( hSelectionMenu, IDM_DESELECT_ALL     );
		appendMenu( hSelectionMenu, IDM_COPY_SELECTION   );
		appendMenu( hSelectionMenu, IDM_DELETE_SELECTION );
		appendMenu( hSelectionMenu, IDM_MAKE_CONNECTOR   );
//		appendMenu( hSelectionMenu, IDM_ALIGN_NOBS       );
	}

	if ( IsUndefined(m_nobHighlighted) )  // no nob selected, cursor on background
	{
		appendMenu( hPopupMenu, IDD_NEW_INPUT_NEURON  );
		appendMenu( hPopupMenu, IDD_NEW_OUTPUT_NEURON );
		appendMenu( hPopupMenu, IDD_ADD_SIGNAL        );
	}
	else switch ( m_pNMRI->GetNobType(m_nobHighlighted).GetValue() )
	{
	case NobType::Value::inputNeuron:
		if ( ! m_pNMRI->HasOutgoing(m_nobHighlighted) )
			appendMenu( hPopupMenu, IDD_ADD_OUTGOING2KNOT );
		appendMenu( hPopupMenu, IDD_PULSE_RATE );         
		break;

	case NobType::Value::outputNeuron:
		appendMenu( hPopupMenu, IDD_ADD_INCOMING2KNOT );
		appendMenu( hPopupMenu, IDD_TRIGGER_SOUND_DLG );
		appendMenu( hPopupMenu, IDD_STOP_ON_TRIGGER );   
		break;

	case NobType::Value::neuron:
		if ( ! m_pNMRI->HasOutgoing(m_nobHighlighted) )
			appendMenu( hPopupMenu, IDD_ADD_OUTGOING2KNOT );
		appendMenu( hPopupMenu, IDD_ADD_INCOMING2KNOT );
		appendMenu( hPopupMenu, IDD_SPLIT_NEURON );        
		appendMenu( hPopupMenu, IDD_TRIGGER_SOUND_DLG );
		appendMenu( hPopupMenu, IDD_STOP_ON_TRIGGER );      
		appendMenu( hPopupMenu, IDM_SELECT_SUBTREE );   
		break;

	case NobType::Value::inputConnector:
	case NobType::Value::outputConnector:
		appendMenu( hPopupMenu, IDD_DISC_IOCONNECTOR );        
		break;

	case NobType::Value::closedConnector:
		appendMenu( hPopupMenu, IDM_SPLIT_CLOSED_CONNECTOR );        
		appendMenu( hPopupMenu, IDM_DISC_CLOSED_CONNECTOR );        
		break;

	case NobType::Value::knot:  
		appendMenu( hPopupMenu, IDD_ADD_OUTGOING2KNOT );
		appendMenu( hPopupMenu, IDD_ADD_INCOMING2KNOT );
		if ( 
			 ( ! m_pNMRI->HasIncoming(m_nobHighlighted) ) &&
			 ( m_pNMRI->GetNrOfOutgoingConnections(m_nobHighlighted) <= 1 )
		   )
			appendMenu( hPopupMenu, IDD_APPEND_INPUT_NEURON );
		if ( ! m_pNMRI->HasOutgoing( m_nobHighlighted ) ) 
			appendMenu( hPopupMenu, IDD_APPEND_OUTPUT_NEURON );
		appendMenu( hPopupMenu, IDD_DISC_BASEKNOT );
		break;

	case NobType::Value::pipe:
		appendMenu( hPopupMenu, IDD_ADD_OUTGOING2PIPE );
		appendMenu( hPopupMenu, IDD_ADD_INCOMING2PIPE );
		appendMenu( hPopupMenu, IDD_INSERT_NEURON );
		appendMenu( hPopupMenu, IDD_INSERT_KNOT );   
		if ( ArrowsVisible() )
			appendMenu( hPopupMenu, IDD_ARROWS_OFF );    
		else
			appendMenu( hPopupMenu, IDD_ARROWS_ON );  
		break;

	default:
		assert( false );
	}
	if ( IsDefined(m_nobHighlighted) )
	{
		appendMenu( hPopupMenu, IDD_DELETE_NOB );
		if ( m_pNMRI->IsSelected(m_nobHighlighted) )
			appendMenu( hPopupMenu, IDM_DESELECT_NOB );
		else
			appendMenu( hPopupMenu, IDM_SELECT_NOB );
	}	

	return static_cast<LPARAM>(m_nobHighlighted.GetValue()); // will be forwarded to HandleContextMenuCommand
}

MicroMeterPnt const MainWindow::GetCursorPos() const
{
	PixelPoint const pixPoint { GetRelativeCrsrPosition() };
	return IsInClientRect( pixPoint )
		? GetCoordC().Transform2MicroMeterPntPos( pixPoint )
		: NP_ZERO;
}

bool MainWindow::Zoom( MicroMeter const newSize, PixelPoint const * const pPixPntCenter )
{
	PixelPoint      const pixPntCenter    { pPixPntCenter ? * pPixPntCenter : GetClRectCenter() };
	fPixelPoint     const fPixPointCenter { Convert2fPixelPoint( pixPntCenter ) };                         // compute center
	MicroMeterPnt const umPointcenter   { GetCoordC().Transform2MicroMeterPntPos( fPixPointCenter ) }; // ** BEFORE ** zooming!
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
	centerAndZoomRect(UPNobList::SelMode::allNobs, 1.2f); // give 20% more space (looks better)
}

void MainWindow::CenterSelection()
{
	centerAndZoomRect(UPNobList::SelMode::selectedNobs, 2.0f);
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
	m_nobTarget   = NO_NOB;
	m_bTargetFits = false;
}

void MainWindow::setTargetNob()
{
	m_nobTarget = m_pNMRI->FindNobAt
	(
		m_pNMRI->GetNobPos(m_nobHighlighted),
		[&](Nob const & s) 
		{ 
			return m_pNMRI->IsConnectionCandidate(m_nobHighlighted, s.GetId()); 
		}
	);
	m_bTargetFits = IsDefined(m_nobTarget) && m_pNMRI->CanConnectTo(m_nobHighlighted, m_nobTarget); 
}

void MainWindow::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
	if ( m_pCursorPosObservable )
		m_pCursorPosObservable->NotifyAll(false);

	PixelPoint    const ptCrsr    { GetCrsrPosFromLparam( lParam ) };  // screen coordinates
	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2MicroMeterPntPos(ptCrsr) };

	if (wParam == 0)                     // no mouse buttons or special keyboard keys pressed
	{
		setHighlightedNob(umCrsrPos);
		m_ptLast.Set2Null();             // make m_ptLast invalid
		return;
	}

	PixelPoint const ptLast = m_ptLast;
	m_ptLast = ptCrsr;
	if (ptLast.IsNull())
		return;

	if (wParam & MK_RBUTTON)        // Right mouse button: selection
	{
		m_rectSelection = MicroMeterRect(m_umPntSelectionAnchor, umCrsrPos);
		Notify( false );
		return;
	}
	
	if (wParam & MK_LBUTTON)        // Left mouse button
	{
		MicroMeterPnt const umLastPos { GetCoordC().Transform2MicroMeterPntPos(ptLast) };
		MicroMeterPnt const umDelta   { umCrsrPos - umLastPos };
		if (umDelta.IsZero())
			return;

		if (Signal * const pSignal { m_pNMRI->FindSensor(umCrsrPos) })
		{                                
			pSignal->Move( umDelta );          // move signal
			Notify( false ); 
		}
		else if ( IsDefined(m_nobHighlighted) )    // operate on single nob
		{
			if (wParam & MK_CONTROL)
			{
				m_pModelCommands->Rotate(m_nobHighlighted, umLastPos, umCrsrPos);
			}
			else
			{
				m_pModelCommands->MoveNob(m_nobHighlighted, umDelta);
				setTargetNob();
			}
		}
		else if (wParam & MK_SHIFT)     // operate on selection
		{
			if (m_pNMRI->AnyNobsSelected())
			{
				if (wParam & MK_CONTROL)
				{
					// rotate selection
				}
				else 
				{
					m_pModelCommands->MoveSelection( umDelta );
				}
			}
		}
		else 
		{
			NNetMove(ptCrsr - ptLast);     // move view by manipulating coordinate system 
		}
	}
}

void MainWindow::OnLeftButtonDblClick( WPARAM const wParam, LPARAM const lParam )
{
	if (! m_pNMRI->IsOfType<Knot>(m_nobHighlighted))
	{
		m_pModelCommands->SelectNob( m_nobHighlighted, tBoolOp::opToggle );
	}
}

void MainWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	if (m_bTargetFits)
		SendCommand2Application( IDD_CONNECT, 0	);
	setNoTarget();
}

bool MainWindow::OnRButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	bool const bSelection { m_rectSelection.IsNotEmpty() };
	if ( bSelection )
	{
		m_pModelCommands->SelectNobsInRect(m_rectSelection);
		m_rectSelection.SetZero();
	}
	return bSelection; // let base class handle other cases
}

bool MainWindow::OnRButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint      const ptCrsr    { GetCrsrPosFromLparam( lParam ) };  // screen coordinates
	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2MicroMeterPntPos(ptCrsr) };

	m_umPntSelectionAnchor = umCrsrPos;
	SetFocus();
	return false;
}

void MainWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{  
	PixelPoint const ptCrsr     { GetRelativeCrsrPosition() };  // screen coordinates
	int        const iDelta     { GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA };
	bool       const bDirection { iDelta > 0 };

	MicroMeterPnt const umCrsrPos { GetCoordC().Transform2MicroMeterPntPos( ptCrsr ) };
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
	UPNobList::SelMode const mode, 
	float              const fRatioFactor 
)
{
	MicroMeterRect umRect { m_pNMRI->GetUPNobs().CalcEnclosingRect(mode) };
	PixelCoordsFp  coordTarget;
	coordTarget.SetPixelSize  // do not change order!
	( 
		GetCoord().ComputeZoom(umRect.Scale(NEURON_RADIUS), GetClRectSize(), fRatioFactor) 
	);
	coordTarget.SetPixelOffset // do not change order! 
	( 
		coordTarget.Transform2fPixelSize(umRect.GetCenter()) -  // SetPixelSize result is used here  
		Convert2fPixelPoint(GetClRectCenter()) 
	);
	m_upCoordAnimation->Start(GetCoord(), coordTarget);
}

void MainWindow::OnPaint()
{
	m_pDisplayTimer->TimerStart();
	NNetWindow::OnPaint();
	m_pDisplayTimer->TimerStop();
}

bool MainWindow::changePulseRate( NobId const id, bool const bDirection )
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
		changePulseRate( m_nobHighlighted, true );
	else if ( wParam == '-' )
		changePulseRate( m_nobHighlighted, false );
}

/////////////////////// local functions ////////////////////////////////

void MainWindow::doPaint() 
{
	PixelRect   const   pixRect { GetClPixelRect() };
	DrawContext const & context { GetDrawContext() };

	if ( m_rectSelection.IsNotEmpty() )
		context.DrawTranspRect(m_rectSelection, NNetColors::SELECTION_RECT);

	m_scale.DisplayStaticScale();

	if ( context.GetPixelSize() <= 5._MicroMeter )
	{
		DrawExteriorInRect( pixRect );
		if ( ArrowsVisible() )
			DrawArrowsInRect(pixRect, m_arrowSize);
	}

	DrawInteriorInRect  (pixRect, [&](Nob const & s) { return s.IsPipe() && ! s.IsSelected(); }); 
	DrawInteriorInRect  (pixRect, [&](Nob const & s) { return s.IsPipe() &&   s.IsSelected(); }); 
	DrawInteriorInRect  (pixRect, [&](Nob const & s) { return s.IsBaseKnot (); }); // draw BaseKnots OVER Pipes
	DrawInteriorInRect  (pixRect, [&](Nob const & s) { return s.IsIoConnector(); }); 
	DrawNeuronTextInRect(pixRect);

	if ( IsDefined(m_nobTarget) ) // draw target nob again to be sure that it is visible
	{
		tHighlight type { m_bTargetFits ? tHighlight::targetFit : tHighlight::targetNoFit };
		m_pNMRI->DrawExterior(m_nobTarget, context, type);
		m_pNMRI->DrawInterior(m_nobTarget, context, type);
	}

	if ( IsDefined(m_nobHighlighted) )  // draw highlighted nob again to be sure that it is in foreground
	{
		m_pNMRI->DrawExterior  (m_nobHighlighted, context, tHighlight::highlighted);
		m_pNMRI->DrawInterior  (m_nobHighlighted, context, tHighlight::highlighted);
		m_pNMRI->DrawNeuronText(m_nobHighlighted, context);
	}

	DrawSensors();
	DrawBeacon();
}

void MainWindow::setHighlightedNob( MicroMeterPnt const & umCrsrPos )
{
	NobId const idHighlight { m_pNMRI->FindNobAt(umCrsrPos) };
	if ( idHighlight != m_nobHighlighted )
	{
		m_nobHighlighted = idHighlight; 
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
		if ( ! m_pWinCommands->MakeIoConnector(*this) )
			SendCommand2Application(IDX_PLAY_SOUND, reinterpret_cast<LPARAM>(TEXT("NOT_POSSIBLE_SOUND")));
		break;

	default:
		break;
	}

	return NNetWindow::OnCommand( wParam, lParam, pixPoint );
}
