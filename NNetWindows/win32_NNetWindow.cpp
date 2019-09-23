// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Segment.h"
#include "Pipeline.h"
#include "NNetPixelCoords.h"
#include "win32_util_resource.h"
#include "win32_graphicsInterface.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWindow.h"

NNetReadBuffer          * NNetWindow::m_pReadBuffer             = nullptr;
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
	m_NNetPixelCoords.Start( npPixelSize );
	m_pGraphics = pGraphics;

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
//			m_NNetPixelCoords.NNet2PixelRect( GridDimensions::GridRectFull() ),
//			(DWORD)GetWindowLongPtr( GetWindowHandle( ), GWL_STYLE ) 
//		), 
//		FALSE 
//	);
//}

void NNetWindow::Zoom( bool const bZoomIn  )
{
	SetPixelSize( m_NNetPixelCoords.ComputeNewPixelSize( bZoomIn ) );
}

void NNetWindow::newPixelSize
( 
	NanoMeter const nmPixelSize, 
	NNetPoint const npCenter 
)
{
	if ( m_NNetPixelCoords.SetPixelSize( nmPixelSize ) )
	{
		//EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer( );
		//m_EvoPixelCoords.CenterGrid( gpCenter, GetClRectSize( ) ); // center grid around gpCenter
		//m_DrawFrame.ResizeDrawFrame( pCore );                        // trigger DrawFrame to adapt font size etc.
		//PixelPoint const ppCrsr = GetRelativeCrsrPosition( );
		//if ( IsInClientRect( ppCrsr ) )
		//	m_DrawFrame.SetHighlightPos( pCore, ppCrsr );  
		//m_pReadBuffer->ReleaseReadBuffer( );
		Trigger( );   // cause repaint
	}
	else
	{
		MessageBeep( MB_ICONWARNING );
	}
}

void NNetWindow::SetPixelSize( NanoMeter const nmNewSize )
{
	NNetPoint const npCenter = m_NNetPixelCoords.Pixel2NNetPos( GetClRectCenter( ) );
	m_NNetPixelCoords.SetPixelSize( nmNewSize );
	newPixelSize( nmNewSize, npCenter );
}

NanoMeter NNetWindow::GetPixelSize( ) const
{
	return m_NNetPixelCoords.GetPixelSize( );
}

void NNetWindow::AddContextMenuEntries( HMENU const hPopupMenu, POINT const pntPos )
{
}

void NNetWindow::OnMouseMove( WPARAM const wParam, LPARAM const lParam )
{
	PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );  // relative to client area

	if ( wParam & MK_RBUTTON )                // Right mouse button: selection
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
		m_NNetPixelCoords.MoveNNet( ptDiff );
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

			Segment segment;
			Pipeline const * pPipeline = pModel->GetPipeline( );
			int iPotential;
			unsigned int uiSegmentNr = 0;
			while ( pPipeline->GetSegment( uiSegmentNr, segment, iPotential ) )
			{
				fPixelPoint const fPixPoint1( m_NNetPixelCoords.NNet2fPixelPos( segment.GetStartPoint() ) );
				fPixelPoint const fPixPoint2( m_NNetPixelCoords.NNet2fPixelPos( segment.GetEndPoint  () ) );
				PIXEL      const pixWidth ( m_NNetPixelCoords.Nm2Pixel( segment.GetWidth() ) );
				COLORREF   const color = RGB(   0, 255 - iPotential, 0 );
				m_pGraphics->AddfPixelLine( fPixPoint1, fPixPoint2, pixWidth, color );
				++ uiSegmentNr;
			}
			m_pReadBuffer->ReleaseReadBuffer( );

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
		nmNewPixelSize = m_NNetPixelCoords.ComputeNewPixelSize( bDirection );
	}

	LPARAM lParamPixelSize = static_cast<LPARAM>( nmNewPixelSize.GetValue() );
	PostCommand2Application( IDM_SET_ZOOM, lParamPixelSize );
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

	default:
		return TRUE;
	}
	return FALSE;
}

BOOL NNetWindow::inObservedClientRect( LPARAM const lParam )
{
	return TRUE;  // Is cursor position in observed client rect?
}
