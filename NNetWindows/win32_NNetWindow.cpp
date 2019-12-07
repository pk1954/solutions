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
#include "win32_graphicsInterface.h"
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
	m_pGraphics( nullptr ),
	m_pScale( nullptr ),
	m_ptLast( PP_NULL ),
	m_bMoveAllowed( TRUE )
{ }

void NNetWindow::Start
( 
	HWND                  const hwndApp, 
	GraphicsInterface   * const pGraphics,
	DWORD                 const dwStyle,
	std::function<bool()> const visibilityCriterion
)
{
	m_pGraphics = pGraphics;

	m_pScale = new Scale( m_pGraphics, & m_coord );

	HWND hwnd = StartBaseWindow
	( 
		hwndApp,
		CS_OWNDC | CS_DBLCLKS,
		L"ClassNNetWindow",
		dwStyle,
		nullptr,
		visibilityCriterion
	);

	m_pReadBuffer->RegisterObserver( this );
}

void NNetWindow::Stop( )
{
	delete m_pScale;
	m_pScale = nullptr;
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pNNetWorkThreadInterface = nullptr;
	m_pGraphics = nullptr;
}

//void NNetWindow::Size( )
//{
//	Move
//	(
//		Util::CalcWindowRect
//		( 
//			m_coord.NNet2PixelRect( GridDimensions::GridRectFull() ),
//			(DWORD)GetWindowLongPtr( GetWindowHandle( ), GWL_STYLE ) 
//		), 
//		FALSE 
//	);
//}

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

	if ( pModel->GetHighlightedShapeId( ) == NO_SHAPE )  
	{
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_DAMPING_FACTOR,    pModel->GetParameterName( tParameter::dampingFactor    ) );
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_THRESHOLD,         pModel->GetParameterName( tParameter::threshold        ) );
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_PEAK_VOLTAGE,      pModel->GetParameterName( tParameter::peakVoltage      ) );
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_REFRACTORY_PERIOD, pModel->GetParameterName( tParameter::refractoryPeriod ) );
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_WIDTH,       pModel->GetParameterName( tParameter::pulseWidth       ) );
	}
	else
	{
		switch ( pModel->GetHighlightedShapeType( ) )
		{
		case tShapeType::knot:  
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON,  L"Add input neuron" );
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTPUT_NEURON, L"Add output neuron" );
			break;

		case tShapeType::inputNeuron:
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_RATE, pModel->GetParameterName( tParameter::pulseRate ) );
			if ( pModel->GetNrOfInputNeurons( ) > 1 )
				(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE, L"Remove" );
			break;

		case tShapeType::neuron:
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON,  L"Add input neuron" );
			break;

		case tShapeType::outputNeuron:
 			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON, L"Add input neuron" );
			if ( pModel->GetNrOfOutputNeurons( ) > 1 )
				(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_REMOVE_SHAPE, L"Remove" );
			break;

		case tShapeType::pipeline:
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_OUTPUT_NEURON, L"Add output neuron" );
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_ADD_INPUT_NEURON,  L"Add input neuron" );
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_INSERT_NEURON,     L"Insert neuron" );
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_SPLIT_PIPELINE,    L"Split" );
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_SPEED, pModel->GetParameterName( tParameter::pulseSpeed ) );
			break;

		default:
			assert( false );
		}
	}
}

float NNetWindow::ParameterDialog( tParameter const param )
{
	NNetModel const * pModel    { m_pReadBuffer->GetModel( ) };
	float     const   fOldValue { pModel->GetParameterValue( param ) };
	wstring   const   header    { pModel->GetParameterName ( param ) }; 
	wstring   const   unit      { pModel->GetParameterUnit ( param ) }; 
	float     const   fNewValue { StdDialogBox::Show( GetWindowHandle(), fOldValue, header, unit ) };
	return fNewValue;
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
							return (shape.GetId() != idHighlighted) && IsNeuronType(shape.GetShapeType());
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
	//if ( m_pGridWindowObserved != nullptr )     // I observe someone
	//{
	//	EvoPixelCoords * const pixCoordObserved = & m_pGridWindowObserved->m_EvoPixelCoords;
	//	PixelPoint       const ptDiffObserved   = EvoPixel2PixelSize( ptDiff, & m_EvoPixelCoords, pixCoordObserved );
	//	pixCoordObserved->MoveGrid( -ptDiffObserved );   // move the observed in opposite direction 
	//}

	if ( m_bMoveAllowed )
	{
		m_coord.MoveNNet( ptDiff );
	}
}

void NNetWindow::drawHighlightedShape( NNetModel const & model, PixelCoordsFp & coord )
{
	ShapeId       const idHighlighted     { model.GetHighlightedShapeId( ) };
	Shape const * const pShapeHighlighted { model.GetConstShape( model.GetHighlightedShapeId( ) ) };
	if ( pShapeHighlighted && IsNeuronType(pShapeHighlighted->GetShapeType()) )
	{
		pShapeHighlighted->DrawExterior( model, coord );
		pShapeHighlighted->DrawInterior( model, coord );
	}
}

void NNetWindow::doPaint( ) 
{
	NNetModel const * pModel = m_pReadBuffer->GetModel( );
	pModel->Apply2All<Shape>   ( [&]( Shape    & shape ) { shape.DrawExterior( * pModel, m_coord ); } );
	pModel->Apply2All<Pipeline>( [&]( Pipeline & shape ) { shape.DrawInterior( * pModel, m_coord ); } );
	pModel->Apply2All<BaseKnot>( [&]( BaseKnot & shape ) { shape.DrawInterior( * pModel, m_coord ); } );
	drawHighlightedShape( * pModel, m_coord );
	m_pScale->ShowScale( convert2fPIXEL( GetClientWindowHeight() ) );
//	m_pGraphics->SetFontSize( 15_PIXEL );
	pModel->Apply2All<InputNeuron>( [&]( InputNeuron & shape ) { shape.DrawNeuronText( * pModel, m_coord ); } );
	m_pGraphics->RenderForegroundObjects( );
}

void NNetWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );
		if ( m_pGraphics->StartFrame( GetWindowHandle(), hDC ) )
		{
			try
			{
				doPaint( );
			}
			catch ( NNetModel::ModelInconsistencyException )
			{
				int x = 5;
			}
			m_pGraphics->EndFrame( GetWindowHandle() );
		}
		(void)EndPaint( &ps );
	}
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
	NNetModel const * pModel              { m_pReadBuffer->GetModel( ) };
	ShapeId   const   idHighlighted       { pModel->GetHighlightedShapeId( ) };
	ShapeId   const   idHSuperhighlighted { pModel->GetSuperHighlightedShapeId( ) };
	if ( 
		  ( pModel->GetHighlightedShapeType() == tShapeType::outputNeuron ) &&
		  IsEndKnotType( pModel->GetSuperHighlightedShapeType() ) 
	   )
       PostCommand2Application( IDD_CONNECT, 0 );
//	(void)ReleaseCapture( );
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
