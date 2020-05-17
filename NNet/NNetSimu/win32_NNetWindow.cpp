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
#include "win32_WorkThreadInterface.h"
#include "win32_NNetWindow.h"

using std::function;

void NNetWindow::InitClass
(        
	WorkThreadInterface * const pWorkThreadInterface,
	ActionTimer         * const pActionTimer
)
{
	ModelWindow::InitClass( pActionTimer );
	m_pWorkThreadInterface = pWorkThreadInterface;
}

NNetWindow::NNetWindow( ) :
	ModelWindow( )
{ }

void NNetWindow::Start
( 
	HWND                 const hwndApp, 
	DWORD                const dwStyle,
	NNetController     * const pController,
	NNetModelInterface * const pModelInterface,
	DrawModel          * const pDrawModel,
	Observable         * const pCursorObservable
)
{
	m_pDrawModel = pDrawModel;
	HWND hwnd = StartBaseWindow
	( 
		hwndApp,
		CS_OWNDC | CS_DBLCLKS,
		L"ClassNNetWindow",
		dwStyle,
		nullptr,
		nullptr
	);
	m_context.Start( hwnd );
	m_pController          = pController;
	m_pModelInterface      = pModelInterface;
	m_pCursorPosObservable = pCursorObservable;
	m_pAnimationThread     = new AnimationThread( );
}

void NNetWindow::Stop( )
{
	m_context.Stop();
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pDrawModel           = nullptr;
	m_pAnimationThread     = nullptr;
	m_pCursorPosObservable = nullptr;
}

void NNetWindow::Zoom( MicroMeter const newSize )
{
	if ( m_context.ZoomKeepCrsrPos( GetRelativeCrsrPosition(), newSize ) )
		Notify( true );     // cause immediate repaint
}

void NNetWindow::ZoomStep( bool const bZoomIn )
{
	Zoom( m_context.GetCoordC().ComputeNewPixelSize( bZoomIn ) );
}

void NNetWindow::NNetMove( PixelPoint const & pixDelta ) 
{ 
	m_context.GetCoord().Move( pixDelta ); 
	Notify( true );     // cause immediate repaint
}

void NNetWindow::NNetMove( MicroMeterPoint const & umDelta )	
{ 
	NNetMove( m_context.GetCoord().Convert2PixelSize( umDelta ) ); 
}

long NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu, PixelPoint const ptPos )
{
	UINT static const STD_FLAGS { MF_BYPOSITION | MF_STRING };

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
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_STOP_ON_TRIGGER,       L"Stop on trigger on/off" );
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

	return m_shapeHighlighted.GetValue(); // will be forwarded to HandleContextMenuCommand
}

void NNetWindow::HandleContextMenuCommand
( 
	UINT       const   uiCmdId,  // command selected by user  
	long       const   lParam,   // comes from AddContextMenuEntries
	PixelPoint const & pixPoint  // cursor position when comtext menue was initiated
)
{
	ShapeId shapeId( lParam );  
}

