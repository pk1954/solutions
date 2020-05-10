// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "Resource.h"
#include "MoreTypes.h"
#include "Segment.h"
#include "scale.h"
#include "Pipe.h"
#include "InputNeuron.h"
#include "PixelTypes.h"
#include "Analyzer.h"
#include "AnimationThread.h"
#include "NNetParameters.h"
#include "NNetModelInterface.h"
#include "NNetColors.h"
#include "DrawModel.h"
#include "win32_sound.h"
#include "win32_tooltip.h"
#include "win32_triggerSoundDlg.h"
#include "win32_stdDialogBox.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWindow.h"

using std::function;

static MicroMeter const STD_FONT_SIZE { 20._MicroMeter };

void NNetWindow::InitClass
(        
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	ActionTimer             * const pActionTimer
)
{
	ModelWindow::InitClass( pActionTimer );
	m_pNNetWorkThreadInterface = pNNetWorkThreadInterface;
}

NNetWindow::NNetWindow( ) :
	ModelWindow( )
{ }

void NNetWindow::Start
( 
	HWND                 const hwndApp, 
	DWORD                const dwStyle,
	function<bool()>     const visibilityCriterion,
	NNetModelInterface * const pModelInterface,
	DrawModel          * const pDrawModel,
	Observable         * const pCursorObservable,
	bool                 const bFixed
)
{
	m_pDrawModel = pDrawModel;
	m_bFixed = bFixed;
	HWND hwnd = StartBaseWindow
	( 
		hwndApp,
		CS_OWNDC | CS_DBLCLKS,
		L"ClassNNetWindow",
		dwStyle,
		nullptr,
		visibilityCriterion
	);
	m_context.Start( hwnd );
	m_context.SetStdFontSize( STD_FONT_SIZE );
	m_pModelInterface = pModelInterface;
	m_pAnimationThread = new AnimationThread( );
	m_pCursorPosObservable = pCursorObservable;
	ShowRefreshRateDlg( false );
}

void NNetWindow::Stop( )
{
	m_context.Stop();
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
}

void NNetWindow::Zoom( bool const bZoomIn  )
{
	ZoomKeepCrsrPos( m_context.GetCoordC().ComputeNewPixelSize( bZoomIn ) );
}

void NNetWindow::ZoomKeepCrsrPos( MicroMeter const newSize )
{
	PixelPoint      const pixPointCenter  { GetRelativeCrsrPosition() };
	fPixelPoint     const fPixPointCenter { convert2fPixelPoint( pixPointCenter ) };
	MicroMeterPoint const umPointcenter   { m_context.GetCoordC().convert2MicroMeterPointPos( fPixPointCenter ) };
	if ( m_context.GetCoord().Zoom( newSize ) ) 
	{
		m_context.GetCoord().Center( umPointcenter, fPixPointCenter ); 
		Notify( TRUE );     // cause immediate repaint
		m_context.SetStdFontSize( STD_FONT_SIZE );
	}
	else
	{
		MessageBeep( MB_ICONWARNING );
	}
}

long NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu, PixelPoint const ptPos )
{
	UINT static const STD_FLAGS { MF_BYPOSITION | MF_STRING };

	m_ptCommandPosition = ptPos;

	ShapeType type { m_pModelInterface->GetShapeType( m_shapeHighlighted ) };

	if ( m_pModelInterface->AnyShapesSelected( ) )
	{
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_DESELECT_ALL,     L"Deselect all" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_COPY_SELECTION,   L"Copy selection" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_MARK_SELECTION,   L"Mark selection" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_UNMARK_SELECTION, L"Unmark selection" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_REMOVE_SELECTION, L"Remove selected objects" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_REMOVE_BEEPERS,   L"Remove selected trigger sounds" );
	}
	else switch ( type.GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		if ( ! m_pModelInterface->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_RATE,            L"Pulse rate" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE,          L"Remove" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_DISCONNECT,            L"Disconnect" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_CONVERT2NEURON,        L"Convert into normal neuron" );
		break;

	case ShapeType::Value::neuron:
		if ( ! m_pModelInterface->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INCOMING2KNOT,     L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE,          L"Remove" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_DISCONNECT,            L"Disconnect" );
		if ( ! m_pModelInterface->HasIncoming( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_CONVERT2INPUT_NEURON, L"Convert into input neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_TRIGGER_SOUND_DLG,     L"Trigger sound" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_SELECT_SUBTREE,        L"Select subtree" );
		break;

	case ShapeType::Value::knot:  
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INCOMING2KNOT, L"Add incoming dendrite" );
		if ( 
				(! m_pModelInterface->HasOutgoing( m_shapeHighlighted )) || 
				(
					! m_pModelInterface->HasIncoming( m_shapeHighlighted ) && 
					( m_pModelInterface->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 )  
				) 
			)
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_APPEND_NEURON, L"Add neuron" );
		if ( 
				( m_pModelInterface->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 ) && 
				(! m_pModelInterface->HasIncoming( m_shapeHighlighted )) 
			)
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_APPEND_INPUT_NEURON, L"Add input neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_DISCONNECT, L"Disconnect" );
		break;

	case ShapeType::Value::pipe:
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2PIPE, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INCOMING2PIPE, L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_INSERT_NEURON,     L"Insert neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE,      L"Remove" );
		if ( Pipe::GetArrowSize( ) > 0.0_MicroMeter )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ARROWS_OFF,    L"Arrows off" );
		else
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ARROWS_ON,     L"Arrows on" );
		break;

	case ShapeType::Value::undefined: // no shape selected, cursor on background
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_NEW_NEURON,         L"New neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_NEW_INPUT_NEURON,   L"New input neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_SELECT_ALL_BEEPERS, L"Select all neurons with trigger sound" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_REMOVE_BEEPERS,     L"Remove all trigger sounds" );
		break;

	default:
		assert( false );
	}

	if ( IsDefined( m_shapeHighlighted ) )
	{
		if ( m_pModelInterface->IsSelected( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, STD_FLAGS, IDM_DESELECT_SHAPE, L"Deselect" );
		else
			AppendMenu( hPopupMenu, STD_FLAGS, IDM_SELECT_SHAPE, L"Select" );
	}

	return m_shapeHighlighted.GetValue();
}

