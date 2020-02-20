// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "Resource.h"
#include "MoreTypes.h"
#include "Segment.h"
#include "Pipeline.h"
#include "InputNeuron.h"
#include "PixelTypes.h"
#include "AnimationThread.h"
#include "tHighlightType.h"
#include "PixelCoordsFp.h"
#include "Direct2D.h"
#include "win32_fatalError.h"
#include "win32_sound.h"
#include "win32_tooltip.h"
#include "win32_stdDialogBox.h"
#include "win32_scale.h"
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
	ModelWindow( ),
	m_hPopupMenu( nullptr ),
	m_pScale( nullptr ),
	m_bMoveAllowed( TRUE ),
	m_ptLast( PP_NULL ),
	m_ptCommandPosition( PP_NULL ),
	m_shapeHighlighted     ( NO_SHAPE ),
	m_shapeSuperHighlighted( NO_SHAPE ),
	m_pAnimationThread( nullptr ),
	m_pCursorPosObservable( nullptr )
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
	SetPixelSize( m_coord.ComputeNewPixelSize( bZoomIn ) );
}

void NNetWindow::SetPixelSize( MicroMeter const newSize )
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

MicroMeter NNetWindow::GetPixelSize( ) const
{
	return m_coord.GetPixelSize( );
}

void NNetWindow::setHighlightShape( PixelPoint const pnt )
{
	MicroMeterPoint const   umCrsrPos     { m_coord.convert2MicroMeterPointPos( pnt ) };
	Shape           const * pShape        { m_pModel->FindShapeAt( umCrsrPos, [&]( Shape const & s) { return true; } ) };
	ShapeId         const   idHighlighted { m_pModel->GetId( pShape ) };
	if ( idHighlighted != m_shapeHighlighted )
	{
		m_shapeHighlighted = idHighlighted; 
		Notify( TRUE );     // cause immediate repaint
	}
}

void NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu, PixelPoint const ptPos )
{
	UINT const STD_FLAGS { MF_BYPOSITION | MF_STRING };

	m_ptCommandPosition = ptPos;

	switch ( m_pModel->GetShapeType( m_shapeHighlighted ) )
	{
	case tShapeType::inputNeuron:
		if ( ! m_pModel->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_RATE,            L"Pulse rate" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE,          L"Remove" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_DISCONNECT,            L"Disconnect" );
		break;

	case tShapeType::neuron:
		if ( ! m_pModel->HasOutgoing( m_shapeHighlighted ) )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2KNOT, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INCOMING2KNOT,     L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE,          L"Remove" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_DISCONNECT,            L"Disconnect" );
		break;

	case tShapeType::knot:  
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

	case tShapeType::pipeline:
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING2PIPE, L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INCOMING2PIPE, L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_INSERT_NEURON,     L"Insert neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE,      L"Remove" );
		if ( Pipeline::GetArrowSize( ) > 0.0_MicroMeter )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ARROWS_OFF,    L"Arrows off" );
		else
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ARROWS_ON,     L"Arrows on" );

		break;

	case tShapeType::undefined: // noshape selected
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_NEW_NEURON,       L"New neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_NEW_INPUT_NEURON, L"New input neuron" );
		break;

	default:
		assert( false );
	}
}

