// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "Resource.h"
#include "MoreTypes.h"
#include "Segment.h"
#include "Pipe.h"
#include "InputNeuron.h"
#include "PixelTypes.h"
#include "Analyzer.h"
#include "AnimationThread.h"
#include "tHighlightType.h"
#include "PixelCoordsFp.h"
#include "Direct2D.h"
#include "NNetParameters.h"
#include "win32_fatalError.h"
#include "win32_sound.h"
#include "win32_tooltip.h"
#include "win32_scale.h"
#include "win32_triggerSoundDlg.h"
#include "win32_stdDialogBox.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWindow.h"

using std::function;

NNetModel               * NNetWindow::m_pModel                   = nullptr;
NNetWorkThreadInterface * NNetWindow::m_pNNetWorkThreadInterface = nullptr;

void NNetWindow::InitClass
(        
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	ActionTimer             * const pActionTimer
)
{
	ModelWindow::InitClass( pActionTimer );
	m_pNNetWorkThreadInterface = pNNetWorkThreadInterface;
}

void NNetWindow::setStdFontSize( )
{
	m_D2d_driver.SetStdFontSize( m_coord.convert2fPixel( 20._MicroMeter ).GetValue() );
}

NNetWindow::NNetWindow( ) :
	ModelWindow( )
{ }

void NNetWindow::Start
( 
	HWND             const hwndApp, 
	DWORD            const dwStyle,
	function<bool()> const visibilityCriterion,
	NNetModel      * const pModel,
	Observable     * const pCursorObservable
)
{
	HWND hwnd = StartBaseWindow
	( 
		hwndApp,
		CS_OWNDC | CS_DBLCLKS,
		L"ClassNNetWindow",
		dwStyle,
		nullptr,
		visibilityCriterion
	);
	m_D2d_driver.Initialize( hwnd );
	setStdFontSize( );
	Shape::SetGraphics( & m_D2d_driver );
	m_pModel = pModel;
	m_pScale = new Scale( & m_D2d_driver, & m_coord );
	m_pAnimationThread = new AnimationThread( );
	m_pCursorPosObservable = pCursorObservable;
}

void NNetWindow::Stop( )
{
	m_D2d_driver.ShutDown();
	delete m_pScale;
	m_pScale = nullptr;
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pNNetWorkThreadInterface = nullptr;
}

void NNetWindow::Zoom( bool const bZoomIn  )
{
	ZoomKeepCrsrPos( m_coord.ComputeNewPixelSize( bZoomIn ) );
}

void NNetWindow::ZoomKeepCrsrPos( MicroMeter const newSize )
{
	PixelPoint      const pixPointCenter  { GetRelativeCrsrPosition() };
	fPixelPoint     const fPixPointCenter { convert2fPixelPoint( pixPointCenter ) };
	MicroMeterPoint const umPointcenter   { m_coord.convert2MicroMeterPointPos( fPixPointCenter ) };
	if ( m_coord.Zoom( newSize ) ) 
	{
		m_coord.Center( umPointcenter, fPixPointCenter ); 
		Notify( TRUE );     // cause immediate repaint
		setStdFontSize( );
	}
	else
	{
		MessageBeep( MB_ICONWARNING );
	}
}

void NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu, PixelPoint const ptPos )
{
	UINT const STD_FLAGS { MF_BYPOSITION | MF_STRING };

	m_ptCommandPosition = ptPos;

	if( m_pModel->AnyShapesSelected() )
	{
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_COPY_SELECTION,   L"Copy" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDM_DELETE_SELECTION, L"Delete" );
	}
	else switch ( m_pModel->GetShapeType( m_shapeHighlighted ).GetValue() )
	{
	case ShapeType::Value::inputNeuron:
		if ( ! m_pModel->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_RATE,            L"Pulse rate" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE,          L"Remove" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_DISCONNECT,            L"Disconnect" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_CONVERT2NEURON,        L"Convert into normal neuron" );
		break;

	case ShapeType::Value::neuron:
		if ( ! m_pModel->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INCOMING2KNOT,     L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE,          L"Remove" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_DISCONNECT,            L"Disconnect" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_TRIGGER_SOUND_DLG,     L"Trigger sound" );
		break;

	case ShapeType::Value::knot:  
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INCOMING2KNOT, L"Add incoming dendrite" );
		if ( 
			   (! m_pModel->HasOutgoing( m_shapeHighlighted )) || 
			   (
				  ! m_pModel->HasIncoming( m_shapeHighlighted ) && 
				  ( m_pModel->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 )  
			   ) 
		   )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_APPEND_NEURON,       L"Add neuron" );
		if ( 
			  ( m_pModel->GetNrOfOutgoingConnections( m_shapeHighlighted ) <= 1 ) && 
			  (! m_pModel->HasIncoming( m_shapeHighlighted )) 
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

	case ShapeType::Value::undefined: // noshape selected
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_NEW_NEURON,       L"New neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_NEW_INPUT_NEURON, L"New input neuron" );
		break;

	default:
		assert( false );
	}
}