bool NNetWindow::ChangePulseRate( bool const bDirection )
{
	static fHertz const INCREMENT { 0.01_fHertz };
	fHertz const fOldValue { m_pModelInterface->GetPulseFreq( m_shapeHighlighted ) };
	if ( fOldValue.IsNull() )
		return false;
	fHertz const fNewValue = fOldValue + ( bDirection ? INCREMENT : -INCREMENT );
	m_pNNetWorkThreadInterface->PostSetPulseRate( m_shapeHighlighted, fNewValue );
	return true;
}

void NNetWindow::PulseRateDlg( ShapeId const id )
{
	fHertz  const fOldValue { m_pModelInterface->GetPulseFreq( id ) };
	if ( fOldValue.IsNull() )
		return;
	wstring const header    { GetParameterName ( tParameter::pulseRate ) }; 
	wstring const unit      { GetParameterUnit ( tParameter::pulseRate ) }; 
	fHertz  const fNewValue { StdDialogBox::Show( GetWindowHandle(), fOldValue.GetValue(), header, unit ) };
	if ( fNewValue != fOldValue )
		m_pNNetWorkThreadInterface->PostSetPulseRate( id, fNewValue );
}

void NNetWindow::TriggerSoundDlg( ShapeId const id )
{
	ShapeType const type { m_pModelInterface->GetShapeType(id) };
	if ( ! type.IsAnyNeuronType() )
		return;

	TriggerSoundDialog dialog
	( 
		m_pModelInterface->HasTriggerSound( id ), 
		m_pModelInterface->GetTriggerSoundFrequency( id ), 
		m_pModelInterface->GetTriggerSoundDuration( id ) 
	);

	dialog.Show( GetWindowHandle() );

	m_pNNetWorkThreadInterface->PostSetTriggerSound
	( 
		id,
	    dialog.IsTriggerSoundActive(),
	    dialog.GetFrequency(),
	    dialog.GetDuration ()
	);
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	if ( m_bFixed )
		return;

	PixelPoint const ptCrsr { GetCrsrPosFromLparam( lParam ) };  // relative to client area

	m_pCursorPosObservable->NotifyAll( false );

	if ( wParam & MK_RBUTTON )         // Right mouse button: selection
	{
		if ( m_ptLast.IsNotNull() )    // last cursor pos stored in m_ptLast
		{
			m_rectSelection = PixelRect( ptCrsr, m_ptLast );
			MicroMeterRect umRect { m_context.GetCoordC().convert2MicroMeterRect( m_rectSelection ) };
			m_pNNetWorkThreadInterface->PostSelectShapesInRect( umRect );
		}
		else                           // first time here after RBUTTON pressed
		{
			m_ptLast = ptCrsr;         // store current cursor pos
		}
	}
	else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
	{
		PixelPoint const delta { ptCrsr - m_ptLast };
		if ( m_ptLast.IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			m_shapeSuperHighlighted = NO_SHAPE;
			if ( IsDefined( m_shapeHighlighted ) )
			{
				MicroMeterPoint umDelta { m_context.GetCoordC().convert2MicroMeterPointSize( delta ) };
				m_pNNetWorkThreadInterface->PostMoveShape( m_shapeHighlighted, umDelta );
				if ( m_pModelInterface->IsOfType<BaseKnot>( m_shapeHighlighted ) )
				{
					m_shapeSuperHighlighted = m_pDrawModel->FindShapeAt
					( 
						ptCrsr,
						m_context,
						[&]( Shape const & shape ) { return m_pModelInterface->ConnectsTo( m_shapeHighlighted, shape.GetId() ); } 
					);
				}
			}
			else if ( m_pModelInterface->AnyShapesSelected( ) )   // move selected shapes 
			{
				MicroMeterPoint umDelta { m_context.GetCoord().convert2MicroMeterPointSize( delta ) };
				m_pNNetWorkThreadInterface->PostMoveSelection( umDelta );
			}
			else if ( m_bMoveAllowed ) // move view by manipulating coordinate system 
			{
				m_context.GetCoord().Move( delta );
			}
		}
		m_ptLast = ptCrsr;
		Notify( TRUE );     // cause immediate repaint
	}
	else  // no mouse button pressed
	{                         
		ShapeId const idHighlight { m_pDrawModel->FindShapeAt( ptCrsr, m_context, ShapeCritAlwaysTrue ) };
		if ( idHighlight != m_shapeHighlighted )
		{
			m_shapeHighlighted = idHighlight; 
			Notify( TRUE );     // cause immediate repaint
		}
		m_ptLast = PP_NULL;   // make m_ptLast invalid
		 // no refresh! It would cause repaint for every mouse move 
	}
}

