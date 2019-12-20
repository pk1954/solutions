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
#include "PixelCoordsFp.h"
#include "win32_tooltip.h"
#include "win32_stdDialogBox.h"
#include "win32_scale.h"
#include "win32_util_resource.h"
#include "Direct2D.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWindow.h"

using std::function;

NNetReadBuffer          * NNetWindow::m_pReadBuffer              = nullptr;
NNetWorkThreadInterface * NNetWindow::m_pNNetWorkThreadInterface = nullptr;

void NNetWindow::InitClass
(        
	NNetReadBuffer          * const pReadBuffer,
	NNetWorkThreadInterface * const pNNetWorkThreadInterface,
	ActionTimer             * const pActionTimer
)
{
	ModelWindow::InitClass( pActionTimer );
	m_pReadBuffer              = pReadBuffer;
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
	m_ptLast( PP_NULL ),
	m_bMoveAllowed( TRUE )
{ }

void NNetWindow::Start
( 
	HWND             const hwndApp, 
	DWORD            const dwStyle,
	function<bool()> const visibilityCriterion
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
	m_pReadBuffer->RegisterObserver( this );
	m_pScale = new Scale( & m_D2d_driver, & m_coord );
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
	MicroMeterPoint const umPointcenter   { m_coord.convert2MicroMeterPoint( fPixPointCenter ) };
	if ( m_coord.ZoomNNet( newSize ) ) 
	{
		m_coord.CenterSimulationArea( umPointcenter, fPixPointCenter ); 
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

ShapeId const NNetWindow::getShapeUnderPoint( PixelPoint const pnt )
{
	MicroMeterPoint const   umCrsrPos { m_coord.convert2MicroMeterPoint( pnt ) };
	NNetModel       const * pModel    { m_pReadBuffer->GetModel( ) };
	Shape           const * pShape    { pModel->FindShapeUnderPoint( umCrsrPos ) };
	return pModel->GetId( pShape );
}

void NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu, PixelPoint const pntPos )
{
	NNetModel const * pModel    { m_pReadBuffer->GetModel( ) };
	UINT      const   STD_FLAGS { MF_BYPOSITION | MF_STRING };

	switch ( pModel->GetHighlightedShapeType( ) )
	{
	case tShapeType::inputNeuron:
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_RATE, pModel->GetParameterName( tParameter::pulseRate ) );
		break;

	case tShapeType::neuron:
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON,  L"Add input neuron" );
		break;

	case tShapeType::knot:  
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_NEURON,        L"Add neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON,  L"Add input neuron" );
		break;

	case tShapeType::pipeline:
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTGOING,     L"Add outgoing dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INCOMING,     L"Add incoming dendrite" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_NEURON,       L"Add neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON, L"Add input neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_INSERT_NEURON,    L"Insert neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_SPLIT_PIPELINE,   L"Split" );
		break;

	case tShapeType::undefined: // noshape selected
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_NEURON,       L"Add neuron" );
		AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON, L"Add input neuron" );
		break;

	default:
		assert( false );
	}

	AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE, L"Remove" );
}

void NNetWindow::ParameterDlg( tParameter const param )
{
	NNetModel const * pModel    { m_pReadBuffer->GetModel( ) };
	float     const   fOldValue { pModel->GetParameterValue( param ) };
	wstring   const   header    { pModel->GetParameterName ( param ) }; 
	wstring   const   unit      { pModel->GetParameterUnit ( param ) }; 
	float     const   fNewValue { StdDialogBox::Show( GetWindowHandle(), fOldValue, header, unit ) };
	m_pNNetWorkThreadInterface->PostSetParameter( param, fNewValue );
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const   ptCrsr { GetCrsrPosFromLparam( lParam ) };  // relative to client area
	NNetModel  const * pModel { m_pReadBuffer->GetModel() };

	if ( wParam & MK_RBUTTON )          // Right mouse button: selection
	{
	}
	else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
	{
		if ( m_ptLast.IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			Shape   const * pShapeSuper   { nullptr };
			ShapeId const   idHighlighted { pModel->GetHighlightedShapeId( ) };
			if ( IsDefined( idHighlighted ) )
			{
				MicroMeterPoint const umOldPos { m_coord.convert2MicroMeterPoint( m_ptLast ) };
				MicroMeterPoint const umNewPos { m_coord.convert2MicroMeterPoint( ptCrsr   ) };
				m_pNNetWorkThreadInterface->PostMoveShape( idHighlighted, umNewPos - umOldPos );

				if ( pModel->GetHighlightedShapeType( ) == tShapeType::neuron )
				{
					pShapeSuper = pModel->FindShapeUnderPoint
					( 
						umNewPos, 
						[&]( Shape const & shape ) 
						{ 
							return (shape.GetId() != idHighlighted) && NeuronType::TypeFits(shape.GetShapeType());
						} 
					);
				}
			}
			else if ( m_bMoveAllowed )
			{
				m_coord.MoveNNet( ptCrsr - m_ptLast );
			}

			m_pNNetWorkThreadInterface->PostHighlight( pModel->GetId( pShapeSuper ), true );
		}
		m_ptLast = ptCrsr;
		PostCommand2Application( IDM_REFRESH, 0 );
	}
	else  // no mouse button pressed
	{                         
		m_pNNetWorkThreadInterface->PostHighlight( getShapeUnderPoint( ptCrsr ), false );
		m_ptLast = PP_NULL;   // make m_ptLast invalid
		 // no refresh! It would cause repaint for every mouse move 
	}
}