bool NNetWindow::ChangePulseRate( ShapeId const id, bool const bDirection )
{
	static fHertz const INCREMENT { 0.01_fHertz };
	InputNeuron const * pInputNeuron { m_pModel->GetShapeConstPtr<InputNeuron const *>(id) };
	if ( pInputNeuron == nullptr )
		return false;
	fHertz const fOldValue { pInputNeuron->GetPulseFrequency( ) };
	fHertz const fNewValue = fOldValue + ( bDirection ? INCREMENT : -INCREMENT );
	m_pNNetWorkThreadInterface->PostSetPulseRate( id, fNewValue.GetValue() );
	return true;
}

void NNetWindow::PulseRateDlg( ShapeId const id )
{
	InputNeuron const * pInputNeuron { m_pModel->GetShapeConstPtr<InputNeuron const *>(id) };
	if ( pInputNeuron == nullptr )
		return;
	float   const fOldValue { m_pModel->GetPulseRate( pInputNeuron ) };
	wstring const header    { GetParameterName ( tParameter::pulseRate ) }; 
	wstring const unit      { GetParameterUnit ( tParameter::pulseRate ) }; 
	float   const fNewValue { StdDialogBox::Show( GetWindowHandle(), fOldValue, header, unit ) };
	if ( fNewValue != fOldValue )
		m_pNNetWorkThreadInterface->PostSetPulseRate( id, fNewValue );
}

void NNetWindow::TriggerSoundDlg( ShapeId const id )
{
	Neuron * pNeuron { m_pModel->GetShapePtr<Neuron *>(id) };
	if ( pNeuron == nullptr )
		return;

	TriggerSoundDialog dialog( pNeuron->HasTriggerSound(), pNeuron->GetTriggerSoundFrequency(), pNeuron->GetTriggerSoundDuration() );

	dialog.Show( GetWindowHandle() );

	pNeuron->SetTriggerSoundOn       ( dialog.IsTriggerSoundActive() );
	pNeuron->SetTriggerSoundFrequency( dialog.GetFrequency() );
	pNeuron->SetTriggerSoundDuration ( dialog.GetDuration () );
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint      const ptCrsr    { GetCrsrPosFromLparam( lParam ) };  // relative to client area
	MicroMeterPoint const umCrsrPos { m_coord.convert2MicroMeterPointPos( ptCrsr ) };
	MicroMeterPoint const umOldPos  { m_coord.convert2MicroMeterPointPos( m_ptLast ) };

	m_pCursorPosObservable->NotifyAll( false );

	if ( wParam & MK_RBUTTON )         // Right mouse button: selection
	{
		if ( m_ptLast.IsNotNull() )    // last cursor pos stored in m_ptLast
		{
			m_umRectSelection = MicroMeterRect( umCrsrPos, umOldPos );
			m_pModel->Apply2AllInRect<Shape>( m_umRectSelection, [&]( Shape & shape ) { shape.Select(); } );
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
			Shape const * pShapeSuper { nullptr };
			if ( IsDefined( m_shapeHighlighted ) )
			{
				m_pNNetWorkThreadInterface->PostMoveShape( m_shapeHighlighted, umCrsrPos - umOldPos );
				if ( m_pModel->IsOfType<BaseKnot>( m_shapeHighlighted ) )
				{
					pShapeSuper = m_pModel->FindShapeAt
					( 
						umCrsrPos, 
						[&]( Shape const & shape ) { return m_pModel->ConnectsTo( m_shapeHighlighted, shape.GetId() ); } 
					);
				}
			}
			else if ( m_bMoveAllowed )
			{
				m_coord.Move( ptCrsr - m_ptLast );
			}

			m_shapeSuperHighlighted = pShapeSuper ? pShapeSuper->GetId( ) : NO_SHAPE;
		}
		m_ptLast = ptCrsr;
		Notify( TRUE );     // cause immediate repaint
	}
	else  // no mouse button pressed
	{                         
		Shape   const * pShapeHighlight { m_pModel->FindShapeAt( umCrsrPos, [&]( Shape const & s) { return true; } ) };
		ShapeId const   idHighlight     { pShapeHighlight ? pShapeHighlight->GetId( ) : NO_SHAPE };
		if ( idHighlight != m_shapeHighlighted )
		{
			m_shapeHighlighted = idHighlight; 
			Notify( TRUE );     // cause immediate repaint
		}
		m_ptLast = PP_NULL;   // make m_ptLast invalid
		 // no refresh! It would cause repaint for every mouse move 
	}
}