tHighlightType const NNetWindow::GetHighlightType( Shape const & shape ) const
{
	return ( shape.GetId() == m_shapeSuperHighlighted )
		   ? tHighlightType::superHighlighted
		   : ( shape.GetId() == m_shapeHighlighted )
			 ? tHighlightType::highlighted
			 : tHighlightType::normal;
}

void NNetWindow::doPaint( ) 
{
	PixelRect const pixRect { GetClPixelRect( ) };

	if ( m_rectSelection.IsNotEmpty( ) )
		m_context.DrawTranspRect( m_rectSelection, NNetColors::SELECTION_RECT );

	if ( m_context.GetPixelSize() <= 5._MicroMeter )
		m_pDrawModel->DrawExteriorInRect( pixRect, m_context );

	m_pDrawModel->DrawInteriorInRect( pixRect, m_context, [&](Shape const & s){ return s.IsPipe    (); } );
	m_pDrawModel->DrawInteriorInRect( pixRect, m_context, [&](Shape const & s){ return s.IsBaseKnot(); } );

	// draw highlighted shape again to be sure that it is in foreground
	m_pModelInterface->DrawExterior( m_shapeHighlighted, m_context, tHighlightType::highlighted );
	m_pModelInterface->DrawInterior( m_shapeHighlighted, m_context );

	m_context.ShowScale( GetClientWindowHeight() );

	if ( m_context.GetPixelSize() <= 2.5_MicroMeter )
		m_pDrawModel->DrawNeuronTextInRect( pixRect, m_context );
}

void NNetWindow::CenterModel( bool const bSmooth )
{
	centerAndZoomRect( m_pModelInterface->GetEnclosingRect( ), 1.2f, bSmooth );
}

void NNetWindow::AnalysisFinished( )
{
	centerAndZoomRect( ModelAnalyzer::GetEnclosingRect(), 2.0f, true );
}

void NNetWindow::centerAndZoomRect( MicroMeterRect const rect, float const fRatioFactor, bool const bSmooth )
{
	PixelPoint      const pixPointCenter    { GetClRectCenter( ) };
	float           const fVerticalRatio    { rect.GetHeight() / m_context.GetCoordC().convert2MicroMeter( GetClientWindowHeight() ) };
	float           const fHorizontalRatio  { rect.GetWidth () / m_context.GetCoordC().convert2MicroMeter( GetClientWindowWidth() ) };
	float           const fMaxRatio         { max( fVerticalRatio, fHorizontalRatio ) };
	float           const fDesiredRatio     { fMaxRatio * fRatioFactor };
	fPixelPoint     const fpCenter          { m_context.GetCoordC().convert2fPixelPoint( GetClRectCenter( ) ) };
	MicroMeter      const umPixelSizeTarget { m_context.GetCoordC().LimitPixelSize( m_context.GetCoordC().GetPixelSize() * fDesiredRatio ) };
	MicroMeterPoint const umPntCenterTarget { rect.GetCenter() };
	if ( bSmooth )
	{
		m_umPixelSizeStart = m_context.GetCoordC().GetPixelSize();                                // actual pixel size 
		m_umPntCenterStart = m_context.GetCoordC().convert2MicroMeterPointPos( pixPointCenter );  // actual center 
		m_umPixelSizeDelta = umPixelSizeTarget - m_umPixelSizeStart;
		m_umPntCenterDelta = umPntCenterTarget - m_umPntCenterStart;
		m_smoothMove.Reset();
		m_bFocusMode = true;
	}
	else
	{
		m_context.GetCoord().Zoom( umPixelSizeTarget );
		m_context.Center( pixPointCenter );
	}
}

