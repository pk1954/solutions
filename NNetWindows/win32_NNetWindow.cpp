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
	m_pShapeSelected( nullptr ),
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

Shape const * NNetWindow::getShapeUnderPoint( PixelPoint const pnt )
{
	MicroMeterPoint const   umCrsrPos { m_coord.convert2MicroMeterPoint( pnt ) };
	NNetModel       const * pModel    { m_pReadBuffer->GetModel( ) };
	Shape           const * pShape    { pModel->FindShapeUnderPoint( umCrsrPos ) };
	return pShape;
}

void NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu, PixelPoint const pntPos )
{
	UINT const STD_FLAGS = MF_BYPOSITION | MF_STRING;

	Shape const * pShape = getShapeUnderPoint( pntPos );
	if ( pShape )
	{
		switch ( pShape->GetShapeType( ) )
		{
		case tShapeType::inputNeuron:
			m_pShapeSelected = pShape;
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_RATE_DIALOG, L"Pulse rate" );
			break;

		case tShapeType::knot:  
			m_pShapeSelected = pShape;
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_CREATE_NEW_BRANCH, L"New Branch" );
			break;

		case tShapeType::neuron:
			break;

		case tShapeType::outputNeuron:
			break;

		case tShapeType::pipeline:
			m_pShapeSelected = pShape;
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_NNETW_SPLIT_PIPELINE, L"Split" );
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_SPEED_DIALOG,   L"Conduction velocity" );
			break;

		default:
			assert( false );
		}
	}
	else
	{
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_NNETW_NEW_NEURON,        L"New neuron" );
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_NNETW_NEW_INPUT_NEURON,  L"New input neuron" );
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_NNETW_NEW_OUTPUT_NEURON, L"New output neuron" );
	}
}

void NNetWindow::PulseRateDialog( )
{
	InputNeuron const * pInputNeuron = Cast2InputNeuron( m_pShapeSelected );
	fHertz      const   pulseRateOld = pInputNeuron->GetPulseFrequency();
	float fNewValue = StdDialogBox::Show
	( 
		GetWindowHandle(),
		static_cast<float>( pulseRateOld.GetValue() ),
		L"Pulse rate",
		L"Hertz"
	);
	PostCommand2Application
	( 
		IDM_PULSE_FREQ, 
		Util::Pack2UINT64
		(
			m_pShapeSelected->GetId().GetValue(), 
			(UINT32&)fNewValue
		) 
	);
}

void NNetWindow::PulseSpeedDialog( )
{
	NNetModel   const * pModel        = m_pReadBuffer->GetModel( );
	Pipeline    const * pPipeline     = Cast2Pipeline( m_pShapeSelected );
	meterPerSec const   pulseSpeedOld = pModel->GetImpulseSpeed();
	float fNewValue = StdDialogBox::Show
	( 
		GetWindowHandle(),
		static_cast<float>( pulseSpeedOld.GetValue() ),
		L"Conduction velocity",
		L"m/sec"
	);
	meterPerSec const pulseSpeedNew( fNewValue );
	PostCommand2Application
	( 
		IDM_PULSE_SPEED,
		(LPARAM &)pulseSpeedNew
	);
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );  // relative to client area

	if ( wParam & MK_RBUTTON )          // Right mouse button: selection
	{
	}
	else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
	{
		if ( m_ptLast.IsNotNull() )     // last cursor pos stored in m_ptLast
		{
			Shape const * pShapeSuper = nullptr;
			if ( m_pShapeSelected )
			{
				ShapeId         const idSelected { m_pShapeSelected->GetId() };
				MicroMeterPoint const umPoint    { m_coord.convert2MicroMeterPoint( ptCrsr ) };
				m_pNNetWorkThreadInterface->PostMoveShape( idSelected, umPoint );
				if ( m_pShapeSelected->GetShapeType() == tShapeType::knot )
				{
					NNetModel const * pModel { m_pReadBuffer->GetModel() };
					Knot      const * pKnot  { pModel->GetConstKnot( idSelected ) };
					if ( ! pKnot->HasOutgoing() )
					{
						pShapeSuper = m_pReadBuffer->GetModel( )->FindShapeUnderPoint
						( 
							umPoint, 
							[&]( Shape const & shape ) 
							{ 
								return (shape.GetId() != idSelected) && IsNeuronType(shape.GetShapeType());
							} 
						);
					}
				}
			}
			else
				moveNNet( ptCrsr - m_ptLast );

			PostCommand2Application( IDM_SUPER_HIGHLIGHT, (pShapeSuper ? pShapeSuper->GetId() : NO_SHAPE).GetValue() );
		}
		m_ptLast = ptCrsr;
		PostCommand2Application( IDM_REFRESH, 0 );
	}
	else
	{                         
		Shape const * pShape = getShapeUnderPoint( ptCrsr );
		PostCommand2Application( IDM_HIGHLIGHT, (pShape ? pShape->GetId() : NO_SHAPE).GetValue() );
							  // make m_ptLast invalid
		m_ptLast = PP_NULL;   // no refresh! It would cause repaint for every mouse move 
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
	if ( m_pShapeSelected && IsNeuronType( m_pShapeSelected->GetShapeType() ) )
	{
		m_pShapeSelected->DrawExterior( model, coord );
		m_pShapeSelected->DrawInterior( model, coord );
	}
}