bool NNetWindow::PulseRateDlg( ShapeId const id )
{
    InputNeuron const * pInputNeuron { m_pModel->GetConstTypedShape<InputNeuron>(id) };
	if ( pInputNeuron == nullptr )
		return false;
	float   const fOldValue { m_pModel->GetPulseRate( pInputNeuron ) };
	wstring const header    { GetParameterName ( tParameter::pulseRate ) }; 
	wstring const unit      { GetParameterUnit ( tParameter::pulseRate ) }; 
	float   const fNewValue { StdDialogBox::Show( GetWindowHandle(), fOldValue, header, unit ) };
	bool    const bRes      { fNewValue != fOldValue };
	if ( bRes )
		m_pNNetWorkThreadInterface->PostSetPulseRate( id, fNewValue );
	return bRes;
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const ptCrsr { GetCrsrPosFromLparam( lParam ) };  // relative to client area

	m_pCursorPosObservable->NotifyAll( false );

	if ( wParam & MK_RBUTTON )          // Right mouse button: selection
	{
	}
	else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
	{
		if ( m_ptLast.IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			Shape const * pShapeSuper { nullptr };
			if ( IsDefined( m_shapeHighlighted ) )
			{
				MicroMeterPoint const umOldPos { m_coord.convert2MicroMeterPointPos( m_ptLast ) };
				MicroMeterPoint const umNewPos { m_coord.convert2MicroMeterPointPos( ptCrsr   ) };
				m_pNNetWorkThreadInterface->PostMoveShape( m_shapeHighlighted, umNewPos - umOldPos );
				if ( m_pModel->IsType<BaseKnot>( m_shapeHighlighted ) )
				{
					pShapeSuper = m_pModel->FindShapeAt
					( 
						umNewPos, 
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
		setHighlightShape( ptCrsr );
		m_ptLast = PP_NULL;   // make m_ptLast invalid
		 // no refresh! It would cause repaint for every mouse move 
	}
}

void NNetWindow::drawHighlightedShape( NNetModel & model, PixelCoordsFp & coord )
{
	Shape * const pShapeHighlighted { model.GetShape( m_shapeHighlighted ) };
	if ( pShapeHighlighted && IsAnyNeuronType(pShapeHighlighted->GetShapeType()) )
	{
		pShapeHighlighted->DrawExterior( coord, tHighlightType::highlighted );
		pShapeHighlighted->DrawInterior( coord );
	}
}

tHighlightType const NNetWindow::GetHighlightType( ShapeId const id ) const
{
	return ( id == m_shapeSuperHighlighted )
		   ? tHighlightType::superHighlighted
		   : ( id == m_shapeHighlighted )
			 ? tHighlightType::highlighted
			 : tHighlightType::normal;
}

void NNetWindow::doPaint( ) 
{
	PixelRect      const pixRect { GetClPixelRect( ) };
	MicroMeterRect const umRect  { m_coord.convert2MicroMeterRect( pixRect ) };

	if ( m_coord.GetPixelSize() <= 5._MicroMeter )
		m_pModel->Apply2All<Shape>( [&]( Shape & shape ) { if ( shape.IsInRect( umRect ) ) shape.DrawExterior( m_coord, GetHighlightType( shape.GetId() ) ); return false; } );

	m_pModel->Apply2All<Pipeline>( [&]( Pipeline & shape ) { if ( shape.IsInRect( umRect ) ) shape.DrawInterior( m_coord ); return false; } );
	m_pModel->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { if ( shape.IsInRect( umRect ) ) shape.DrawInterior( m_coord ); return false; } );
	
	drawHighlightedShape( * m_pModel, m_coord );

	m_pScale->ShowScale( convert2fPIXEL( GetClientWindowHeight() ) );

	if ( m_coord.GetPixelSize() <= 2.5_MicroMeter )
		m_pModel->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { if ( shape.IsInRect( umRect ) )shape.DrawNeuronText( m_coord ); return false; } );
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
}

void NNetWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_D2d_driver.Resize( width, height );
}

void NNetWindow::OnLeftButtonDblClick( WPARAM const wParam, LPARAM const lParam )
{
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

void NNetWindow::OnLButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	setHighlightShape( GetCrsrPosFromLparam( lParam ) );
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
	fPixelPoint     const fPixPoint { convert2fPixelPoint( pixPoint ) };
	MicroMeterPoint const umPoint   { m_coord.convert2MicroMeterPointPos( fPixPoint ) };
	return umPoint;
}

LPARAM NNetWindow::pixelPoint2LPARAM( PixelPoint const pixPoint ) const
{
	MicroMeterPoint const umPoint { PixelPoint2MicroMeterPoint( pixPoint ) };
	return Util::Pack2UINT64(umPoint);
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
	m_pAnimationThread->SetTarget( bShow ? Pipeline::STD_ARROW_SIZE : 0.0_MicroMeter );
}