void NNetWindow::Escape( )
{
	m_pModel->UnselectAll( );
}

void NNetWindow::SelectAll( )
{
	m_pModel->SelectAll( );
}

void NNetWindow::drawHighlightedShape( )
{
	Shape * const pShapeHighlighted { m_pModel->GetShape( m_shapeHighlighted ) };
	if ( pShapeHighlighted && pShapeHighlighted->IsAnyNeuron() )
	{
		pShapeHighlighted->DrawExterior( m_coord, tHighlightType::highlighted );
		pShapeHighlighted->DrawInterior( m_coord );
	}
}

tHighlightType const NNetWindow::GetHighlightType( Shape const & shape ) const
{
	return ( (shape.GetId() == m_shapeSuperHighlighted) || shape.IsSelected() )
		   ? tHighlightType::superHighlighted
		   : ( shape.GetId() == m_shapeHighlighted )
			 ? tHighlightType::highlighted
			 : tHighlightType::normal;
}

void NNetWindow::doPaint( ) 
{
	PixelRect      const pixRect { GetClPixelRect( ) };
	MicroMeterRect const umRect  { m_coord.convert2MicroMeterRect( pixRect ) };

	if ( m_umRectSelection.IsNotEmpty( ) )
	{
		m_D2d_driver.DrawTranspRect( m_coord.convert2fPixelRect( m_umRectSelection ), COLOR_SELECTION );
	}

	if ( m_coord.GetPixelSize() <= 5._MicroMeter )
		m_pModel->Apply2AllInRect<Shape>( umRect, [&]( Shape & shape ) { shape.DrawExterior( m_coord, GetHighlightType( shape ) ); } );

	m_pModel->Apply2AllInRect<Pipe>( umRect, [&]( Pipe & shape ) { shape.DrawInterior( m_coord ); } );
	m_pModel->Apply2AllInRect<BaseKnot>( umRect, [&]( BaseKnot & shape ) { shape.DrawInterior( m_coord ); } );
	
	drawHighlightedShape( );

	m_pScale->ShowScale( convert2fPIXEL( GetClientWindowHeight() ) );

	if ( m_coord.GetPixelSize() <= 2.5_MicroMeter )
		m_pModel->Apply2AllInRect<BaseKnot>( umRect, [&]( BaseKnot & shape ) { shape.DrawNeuronText( m_coord ); } );
}

void NNetWindow::CenterModel( )
{
	m_focusMode = FOCUS_MODE::ZOOM_IN;
	centerAndZoomRect( m_pModel->GetEnclosingRect( ), 1.2f );
}

void NNetWindow::AnalysisFinished( )
{
	if ( m_pModel->IsInEmphasizeMode() )
	{
		m_focusMode = FOCUS_MODE::ZOOM_OUT;
		centerAndZoomRect( m_pModel->GetEnclosingRect( ), 1.2f );
	}
	else
	{
		m_focusMode = FOCUS_MODE::NO_FOCUS;
	}
}

