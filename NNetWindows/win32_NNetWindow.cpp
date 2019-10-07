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
#include "win32_stdDialogBox.h"
#include "win32_scale.h"
#include "win32_util_resource.h"
#include "win32_graphicsInterface.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWindow.h"

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

void NNetWindow::newPixelSize
( 
	NanoMeter const nmPixelSize, 
	MicroMeterPoint const npCenter 
)
{
	if ( m_coord.SetPixelSize( nmPixelSize ) )
	{
		//EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer( );
		//m_EvoPixelCoords.CenterGrid( gpCenter, GetClRectSize( ) ); // center grid around gpCenter
		//m_DrawFrame.ResizeDrawFrame( pCore );                        // trigger DrawFrame to adapt font size etc.
		//PixelPoint const ppCrsr = GetRelativeCrsrPosition( );
		//if ( IsInClientRect( ppCrsr ) )
		//	m_DrawFrame.SetHighlightPos( pCore, ppCrsr );  
		//m_pReadBuffer->ReleaseReadBuffer( );
		Notify( TRUE );     // cause immediate repaint
	}
	else
	{
		MessageBeep( MB_ICONWARNING );
	}
}

void NNetWindow::SetPixelSize( NanoMeter const nmNewSize )
{
	PixelPoint      const pixPoint  = GetClRectCenter( );
	MicroMeterPoint const umCenter  = m_coord.convert2MicroMeterPoint( pixPoint );
	m_coord.SetPixelSize( nmNewSize );
	newPixelSize( nmNewSize, umCenter );
}

NanoMeter NNetWindow::GetPixelSize( ) const
{
	return m_coord.GetPixelSize( );
}

void NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu, POINT const pntPos )
{
	PixelPoint      const   pp        = Util::POINT2PixelPoint( pntPos );
	MicroMeterPoint const   umCrsrPos = m_coord.convert2MicroMeterPoint( pp );
	Shape           const * pShape;
	{
		NNetModel const * pModel = m_pReadBuffer->LockReadBuffer( );
		pShape = pModel->GetShapeUnderPoint( umCrsrPos );
		m_pReadBuffer->ReleaseReadBuffer( );
	}
	if ( pShape )
	{
		UINT const STD_FLAGS = MF_BYPOSITION | MF_STRING;
		switch ( pShape->GetShapeType( ) )
		{
		case tShapeType::inputNeuron:
			m_pShapeSelected = pShape;
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_RATE_DIALOG, L"Pulse rate" );
			break;

		case tShapeType::knot:
			break;

		case tShapeType::neuron:
			break;

		case tShapeType::pipeline:
			m_pShapeSelected = pShape;
			(void)AppendMenu( hPopupMenu, STD_FLAGS, IDD_PULSE_SPEED_DIALOG, L"Conduction velocity" );
			break;

		default:
			assert( false );
		}
	}
}

void NNetWindow::PulseRateDialog( )
{
	InputNeuron const * pInputNeuron = Cast2InputNeuron( m_pShapeSelected );
	Hertz       const   pulseRateOld = pInputNeuron->GetPulseFrequency();
	double dNewValue = StdDialogBox::Show
	( 
		GetWindowHandle(),
		static_cast<double>( pulseRateOld.GetValue() ),
		L"Pulse rate",
		L"Hertz"
	);
	PostCommand2Application
	( 
		IDM_PULSE_FREQ, 
		Util::Pack2UINT64
		(
			m_pShapeSelected->GetId().GetValue(), 
			CastToUnsignedLong( dNewValue ) 
		) 
	);
}

void NNetWindow::PulseSpeedDialog( )
{
	Pipeline    const * pPipeline     = Cast2Pipeline( m_pShapeSelected );
	meterPerSec const   pulseSpeedOld = pPipeline->GetPulseSpeed();
	double dNewValue = StdDialogBox::Show
	( 
		GetWindowHandle(),
		static_cast<double>( pulseSpeedOld.GetValue() ),
		L"Conduction velocity",
		L"m/sec"
	);
	meterPerSec const pulseSpeedNew( dNewValue );
	PostCommand2Application
	( 
		IDM_PULSE_SPEED, 
		Util::Pack2UINT64
		( 
			m_pShapeSelected->GetId().GetValue(), 
			Convert2milliMeterPerSec(pulseSpeedNew).GetValue() 
		) 
	);
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );  // relative to client area

	if ( wParam & MK_RBUTTON )           // Right mouse button: selection
	{
	}
	else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
	{
		if ( m_ptLast.IsNotNull() )  // last cursor pos stored in m_ptLast
		{
			moveNNet( ptCrsr - m_ptLast );
		}
		m_ptLast = ptCrsr;
		PostCommand2Application( IDM_REFRESH, 0 );
	}
	else
	{
		MicroMeterPoint const   umCrsrPos = m_coord.convert2MicroMeterPoint( ptCrsr );
		Shape           const * pShape;
		{
			NNetModel const * pModel = m_pReadBuffer->LockReadBuffer( );
			pShape = pModel->GetShapeUnderPoint( umCrsrPos );
			m_pReadBuffer->ReleaseReadBuffer( );
		}
		ShapeId shapeId = pShape ? pShape->GetId() : NO_SHAPE;
		PostCommand2Application( IDM_HIGHLIGHT, shapeId.GetValue() );

		m_ptLast = PP_NULL;    // make m_ptLast invalid
							   // no refresh! It would cause repaint for every mouse move.
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

void NNetWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );

		if ( m_pGraphics->StartFrame( GetWindowHandle(), hDC ) )
		{
			NNetModel const * pModel = m_pReadBuffer->LockReadBuffer( );
			pModel->Apply2AllShapes
			( 
				[&]( Shape * const pShape ) 
				{ 
					pShape->Draw( * m_pGraphics, m_coord );	
				} 
			);
			m_pReadBuffer->ReleaseReadBuffer( );

			m_pScale->ShowScale( fPIXEL( static_cast<double>( GetClientWindowHeight().GetValue() ) ) );
			m_pGraphics->RenderForegroundObjects( );

			m_pGraphics->EndFrame( GetWindowHandle() );
		}

		(void)EndPaint( &ps );
	}
}

void NNetWindow::OnMouseWheel( WPARAM const wParam, LPARAM const lParam )
{
	int        iDelta     = GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA;
	BOOL const bDirection = ( iDelta > 0 );
	NanoMeter  nmNewPixelSize;

	iDelta = abs( iDelta );

	while ( --iDelta >= 0 )
	{
		nmNewPixelSize = m_coord.ComputeNewPixelSize( bDirection );
	}

	PostCommand2Application( IDM_SET_ZOOM, static_cast<LPARAM>( nmNewPixelSize.GetValue() ) ); 
}

void NNetWindow::OnLButtonDown( WPARAM const wParam, LPARAM const lParam )
{
	if ( inObservedClientRect( lParam ) || m_bMoveAllowed )
		OnMouseMove( wParam, lParam );
}

void NNetWindow::OnLButtonUp( WPARAM const wParam, LPARAM const lParam )
{
	(void)ReleaseCapture( );
}

void NNetWindow::OnSetCursor( WPARAM const wParam, LPARAM const lParam )
{
	BOOL    const   keyDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	HCURSOR const   hCrsr   = keyDown ? m_hCrsrMove : m_hCrsrArrow;
	SetCursor( hCrsr );
}

BOOL NNetWindow::OnCommand( WPARAM const wParam, LPARAM const lParam )
{
	UINT uiCmdId = LOWORD( wParam );
	switch ( uiCmdId )
	{
	case 0:
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
