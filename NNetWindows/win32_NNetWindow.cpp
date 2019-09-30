// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "Segment.h"
#include "Pipeline.h"
#include "NNetPixelCoords.h"
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
		Refresh( );   // cause repaint
	}
	else
	{
		MessageBeep( MB_ICONWARNING );
	}
}

void NNetWindow::SetPixelSize( NanoMeter const nmNewSize )
{
	PixelPoint  const pixPoint = GetClRectCenter( );
	fPIXEL      const fpX( static_cast<double>(pixPoint.GetXvalue()) );
	fPIXEL      const fpY( static_cast<double>(pixPoint.GetYvalue()) );
	fPixelPoint const fPixPoint( fpX, fpY );
	NNetPoint   const npCenter = m_NNetPixelCoords.fPixel2NNetPos( fPixPoint );
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

			Segment          segment;
			mV               potential;
			Pipeline const * pPipeline = pModel->GetPipeline( );
			unsigned int     uiSegmentNr = 0;
			while ( pPipeline->GetSegment( uiSegmentNr, segment, potential ) )
			{
				fPixelPoint const fPixPoint1( m_NNetPixelCoords.NNet2fPixelPos( segment.GetStartPoint() ) );
				fPixelPoint const fPixPoint2( m_NNetPixelCoords.NNet2fPixelPos( segment.GetEndPoint  () ) );
				fPIXEL      const fPixWidth ( m_NNetPixelCoords.MicroMeter2fPixel( segment.GetWidth() ) );
				COLORREF    const color = RGB(   0, 255 - potential.GetValue(), 0 );
				m_pGraphics->AddfPixelLine( fPixPoint1, fPixPoint2, fPixWidth, color );
				++ uiSegmentNr;
			}

			m_pReadBuffer->ReleaseReadBuffer( );
			showScale( hDC );

			m_pGraphics->RenderIndividuals( );

			m_pGraphics->EndFrame( GetWindowHandle() );
		}

		(void)EndPaint( &ps );
	}
}

void NNetWindow::showScale( HDC hDC )
{
	m_pGraphics->SetFontSize( 12_PIXEL );

	fPIXEL height( static_cast<double>( GetClientWindowHeight().GetValue() ) );
	fPIXEL const vertPos    = height - 20._fPIXEL;
	fPIXEL const horzPos    = 100._fPIXEL;
	fPIXEL const lengthMax  = 500._fPIXEL;

	double           dIntegerPart;
	MicroMeter const umLengthExact = m_NNetPixelCoords.fPixel2MicroMeter( lengthMax );
	double     const logValue      = log10( umLengthExact.GetValue() );
	double     const fractPart     = modf( logValue, & dIntegerPart );
	double     const nextPowerOf10 = pow( 10.0, dIntegerPart );
	MicroMeter const umLength      = MicroMeter( nextPowerOf10 );

	int iFirstDigit = ( fractPart >= log10( 5 ) ) 
		              ? 5
		              : ( fractPart >= log10( 2 ) )
                        ? 2
		                : 1;

	fPIXEL      const fPixLength = m_NNetPixelCoords.MicroMeter2fPixel( umLength * iFirstDigit );
	fPixelPoint const fPixPoint1( horzPos, vertPos );
	fPixelPoint const fPixPoint2( horzPos + fPixLength, vertPos );

	m_pGraphics->AddfPixelLine( fPixPoint1, fPixPoint2, 1._fPIXEL, SCALE_COLOR );
	displayTicks( fPixPoint1, fPixPoint2, dIntegerPart, iFirstDigit );
	displayScaleText( fPixPoint2, dIntegerPart );
}

