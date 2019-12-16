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

NNetReadBuffer          * NNetWindow::m_pReadBuffer              = nullptr;
NNetWorkThreadInterface * NNetWindow::m_pNNetWorkThreadInterface = nullptr;
PixelCoordsFp             NNetWindow::m_coord;

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

NNetWindow::NNetWindow( ) :
	ModelWindow( ),
	m_hPopupMenu( nullptr ),
	m_pScale( nullptr ),
	m_ptLast( PP_NULL ),
	m_bMoveAllowed( TRUE )
{ }

void NNetWindow::Start
( 
	HWND                  const hwndApp, 
	DWORD                 const dwStyle,
	std::function<bool()> const visibilityCriterion
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

	m_D2d_driver.Initialize
	( 
		hwnd, 
		200,   // TODO ????
		100, 
		FALSE 
	);

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

	ShapeId const idShapeHighlighted( pModel->GetHighlightedShapeId( ) ); 
	if ( idShapeHighlighted != NO_SHAPE )  
	{
		switch ( pModel->GetHighlightedShapeType( ) )
		{
		case tShapeType::knot:  
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON,  L"Add input neuron" );
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTPUT_NEURON, L"Add output neuron" );
			break;

		case tShapeType::inputNeuron:
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_RATE, pModel->GetParameterName( tParameter::pulseRate ) );
			break;

		case tShapeType::neuron:
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON,  L"Add input neuron" );
			break;

		case tShapeType::outputNeuron:
 			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON, L"Add input neuron" );
			break;

		case tShapeType::pipeline:
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTPUT_NEURON, L"Add output neuron" );
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON,  L"Add input neuron" );
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_INSERT_NEURON,     L"Insert neuron" );
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_SPLIT_PIPELINE,    L"Split" );
			break;

		default:
			assert( false );
		}

		if ( pModel->HighlightedShapeCanBeDeleted( ) )
			AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE, L"Remove" );
	}
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
				MicroMeterPoint const umPoint { m_coord.convert2MicroMeterPoint( ptCrsr ) };
				m_pNNetWorkThreadInterface->PostMoveShape( idHighlighted, umPoint );
				tShapeType const type { pModel->GetHighlightedShapeType( ) };
				if ( type == tShapeType::outputNeuron )
				{
					pShapeSuper = pModel->FindShapeUnderPoint
					( 
						umPoint, 
						[&]( Shape const & shape ) 
						{ 
							return (shape.GetId() != idHighlighted) && NeuronType::TypeFits(shape.GetShapeType());
						} 
					);
				}
			}
			else
			{
				moveNNet( ptCrsr - m_ptLast );
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

void NNetWindow::moveNNet( PixelPoint const ptDiff )
{
	if ( m_bMoveAllowed )
	{
		m_coord.MoveNNet( ptDiff );
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
	//m_D2d_driver.StartPipeline
	//( 
	//	fPixelPoint::NULL_VAL(), 
	//	fPixelPoint::NULL_VAL(), 
	//	fPIXEL::NULL_VAL(), 
	//	COLORREF( 0 )
	//);
	NNetModel const * pModel = m_pReadBuffer->GetModel( );
	pModel->Apply2All<Shape>   ( [&]( Shape    & shape ) { shape.DrawExterior( m_coord ); } );
	pModel->Apply2All<Pipeline>( [&]( Pipeline & shape ) { shape.DrawInterior( m_coord ); } );
	pModel->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { shape.DrawInterior( m_coord ); } );
	drawHighlightedShape( * pModel, m_coord );
	m_pScale->ShowScale( convert2fPIXEL( GetClientWindowHeight() ) );
//	m_pGraphics->SetFontSize( 15_PIXEL );
	pModel->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { shape.DrawText( m_coord ); } );
	m_D2d_driver.RenderForegroundObjects( );
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
		  OutputNeuron::TypeFits( pModel->GetHighlightedShapeType() ) &&
		  EndKnotType ::TypeFits( pModel->GetSuperHighlightedShapeType() ) 
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
