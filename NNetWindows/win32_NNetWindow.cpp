// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include "NNetPixelCoords.h"
#include "win32_util_resource.h"
#include "win32_graphicsInterface.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_NNetWindow.h"

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
	nm                    const npPixelSize,
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

void NNetWindow::Zoom( bool const )
{

}

void NNetWindow::newPixelSize
( 
	nm        const nmPixelSize, 
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

void NNetWindow::SetPixelSize( nm const nmNewSize )
{
	NNetPoint const npCenter = m_NNetPixelCoords.Pixel2NNetPos( GetClRectCenter( ) );
	m_NNetPixelCoords.SetPixelSize( nmNewSize );
	newPixelSize( nmNewSize, npCenter );
}

nm NNetWindow::GetPixelSize( ) const
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
		static COLORREF const CLR_GREEN = RGB(   0, 128, 0 );

		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );

		if ( m_pGraphics->StartFrame( GetWindowHandle(), hDC ) )
		{
			NNetPoint const nnetPoint1(  20e3_nm,  20e3_nm );
			NNetPoint const nnetPoint2( 800e3_nm, 400e3_nm );
			nm        const nmWidth( 40e3_nm );

			PixelPoint const pixPoint1( m_NNetPixelCoords.NNet2PixelPos( nnetPoint1 ) );
			PixelPoint const pixPoint2( m_NNetPixelCoords.NNet2PixelPos( nnetPoint2 ) );
			PIXEL      const pixWidth ( m_NNetPixelCoords.Nm2Pixel( nmWidth ) );

			m_pGraphics->AddLine( pixPoint1, pixPoint2, pixWidth, CLR_GREEN );
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
	nm         nmNewPixelSize;

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