void NNetWindow::smoothStep( ) 
{
	float fPos            { m_smoothMove.Next() };
	bool  fTargetsReached { fPos >= SmoothMoveFp::END_POINT };

	if ( fTargetsReached )
	{
		m_bFocusMode = false;
	}
	else
	{
		fPixelPoint const fpCenter { m_context.GetCoordC().convert2fPixelPoint( GetClRectCenter( ) ) };
		m_context.GetCoord().Zoom  ( m_umPixelSizeStart + m_umPixelSizeDelta * fPos );
		m_context.GetCoord().Center( m_umPntCenterStart + m_umPntCenterDelta * fPos, fpCenter );
	}

	Notify( TRUE );     // cause immediate repaint
}

void NNetWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );
		if ( m_context.StartFrame( hDC ) )
		{
			doPaint( );
			m_context.EndFrame( );
		}
		EndPaint( &ps );
	}

	if ( m_bFocusMode )
		smoothStep( );
}

void NNetWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_context.Resize( width, height );
}

void NNetWindow::OnLeftButtonDblClick( WPARAM const wParam, LPARAM const lParam )
{
	if ( IsDefined( m_shapeHighlighted ) )
		m_pNNetWorkThreadInterface->PostSelectShape( m_shapeHighlighted, tBoolOp::opToggle );
}

void NNetWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{
	if ( m_bFixed )
		return;

	int        iDelta     = GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA;
	BOOL const bDirection = ( iDelta > 0 );
	MicroMeter newSize;

	iDelta = abs( iDelta );

	while ( --iDelta >= 0 )
	{
		newSize = m_context.GetCoordC().ComputeNewPixelSize( bDirection );
	}

	PostCommand2Application( IDM_SET_ZOOM, (LPARAM &)newSize.GetValue() ); 
}

void NNetWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	if ( IsDefined( m_shapeHighlighted ) && IsDefined( m_shapeSuperHighlighted ) )
	{ 
		PostCommand2Application
		( 
			IDD_CONNECT, 
			Util::Pack2UINT64( m_shapeHighlighted.GetValue(), m_shapeSuperHighlighted.GetValue() )
		);
		m_shapeSuperHighlighted = NO_SHAPE;
	}
	//	ReleaseCapture( );
}

bool NNetWindow::OnRButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	bool bMadeSelection { m_rectSelection.IsNotEmpty() };
	if ( bMadeSelection )
		m_rectSelection.SetZero();
	return bMadeSelection;
}

bool NNetWindow::OnRButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	return false;
}

void NNetWindow::OnLButtonDown( WPARAM const wParam, LPARAM const lParam )
{
}

void NNetWindow::OnSetCursor( WPARAM const wParam, LPARAM const lParam )
{
	BOOL    const keyDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	HCURSOR const hCrsr   = keyDown ? m_hCrsrMove : m_hCrsrArrow;
	SetCursor( hCrsr );
}

MicroMeterPoint NNetWindow::PixelPoint2MicroMeterPoint( PixelPoint const pixPoint ) const
{
	return m_context.GetCoordC().convert2MicroMeterPointPos( pixPoint );  // PixelPoint2MicroMeterPoint belongs to NNetWindow
}                                                           // because every NNetWindow needs its own coordinate system

LPARAM NNetWindow::pixelPoint2LPARAM( PixelPoint const pixPoint ) const
{
	return Util::Pack2UINT64( m_context.GetCoordC().convert2MicroMeterPointPos( pixPoint ) );
}

LPARAM NNetWindow::crsPos2LPARAM( ) const 
{
	return pixelPoint2LPARAM( GetRelativeCrsrPosition() );
}

BOOL NNetWindow::OnCommand( WPARAM const wParam, LPARAM const lParam )
{
	SendCommand2Application( wParam, pixelPoint2LPARAM( m_ptCommandPosition ) );
	return FALSE;
}

BOOL NNetWindow::inObservedClientRect( LPARAM const lParam )
{
	return TRUE;  // Is cursor position in observed client rect?
}

void NNetWindow::ShowDirectionArrows( bool const bShow )
{
	m_pAnimationThread->SetTarget( bShow ? Pipe::STD_ARROW_SIZE : 0.0_MicroMeter );
}