bool NNetWindow::ChangePulseRate( bool const bDirection )
{
	static fHertz const INCREMENT { 0.01_fHertz };
	fHertz const fOldValue { m_pModelInterface->GetPulseFreq( m_shapeHighlighted ) };
	if ( fOldValue.IsNull() )
		return false;
	fHertz const fNewValue = fOldValue + ( bDirection ? INCREMENT : -INCREMENT );
	m_pWorkThreadInterface->PostSetPulseRate( m_shapeHighlighted, fNewValue );
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
		m_pWorkThreadInterface->PostSetPulseRate( id, fNewValue );
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

	m_pWorkThreadInterface->PostSetTriggerSound
	( 
		id,
	    dialog.IsTriggerSoundActive(),
	    dialog.GetFrequency(),
	    dialog.GetDuration ()
	);
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint      const ptCrsr    { GetCrsrPosFromLparam( lParam ) };  // relative to client area
	MicroMeterPoint const umCrsrPos { m_context.GetCoord().Convert2MicroMeterPointPos( ptCrsr   ) };
	MicroMeterPoint const umLastPos { m_context.GetCoord().Convert2MicroMeterPointPos( m_ptLast ) };

	m_pCursorPosObservable->NotifyAll( false );

	if ( wParam & MK_RBUTTON )         // Right mouse button: selection
	{
		if ( m_ptLast.IsNotNull() )    // last cursor pos stored in m_ptLast
		{
			m_rectSelection = MicroMeterRect( umCrsrPos, umLastPos );
			m_pWorkThreadInterface->PostSelectShapesInRect( m_rectSelection );
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
			if ( IsDefined( m_shapeHighlighted ) )
			{
//				setSuperHighlightedShape( umCrsrPos );
				setSuperHighlightedShape( m_pModelInterface->GetShapePos( m_shapeHighlighted ) );
				m_pWorkThreadInterface->PostMoveShape( m_shapeHighlighted, umCrsrPos - umLastPos );
			}
			else if ( m_pModelInterface->AnyShapesSelected( ) )   // move selected shapes 
			{
				m_pWorkThreadInterface->PostMoveSelection( umCrsrPos - umLastPos );
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

void NNetWindow::setHighlightedShape( MicroMeterPoint const & umCrsrPos )
{
	ShapeId const idHighlight { m_pModelInterface->FindShapeAt( umCrsrPos, ShapeCritAlwaysTrue ) };
	if ( idHighlight != m_shapeHighlighted )
	{
		m_shapeHighlighted = idHighlight; 
		Notify( true );     // cause immediate repaint
	}
}

void NNetWindow::setSuperHighlightedShape( MicroMeterPoint const & umCrsrPos )
{
	if ( m_pModelInterface->IsOfType<BaseKnot>( m_shapeHighlighted ) )
	{
		m_shapeSuperHighlighted = m_pModelInterface->FindShapeAt
		( 
			umCrsrPos,
			[&]( Shape const & shape ) { return m_pModelInterface->ConnectsTo( m_shapeHighlighted, shape.GetId() ); } 
		);
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

	m_pDrawModel->DrawInteriorInRect( pixRect, m_context, [&](Shape const & s) { return s.IsPipe    (); } );
	m_pDrawModel->DrawInteriorInRect( pixRect, m_context, [&](Shape const & s) { return s.IsBaseKnot(); } );

	if ( m_shapeSuperHighlighted.IsNotNull() ) // draw super highlighted shape again to be sure that it is in foreground
	{
		m_pModelInterface->DrawExterior( m_shapeSuperHighlighted, m_context, tHighlightType::superHighlighted );
		m_pModelInterface->DrawInterior( m_shapeSuperHighlighted, m_context );
	}

	if ( m_shapeHighlighted.IsNotNull() )  // draw highlighted shape again to be sure that it is in foreground
	{
		m_pModelInterface->DrawExterior( m_shapeHighlighted, m_context, tHighlightType::highlighted );
		m_pModelInterface->DrawInterior( m_shapeHighlighted, m_context );
	}

	m_context.ShowScale( GetClientWindowHeight() );

	if ( m_context.GetPixelSize() <= 2.5_MicroMeter )
		m_pDrawModel->DrawNeuronTextInRect( pixRect, m_context );
}

void NNetWindow::CenterModel( bool const bSmooth )
{
	CenterAndZoomRect( m_pModelInterface->GetEnclosingRect( ), 1.2f, bSmooth ); // give 20% more space (looks better)
}

void NNetWindow::AnalysisFinished( )
{
	MicroMeterRect rect { ModelAnalyzer::GetEnclosingRect() };
	if ( rect.IsNotEmpty() )
		CenterAndZoomRect( rect, 2.0f, true );
}

void NNetWindow::CenterAndZoomRect( MicroMeterRect const & umRect, float const fRatioFactor, bool const bSmooth )
{
	MicroMeterRect  const umRectScaled      { umRect.Scale( NEURON_RADIUS ) }; // give some more space to include complete shapes 
	float           const fVerticalRatio    { umRectScaled.GetHeight() / m_context.GetCoordC().Convert2MicroMeter( GetClientWindowHeight() ) };
	float           const fHorizontalRatio  { umRectScaled.GetWidth () / m_context.GetCoordC().Convert2MicroMeter( GetClientWindowWidth() ) };
	float           const fMaxRatio         { max( fVerticalRatio, fHorizontalRatio ) };
	float           const fDesiredRatio     { fMaxRatio * fRatioFactor };
	PixelPoint      const pixPointCenter    { GetClRectCenter( ) };
	fPixelPoint     const fpCenter          { m_context.GetCoordC().Convert2fPixelPoint( pixPointCenter ) };
	MicroMeter      const umPixelSizeTarget { m_context.GetCoordC().LimitPixelSize( m_context.GetCoordC().GetPixelSize() * fDesiredRatio ) };
	MicroMeterPoint const umPntCenterTarget { umRectScaled.GetCenter() };
	if ( bSmooth )
	{
		m_umPixelSizeStart = m_context.GetCoordC().GetPixelSize();                                // actual pixel size 
		m_umPntCenterStart = m_context.GetCoordC().Convert2MicroMeterPointPos( pixPointCenter );  // actual center 
		m_umPixelSizeDelta = umPixelSizeTarget - m_umPixelSizeStart;
		m_umPntCenterDelta = umPntCenterTarget - m_umPntCenterStart;
		m_smoothMove.Reset();
		m_bFocusMode = true;
	}
	else
	{
		m_context.GetCoord().Zoom( umPixelSizeTarget );
		m_context.GetCoord().Center( umPntCenterTarget, fpCenter );
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
		fPixelPoint const fpCenter { m_context.GetCoordC().Convert2fPixelPoint( GetClRectCenter( ) ) };
		m_context.GetCoord().Zoom  ( m_umPixelSizeStart + m_umPixelSizeDelta * fPos );
		m_context.GetCoord().Center( m_umPntCenterStart + m_umPntCenterDelta * fPos, fpCenter );
	}

	Notify( true );     // cause immediate repaint
}

void NNetWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );
		if ( m_context.StartFrame( hDC ) )
		{
			m_pModelInterface->LockModelShared();
			doPaint( );
			m_pModelInterface->UnlockModelShared();
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
		m_pWorkThreadInterface->PostSelectShape( m_shapeHighlighted, tBoolOp::opToggle );
}

void NNetWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{
	int        iDelta     = GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA;
	bool const bDirection = ( iDelta > 0 );
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
	bool    const keyDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	HCURSOR const hCrsr   = keyDown ? m_hCrsrMove : m_hCrsrArrow;
	SetCursor( hCrsr );
}

MicroMeterRect const NNetWindow::GetViewRect( )
{
	PixelRect const pixRectView = GetClPixelRect( );
	return m_context.GetCoordC().Convert2MicroMeterRect( pixRectView );
}

MicroMeterPoint NNetWindow::PixelPoint2MicroMeterPoint( PixelPoint const pixPoint ) const
{
	return m_context.GetCoordC().Convert2MicroMeterPointPos( pixPoint );  // PixelPoint2MicroMeterPoint belongs to NNetWindow
}                                                           // because every NNetWindow needs its own coordinate system

LPARAM NNetWindow::pixelPoint2LPARAM( PixelPoint const pixPoint ) const
{
	return Util::Pack2UINT64( m_context.GetCoordC().Convert2MicroMeterPointPos( pixPoint ) );
}

LPARAM NNetWindow::crsPos2LPARAM( ) const 
{
	return pixelPoint2LPARAM( GetRelativeCrsrPosition() );
}

bool NNetWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
	MicroMeterPoint const umPoint { m_context.GetCoordC().Convert2MicroMeterPointPos( pixPoint ) };
	if ( m_pController->HandleCommand( wParam, lParam, umPoint ) )
		return true;
	else 
		return ModelWindow::OnCommand( wParam, lParam, pixPoint );
}

bool NNetWindow::inObservedClientRect( LPARAM const lParam )
{
	return true;  // Is cursor position in observed client rect?
}

void NNetWindow::ShowDirectionArrows( bool const bShow )
{
	m_pAnimationThread->SetTarget( bShow ? Pipe::STD_ARROW_SIZE : 0.0_MicroMeter );
}