void NNetWindow::doPaint( )
{
	NNetModel const * pModel = m_pReadBuffer->GetModel( );
	pModel->Apply2AllPipelines( [&]( Shape & shape ) { shape.DrawInterior( * pModel, m_coord ); } );
	pModel->Apply2AllShapes   ( [&]( Shape & shape ) { shape.DrawExterior( * pModel, m_coord ); } );
	pModel->Apply2AllNeurons  ( [&]( Shape & shape ) { shape.DrawInterior( * pModel, m_coord ); } );
	drawHighlightedShape( * pModel, m_coord );
	m_pScale->ShowScale( fPIXEL( static_cast<float>( GetClientWindowHeight().GetValue() ) ) );
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
			doPaint( );
			m_pGraphics->EndFrame( GetWindowHandle() );
		}
		(void)EndPaint( &ps );
	}
}

void NNetWindow::OnLeftButtonDblClick(WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const   ptCrsr = GetCrsrPosFromLparam( lParam );  // relative to client area
	Shape      const * pShape = getShapeUnderPoint( ptCrsr );

	if ( pShape )
	{
		switch ( pShape->GetShapeType() )
		{
		//case tShapeType::inputNeuron:
		//	PostCommand2Application( IDD_TRIGGER, pShape->GetId().GetValue() );
		//	break;
		//
		case tShapeType::pipeline:
			m_pShapeSelected = pShape;
			PostCommand2Application( IDD_SPLIT_PIPELINE, pixelPoint2LPARAM( ptCrsr ) );
			break;

		default:
			break;
		}
	}
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
		PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );  // relative to client area
		m_pShapeSelected = getShapeUnderPoint( ptCrsr );
		OnMouseMove( wParam, lParam );
	}
}

void NNetWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	PostCommand2Application( IDD_CONNECT, 0 );
	m_pShapeSelected = nullptr;
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
	UINT uiCmdId = LOWORD( wParam );
	switch ( uiCmdId )
	{
	case IDD_NNETW_SPLIT_PIPELINE:
		PostCommand2Application( IDD_SPLIT_PIPELINE, crsPos2LPARAM( ) );
		break;

	case IDD_NNETW_NEW_NEURON:
		PostCommand2Application( IDD_CREATE_NEW_NEURON, crsPos2LPARAM( ) );
		break;

	case IDD_NNETW_NEW_INPUT_NEURON:
		PostCommand2Application( IDD_CREATE_NEW_INPUT_NEURON, crsPos2LPARAM( ) );
		break;

	case IDD_NNETW_NEW_OUTPUT_NEURON:
		PostCommand2Application( IDD_CREATE_NEW_OUTPUT_NEURON, crsPos2LPARAM( ) );
		break;

	default:
		return TRUE;
	}
	return FALSE;
}

BOOL NNetWindow::inObservedClientRect( LPARAM const lParam )
{
	return TRUE;  // Is cursor position in observed client rect?
}