void NNetWindow::displayTicks( fPixelPoint const fPixPoint1, fPixelPoint const fPixPoint2, double const dLog10, int const iFirstDigit )
{
	fPixelPoint fLongTick  (  0._fPIXEL, 10._fPIXEL );
	fPixelPoint fMiddleTick(  0._fPIXEL,  7._fPIXEL );
	fPixelPoint fSmallTick (  0._fPIXEL,  5._fPIXEL );

	fPixelPoint fTickPos( fPixPoint1 );
	fPixelPoint fTickDist( (fPixPoint2.GetX() - fPixPoint1.GetX()) / 10, 0._fPIXEL );

	m_pGraphics->AddfPixelLine( fPixPoint1 - fLongTick, fTickPos, 1._fPIXEL, SCALE_COLOR );

	displayScaleNumber( fTickPos, dLog10, 0 );

	if ( iFirstDigit == 1 )
	{
		for ( int i = 1; i <= 4; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}

		fTickPos += fTickDist;
		m_pGraphics->AddfPixelLine( fTickPos - fMiddleTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		displayScaleNumber( fTickPos, dLog10 - 1.0, 5 );

		for ( int i = 6; i <= 9; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}
	}
	else if ( iFirstDigit == 2 )
	{
		for ( int i = 1; i <= 4; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}

		fTickPos += fTickDist;
		m_pGraphics->AddfPixelLine( fTickPos - fMiddleTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		displayScaleNumber( fTickPos, dLog10, 1 );

		for ( int i = 6; i <= 9; ++i )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
		}
	}
	else if ( iFirstDigit == 5 )
	{
		for ( int i = 0;; )
		{
			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fSmallTick, fTickPos, 1._fPIXEL, SCALE_COLOR );

			if ( ++i > 4 )
				break;

			fTickPos += fTickDist;
			m_pGraphics->AddfPixelLine( fTickPos - fMiddleTick, fTickPos, 1._fPIXEL, SCALE_COLOR );
			displayScaleNumber( fTickPos, dLog10, i );
		}
	}
	else 
		assert( false );

	displayScaleNumber( fPixPoint2, dLog10, iFirstDigit );
	m_pGraphics->AddfPixelLine( fPixPoint2 - fLongTick, fPixPoint2, 1._fPIXEL, SCALE_COLOR );
}

void NNetWindow::displayScaleNumber( fPixelPoint const fPos, double const dLog10, int const iFirstDigit )
{
	static PIXEL const textWidth  = 40_PIXEL;
	static PIXEL const textHeight = 20_PIXEL;
	static PIXEL const horzDist   =  2_PIXEL;
	static PIXEL const vertDist   = 12_PIXEL;

	PIXEL posX = PIXEL(static_cast<long>(fPos.GetXvalue()));
	PIXEL posY = PIXEL(static_cast<long>(fPos.GetYvalue()));

	PixelRect pixRect
	( 
		posX + horzDist - textWidth,  // left
		posY - vertDist - textHeight, // top
		posX + horzDist + textWidth,  // right
		posY - vertDist               // bottom
	);

	m_wBuffer.str( std::wstring() );
	m_wBuffer.clear();

	m_wBuffer << iFirstDigit;

	if ( iFirstDigit > 0 )
	{
		int iLog10  = static_cast<int>( floor(dLog10) );
		int nDigits = iLog10 % 3;

		while ( nDigits-- )
			m_wBuffer << L"0";
	}

	m_pGraphics->DisplayGraphicsText( pixRect, m_wBuffer.str( ), DT_CENTER, SCALE_COLOR );
}

void NNetWindow::displayScaleText( fPixelPoint const fPos, double const dLog10 )
{
	static PIXEL const textWidth  = 40_PIXEL;
	static PIXEL const textHeight = 20_PIXEL;
	static PIXEL const horzDist   = 16_PIXEL;
	static PIXEL const vertDist   = 12_PIXEL;

	PIXEL posX = PIXEL(static_cast<long>(fPos.GetXvalue()));
	PIXEL posY = PIXEL(static_cast<long>(fPos.GetYvalue()));

	PixelRect pixRect
	( 
		posX + horzDist,              // left
		posY - vertDist - textHeight, // top
		posX + horzDist + textWidth,  // right
		posY - vertDist               // bottom
	);

	m_wBuffer.str( std::wstring() );
	m_wBuffer.clear();

	if ( dLog10 < 3 )
		m_wBuffer << L"\u03BCm";
	else if ( dLog10 < 6 )
		m_wBuffer << L"mm";
	else
		m_wBuffer << L"m";

	m_pGraphics->DisplayGraphicsText( pixRect, m_wBuffer.str( ), DT_LEFT, SCALE_COLOR );
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
