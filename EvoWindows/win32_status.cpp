// win32_status.cpp : Verwaltet die Status Bar
//
// EvoWindows

#include "stdafx.h"
#include <array>
#include "Resource.h"
#include "commctrl.h"
#include "config.h"
#include "pixelCoordinates.h"
#include "EvolutionCore.h"
#include "EvoHistorySysGlue.h"
#include "EvoReadBuffer.h"
#include "win32_delay.h"
#include "win32_tooltip.h"
#include "win32_EvoWorkThreadInterface.h"
#include "win32_editor.h"
#include "win32_status.h"

extern int g_AllocHookCounter;

static long trackBar2Value( LONG ); 
static LONG value2Trackbar( long );  

static DWORD const MAX_DELAY = 2048;    // in msecs

static double const TRACKBAR_SCALING_FACTOR = 1000.0;

static LONG const SPEED_TRACKBAR_MIN = 0; 
static LONG const SPEED_TRACKBAR_MAX = value2Trackbar( MAX_DELAY ); 

static PIXEL const STATUS_BAR_HEIGHT = 22_PIXEL;

static wchar_t * SZ_RUN_MODE  = L"   Run    ";
static wchar_t * SZ_STOP_MODE = L"  Stop    ";
static wchar_t * SZ_EDIT_MODE = L"Show editor";

StatusBar::StatusBar()
:	m_pDelay( nullptr ),
	m_pReadBuffer( nullptr ),
	m_pEditorWindow( nullptr ),
	m_pEvoHistorySys( nullptr ),
	m_pWorkThreadInterface( nullptr )
{ }

void StatusBar::Start
( 
	HWND                           const hwndParent,
	EvoReadBuffer                * const pReadBuffer,
	EvoHistorySysGlue      const * const pEvoHistorySys,
	EvoWorkThreadInterface const * const pWorkThreadInterface,
	Delay                        * const pDelay,
	EditorWindow                 * const pEditorWindow
)
{
	m_pDelay               = pDelay;
	m_pReadBuffer          = pReadBuffer;
	m_pEditorWindow        = pEditorWindow;
	m_pEvoHistorySys       = pEvoHistorySys;
	m_pWorkThreadInterface = pWorkThreadInterface;

	HWND hwndStatus = CreateWindow
	(
		STATUSCLASSNAME, 
		nullptr, 
		WS_CHILD,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, STATUS_BAR_HEIGHT.GetValue(),
		hwndParent,
		nullptr, 
		GetModuleHandle( nullptr ), 
		nullptr
	); 

	SetWindowHandle( hwndStatus );

	static std::array< PIXEL, static_cast<int>( tPart::Stop ) + 1> statwidths = 
	{ 
		100_PIXEL, // Generation 
		200_PIXEL, // Mode (Edit/Simu)
		400_PIXEL, // Size
		670_PIXEL, // Simu controls
		600_PIXEL, // ScriptLine
		-1_PIXEL   // Stop
	};

	CreateRectToolTip
	( 
		0, 
		& PixelRect( 0_PIXEL, 0_PIXEL, statwidths[0], STATUS_BAR_HEIGHT ), 
		L"Generation number. Is incremented at every computed generation, not influenced by editor operations." 
	);

	PIXEL pixPartWidth = statwidths[0];
	for ( int i = 1; i < static_cast<int>( tPart::Stop ); ++i )
	{
		statwidths[i] += pixPartWidth;
		pixPartWidth = statwidths[i];
	}

	(void)SetWindowSubclass( hwndStatus, OwnerDrawStatusBar, 0, (DWORD_PTR)this ) ;

	m_pixBorderX      = PIXEL(PIXEL(GetSystemMetrics( SM_CXSIZEFRAME ))) + 10_PIXEL;
	m_pixBorderY      = PIXEL(PIXEL(GetSystemMetrics( SM_CYSIZEFRAME )));
	m_pixClientHeight = GetHeight( ) - m_pixBorderY;

	m_pixPosX = statwidths[ static_cast<int>( tPart::Mode ) - 1 ] + m_pixBorderX;
	(void)createButton( SZ_EDIT_MODE, (HMENU)IDM_EDIT_WINDOW, BS_PUSHBUTTON );  

	m_pixPosX = statwidths[ static_cast<int>( tPart::Size ) - 1 ] + m_pixBorderX;
	createSizeControl( );

	m_pixPosX = statwidths[ static_cast<int>( tPart::SimuCtl ) - 1 ] + m_pixBorderX;
	createSimulationControl( );

	(void)SendMessage( SB_SETPARTS, sizeof( statwidths ) / sizeof( int ), (LPARAM)( &statwidths ) );

	SetSizeTrackBar( DEFAULT_FIELD_SIZE );

	{
		long lDefaultDelay = Config::GetConfigValue( Config::tId::generationDelay );
		SetSpeedTrackBar( lDefaultDelay );
		m_pDelay->SetDelay( lDefaultDelay );
	}

	m_pReadBuffer->RegisterObserver( this );   // notify me, if model has changed
	m_pEditorWindow->RegisterObserver( this ); // notify me, if editor window has been closed or opened
}