void NNetWindow::centerAndZoomRect( MicroMeterRect const rect, float const fRatioFactor )
{
	float           const fHorizontalRatio  { rect.GetHeight() / m_coord.convert2MicroMeter( GetClientWindowHeight() ) };
	float           const fVerticalRatio    { rect.GetWidth () / m_coord.convert2MicroMeter( GetClientWindowWidth() ) };
	float           const fMaxRatio         { max( fVerticalRatio, fHorizontalRatio ) };
	float           const fDesiredRatio     { fMaxRatio * fRatioFactor };
	fPixelPoint     const fpCenter          { m_coord.convert2fPixelPoint( GetClRectCenter( ) ) };
	MicroMeter      const umPixelSizeTarget { m_coord.LimitPixelSize( m_coord.GetPixelSize() * fDesiredRatio ) };
	MicroMeterPoint const umPntCenterTarget { rect.GetCenter() };
	m_umPixelSizeStart = m_coord.GetPixelSize();                                     // actual pixel size 
	m_umPntCenterStart = m_coord.convert2MicroMeterPointPos( GetClRectCenter( ) );   // actual center 
	m_umPixelSizeDelta = umPixelSizeTarget - m_umPixelSizeStart;
	m_umPntCenterDelta = umPntCenterTarget - m_umPntCenterStart;
	m_smoothMove.Reset();
}

bool NNetWindow::smoothStep( )  // returns true, if all targets reached
{
	float fPos            { m_smoothMove.Step() };
	bool  fTargetsReached { fPos >= SmoothMoveFp::END_POINT };

	if ( ! fTargetsReached )
	{
		fPixelPoint const fpCenter { m_coord.convert2fPixelPoint( GetClRectCenter( ) ) };
		m_coord.Zoom  ( m_umPixelSizeStart + m_umPixelSizeDelta * fPos );
		m_coord.Center( m_umPntCenterStart + m_umPntCenterDelta * fPos, fpCenter );
		Notify( TRUE );     // cause immediate repaint
	}

	return fTargetsReached;
}

void NNetWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );
		if ( m_D2d_driver.StartFrame( GetWindowHandle(), hDC ) )
		{
			doPaint( );
			m_D2d_driver.EndFrame( GetWindowHandle() );
		}
		EndPaint( &ps );
	}

	if ( (m_focusMode != FOCUS_MODE::NO_FOCUS) && smoothStep() )
	{
		switch ( m_focusMode )
		{
		case FOCUS_MODE::ZOOM_OUT:
			m_focusMode = FOCUS_MODE::ZOOM_IN;
			centerAndZoomRect( ModelAnalyzer::GetEnclosingRect(), 3.0f );
			break;

		case FOCUS_MODE::ZOOM_IN:
			m_focusMode = FOCUS_MODE::NO_FOCUS;
			break;

		default:
			assert( false );
		}
	}
}

void NNetWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_D2d_driver.Resize( width, height );
}

void NNetWindow::OnLeftButtonDblClick( WPARAM const wParam, LPARAM const lParam )
{
	m_umRectSelection.SetZero( );
}

void NNetWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{
	int        iDelta     = GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA;
	BOOL const bDirection = ( iDelta > 0 );
	MicroMeter newSize;

	iDelta = abs( iDelta );

	while ( --iDelta >= 0 )
	{
		newSize = m_coord.ComputeNewPixelSize( bDirection );
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
	m_umRectSelection.SetZero();
	return false;
}

bool NNetWindow::OnRButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	if ( (wParam & MK_CONTROL) == 0 )
		m_pModel->UnselectAll( );
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

// PixelPoint2MicroMeterPoint belongs to NNetWindow
// because every NNetWindow needs its own coordinate system

MicroMeterPoint NNetWindow::PixelPoint2MicroMeterPoint( PixelPoint const pixPoint ) const
{
	return m_coord.convert2MicroMeterPointPos( pixPoint );
}

LPARAM NNetWindow::pixelPoint2LPARAM( PixelPoint const pixPoint ) const
{
	return Util::Pack2UINT64( PixelPoint2MicroMeterPoint( pixPoint ) );
}

LPARAM NNetWindow::crsPos2LPARAM( ) const 
{
	return pixelPoint2LPARAM( GetRelativeCrsrPosition() );
}

BOOL NNetWindow::OnCommand( WPARAM const wParam, LPARAM const lParam )
{
	PostCommand2Application( wParam, pixelPoint2LPARAM( m_ptCommandPosition ) );
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