void NNetWindow::drawHighlightedShape( NNetModel const & model, PixelCoordsFp & coord )
{
	ShapeId       const idHighlighted     { model.GetHighlightedShapeId( ) };
	Shape const * const pShapeHighlighted { model.GetConstShape( model.GetHighlightedShapeId( ) ) };
	if ( pShapeHighlighted && NeuronType::TypeFits(pShapeHighlighted->GetShapeType()) )
	{
		pShapeHighlighted->DrawExterior( coord );
		pShapeHighlighted->DrawInterior( coord );
	}
}

void NNetWindow::doPaint( ) 
{
	NNetModel const * pModel = m_pReadBuffer->GetModel( );
	if ( m_coord.GetPixelSize() <= 5._MicroMeter )
		pModel->Apply2All<Shape>   ( [&]( Shape    & shape ) { shape.DrawExterior( m_coord ); } );
	pModel->Apply2All<Pipeline>( [&]( Pipeline & shape ) { shape.DrawInterior( m_coord ); } );
	pModel->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { shape.DrawInterior( m_coord ); } );
	drawHighlightedShape( * pModel, m_coord );
	m_pScale->ShowScale( convert2fPIXEL( GetClientWindowHeight() ) );
	if ( m_coord.GetPixelSize() <= 2.5_MicroMeter )
		pModel->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { shape.DrawNeuronText( m_coord ); } );
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
//	PixelPoint const   ptCrsr = GetCrsrPosFromLparam( lParam );  // relative to client area
//	Shape      const * pShape = getShapeUnderPoint( ptCrsr );
//
//	if ( pShape )
//	{
//		switch ( pShape->GetShapeType() )
//		{
//		case tShapeType::pipeline:
////			m_pNNetWorkThreadInterface->PostHighlight( pShape->GetId( ) );
////			PostCommand2Application( IDD_INSERT_NEURON, pixelPoint2LPARAM( ptCrsr ) );
//			break;
//
//		default:
//			break;
//		}
//	}
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

void NNetWindow::OnLButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	if ( inObservedClientRect( lParam ) || m_bMoveAllowed )
	{
		//PixelPoint const   ptCrsr { GetCrsrPosFromLparam( lParam ) };  // relative to client area
		//Shape      const * pShape { getShapeUnderPoint( ptCrsr ) };
		//NNetModel  const * pModel { m_pReadBuffer->GetModel( ) };
		//m_pNNetWorkThreadInterface->PostHighlight( pModel->GetId( pShape ) );
		//OnMouseMove( wParam, lParam );
	}
}

void NNetWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	NNetModel const * pModel { m_pReadBuffer->GetModel( ) };
	if ( 
		  Neuron     ::TypeFits( pModel->GetHighlightedShapeType() ) &&
		  EndKnotType::TypeFits( pModel->GetSuperHighlightedShapeType() ) 
	   )
       PostCommand2Application( IDD_CONNECT, 0 );
//	ReleaseCapture( );
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
	MicroMeterPoint const umPoint   { m_coord.convert2MicroMeterPoint( fPixPoint ) };
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
	PostCommand2Application( wParam, crsPos2LPARAM( ) );
	return FALSE;
}

BOOL NNetWindow::inObservedClientRect( LPARAM const lParam )
{
	return TRUE;  // Is cursor position in observed client rect?
}