void StatusBar::Stop( )
{
	Show( FALSE );
	DestroyWindow( );
}

LRESULT StatusBar::UserProc
( 
	UINT   const uMsg, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	return DefSubclassProc( GetWindowHandle(), uMsg, wParam, lParam );
}

static LRESULT CALLBACK OwnerDrawStatusBar( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
    StatusBar * const pStatusBar = (StatusBar *)dwRefData;
    switch ( uMsg )
    {
    case WM_PAINT:
		{
			EvoReadBuffer         * pReadBuffer = pStatusBar->m_pReadBuffer;
			EvolutionCore   const * pCore       = pReadBuffer->LockReadBuffer( );
			EVO_GENERATION          evoGen      = pCore->GetEvoGenerationNr( );
			pReadBuffer->ReleaseReadBuffer( );
			pStatusBar->DisplayCurrentGeneration( evoGen );
			pStatusBar->adjust();
		}
        break;

    case WM_COMMAND:
		{
			int const wmId = LOWORD( wParam );
			switch (wmId)
			{
			case IDM_MAX_SPEED:
				pStatusBar->maxSpeed( );
				break;

			default:
				pStatusBar->PostCommand2Application( LOWORD(wParam), 0 );
				break;
			}
		}
        return FALSE;

    case WM_HSCROLL:
		pStatusBar->scrollBarMessage( GetDlgCtrlID( (HWND)lParam ) );
        return TRUE;

    default: 
        break;
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

HWND WINAPI StatusBar::createControl
(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD   dwStyle,
    HMENU   hMenu
)
{
    PIXEL const pixWidth = PIXEL(PIXEL(static_cast<int>( wcslen( lpWindowName ) ) * 9));
    HWND  const hwnd     = CreateWindow
    (
        lpClassName,                     // class name 
        lpWindowName,                    // title (caption) 
        WS_CHILD | WS_VISIBLE | dwStyle, // style 
        m_pixPosX.GetValue(),            // x position
		m_pixBorderY.GetValue(),         // y position 
        pixWidth.GetValue(),             // width
		m_pixClientHeight.GetValue(),    // height
        GetWindowHandle( ),              // parent window 
        hMenu,                           // control identifier 
        GetModuleHandle( nullptr ),      // instance 
        nullptr                          // no WM_CREATE parameter 
    );
	m_pixPosX += pixWidth;
	return hwnd;
}

HWND WINAPI StatusBar::createStaticControl( LPCTSTR lpWindowName )
{
    HWND hwnd = createControl( WC_STATIC, lpWindowName, 0, nullptr );
	return hwnd;
}

HWND WINAPI StatusBar::createButton( LPCTSTR const lpWindowName, HMENU const hMenu, DWORD const dwStyle )
{ 
	HWND hwnd =  createControl( WC_BUTTON, lpWindowName, dwStyle, hMenu );
	return hwnd;
}

HWND WINAPI StatusBar::createTrackBar( HMENU hMenu )
{ 
	HWND hwnd =  createControl( TRACKBAR_CLASS, L"   Trackbar Control   ", WS_TABSTOP | WS_BORDER | TBS_NOTICKS, hMenu );
	return hwnd;
};

void WINAPI StatusBar::createSizeControl( )
{ 
    createStaticControl( L"Size" );
    createButton       ( L" - ",     (HMENU)IDM_ZOOM_OUT, BS_PUSHBUTTON ); 
    createTrackBar     (             (HMENU)IDM_ZOOM_TRACKBAR ); 
    createButton       ( L" + ",     (HMENU)IDM_ZOOM_IN,  BS_PUSHBUTTON ); 
    createButton       ( L"  Fit  ", (HMENU)IDM_FIT_ZOOM, BS_PUSHBUTTON ); 

    LONG const lMinPos = value2Trackbar( MINIMUM_FIELD_SIZE.GetValue() );
    LONG const lMaxPos = value2Trackbar( MAXIMUM_FIELD_SIZE.GetValue() );

    SetTrackBarRange( IDM_ZOOM_TRACKBAR, lMinPos, lMaxPos );  

	CreateBalloonToolTip( IDM_ZOOM_OUT,      L"Zoom out one step" );
	CreateBalloonToolTip( IDM_ZOOM_TRACKBAR, L"Move slider to zoom in or out" );
	CreateBalloonToolTip( IDM_ZOOM_IN,       L"Zoom in one step" );
	CreateBalloonToolTip( IDM_FIT_ZOOM,      L"Zoom to maximum possible size, which fits in actual window" );
} 

void WINAPI StatusBar::createSimulationControl( )
{ 
    if ( Config::UseHistorySystem( ) )
        createButton  ( L"Backwards ", (HMENU)IDM_BACKWARDS, BS_PUSHBUTTON );

    createButton  ( L"SingleStep", (HMENU)IDM_FORWARD, BS_PUSHBUTTON ); 
	createButton  ( SZ_RUN_MODE,   (HMENU)IDM_RUN,        BS_PUSHBUTTON ); 
	createButton  ( SZ_STOP_MODE,  (HMENU)IDM_STOP,       BS_PUSHBUTTON ); 
	createTrackBar(                (HMENU)IDM_SIMULATION_SPEED ); 
    createButton  ( L" MaxSpeed ", (HMENU)IDM_MAX_SPEED,  BS_PUSHBUTTON ); 

    SetTrackBarRange( IDM_SIMULATION_SPEED, SPEED_TRACKBAR_MIN, SPEED_TRACKBAR_MAX );
} 

void StatusBar::SetSizeTrackBar( PIXEL const pixFieldSize  ) const 
{ 
    SetTrackBarPos( IDM_ZOOM_TRACKBAR, value2Trackbar( pixFieldSize.GetValue() ) ); 
}

void StatusBar::SetSpeedTrackBar( DWORD const dwDelay ) const 
{ 
    LONG const lPos = ( dwDelay == 0 )
                      ? 0
                      : value2Trackbar( dwDelay );
    SetTrackBarPos( IDM_SIMULATION_SPEED, SPEED_TRACKBAR_MAX - lPos );                
}

static long trackBar2Value( LONG lX ) // f(x) = 2 power (x/1000)
{
    double const dX = static_cast<double>( lX ) / TRACKBAR_SCALING_FACTOR;
    double dRes = pow( 2.0, dX );
	return static_cast<long>( dRes );
}

static LONG value2Trackbar( long lX )  // f(x) = 1000 * log2(x)
{
    static double const dFactor = TRACKBAR_SCALING_FACTOR / log( 2 );

    assert( lX > 0 );
	double const dX = static_cast<double>( lX );
    double const dY = log( dX ) * dFactor;
    return static_cast<LONG>( dY );
}

void StatusBar::maxSpeed()
{
	SetSpeedTrackBar( 0 );
	m_pDelay->SetDelay( 0 );
	EnableWindow( GetDlgItem( IDM_MAX_SPEED  ), FALSE );
}

void StatusBar::scrollBarMessage( int const iCtrlId )
{
	LONG const lLogicalPos = GetTrackBarPos( iCtrlId );
	if ( iCtrlId == IDM_ZOOM_TRACKBAR )
	{
		LONG const lValue = lLogicalPos;
		LONG const lPos   = trackBar2Value( lValue );
		PostCommand2Application( IDM_ZOOM_TRACKBAR, lPos );
	}
	else
	{
		LONG const lValue = SPEED_TRACKBAR_MAX - lLogicalPos;
		LONG const lPos   = trackBar2Value( lValue );
		m_pDelay->SetDelay( lPos );
		EnableWindow( GetDlgItem( IDM_MAX_SPEED ), TRUE );
	}
}

void StatusBar::adjust( )
{
	BOOL const bRunMode = m_pWorkThreadInterface->IsRunning();

	EnableWindow( GetDlgItem( IDM_RUN  ), ! bRunMode );
	EnableWindow( GetDlgItem( IDM_STOP ),   bRunMode );

	EnableWindow( GetDlgItem( IDM_FORWARD ), ! bRunMode );

	ShowWindow( GetDlgItem( IDM_EDIT_WINDOW ), ! m_pEditorWindow->IsWindowVisible( ) );

	if ( Config::UseHistorySystem( ) )
		EnableWindow( GetDlgItem( IDM_BACKWARDS ), (! bRunMode) && (! m_pEvoHistorySys->IsFirstHistGen() ) );
}

PIXEL StatusBar::GetHeight( ) const
{
    return STATUS_BAR_HEIGHT;
}
    
void StatusBar::Resize( ) const 
{
    (void)SendNotifyMessage( WM_SIZE, 0, 0 );
}

void StatusBar::ClearStatusLine( )
{
    DisplayStatusLine( L"" );
}

void StatusBar::DisplayStatusLine( std::wstring const & wstrLine )
{
    (void)SendNotifyMessage( SB_SETTEXT, static_cast<int>( tPart::ScriptLine ), (LPARAM)( wstrLine.c_str( ) ) );
}

void StatusBar::DisplayScriptLine( std::wstring const & wszPath, int iLineNr, std::wstring const & wstrLine )
{
    m_wstrScriptLine = wszPath + L"(" + std::to_wstring( iLineNr ) + L"): " + wstrLine;
    DisplayStatusLine( m_wstrScriptLine );
}

void StatusBar::DisplayCurrentGeneration( EVO_GENERATION const gen )
{
    m_wstrGeneration = L"EvoGen " + std::to_wstring( gen.GetValue() );
    (void)SendNotifyMessage( SB_SETTEXT, static_cast<int>( tPart::Generation ), (LPARAM)( m_wstrGeneration.c_str() ) );
}
