// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "Resource.h"
#include "Segment.h"
#include "Pipeline.h"
#include "PixelTypes.h"
#include "fPixelCoords.h"
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
	m_ptLast( PP_NULL ),
	m_bMoveAllowed( TRUE )
{ }

void NNetWindow::Start
( 
	HWND                  const hwndApp, 
	GraphicsInterface   * const pGraphics,
	DWORD                 const dwStyle,
	NanoMeter             const npPixelSize,
	std::function<bool()> const visibilityCriterion
)
{
	m_coord.Start( npPixelSize );
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
		{
			MicroMeterPoint const   umCrsrPos = m_coord.convert2MicroMeterPoint( ptCrsr );
			Shape           const * pShape;
			{
				NNetModel const * pModel = m_pReadBuffer->LockReadBuffer( );
				pShape = pModel->GetShapeUnderPoint( umCrsrPos );
				if ( pShape )
				{
					int x = 8765;
				}
				m_pReadBuffer->ReleaseReadBuffer( );
			}
			ShapeId shapeId = pShape ? pShape->GetId() : NO_SHAPE;
			PostCommand2Application( IDM_HIGHLIGHT, shapeId.GetValue() );
		}

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
			fPIXEL fPixWidth;
			NNetModel const * pModel = m_pReadBuffer->LockReadBuffer( );
			
			// Pipeline

			Segment          segment;
			mV               potential;
			Pipeline const * pPipeline = pModel->GetPipeline( );
			unsigned int     uiSegmentNr = 0;
			while ( pPipeline->GetSegment( uiSegmentNr, segment, potential ) )
			{
				fPixelPoint const fPixPoint1( m_coord.convert2fPixelPos( segment.GetStartPoint() ) );
				fPixelPoint const fPixPoint2( m_coord.convert2fPixelPos( segment.GetEndPoint  () ) );
				                  fPixWidth = fPIXEL( m_coord.convert2fPixel( segment.GetWidth() ) );
				int         const iLevel    = 255 - CastToInt( potential.GetValue() );
				COLORREF    const color     = pPipeline->IsHighlighted( )
					                          ? RGB(   iLevel, 0, iLevel )
                                              : RGB(   0, iLevel, 0 );
				m_pGraphics->AddfPixelLine( fPixPoint1, fPixPoint2, fPixWidth, color );
				++ uiSegmentNr;
			}

			fPIXEL const fPixNeuronSize = fPixWidth * 5.0;

			// Neuron 
			{
				Neuron   const * pNeuron = pModel->GetNeuron( );
				COLORREF const   color   = pNeuron->IsHighlighted( )
					? RGB( 255, 200, 200 )
					: RGB( 128, 128, 128 );
				m_pGraphics->AddRect
				( 
					m_coord.convert2fPixelPos( pNeuron->GetPosition() ), 
					color, 
					m_coord.convert2fPixel( pNeuron->GetExtension() )
				);
			}

			// Knot
			{
				Knot     const * pKnot = pModel->GetKnot( );
				COLORREF const   color = pKnot->IsHighlighted( )
					? RGB( 255, 200, 200 )
					: RGB( 128, 128, 128 );
				m_pGraphics->AddRect
				( 
					m_coord.convert2fPixelPos( pKnot->GetPosition() ), 
					color, 
					m_coord.convert2fPixel( pKnot->GetExtension() )
				);
			}

			m_pReadBuffer->ReleaseReadBuffer( );

			m_pScale->ShowScale( fPIXEL( static_cast<double>( GetClientWindowHeight().GetValue() ) ) );

			m_pGraphics->RenderIndividuals( );

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
