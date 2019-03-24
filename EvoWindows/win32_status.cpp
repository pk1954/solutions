// win32_status.cpp : Verwaltet die Status Bar
//

#include "stdafx.h"
#include <array>
#include "Resource.h"
#include "commctrl.h"
#include "config.h"
#include "pixelCoordinates.h"
#include "EvolutionCore.h"
#include "win32_readBuffer.h"
#include "win32_performanceWindow.h"
#include "win32_status.h"

//lint -e1924    C-style casts

static long trackBar2Value( LONG ); 
static LONG value2Trackbar( long );  

static DWORD const MAX_DELAY = 2048;    // in msecs

static double const TRACKBAR_SCALING_FACTOR = 1000.0;

static LONG const SPEED_TRACKBAR_MIN = 0; 
static LONG const SPEED_TRACKBAR_MAX = value2Trackbar( MAX_DELAY ); 

static PIXEL_Y const STATUS_BAR_HEIGHT = 22_PIXEL_Y;

static wchar_t * SZ_RUN_MODE  = L"   Run    ";
static wchar_t * SZ_STOP_MODE = L"  Stop    ";
static wchar_t * SZ_SIMU_MODE = L"Switch to SIMULATION";
static wchar_t * SZ_EDIT_MODE = L"Switch to EDITOR";

//lint -esym( 715, uIdSubclass )    symbol not referenced

static LRESULT CALLBACK OwnerDrawStatusBar( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
    StatusBar * const pStatusBar = (StatusBar *)dwRefData;
    switch ( uMsg )
    {
    case WM_PAINT:
		{
			ReadBuffer          * pReadBuffer = pStatusBar->m_pReadBuffer;
			EvolutionCore const * pCore  = pReadBuffer->LockReadBuffer( );
			EVO_GENERATION        evoGen = pCore->GetEvoGenerationNr( );
			pReadBuffer->ReleaseReadBuffer( );
			pStatusBar->DisplayCurrentGeneration( evoGen );
		}
        break;

    case WM_COMMAND:
		(void)SendMessage( GetParent( hwnd ), WM_COMMAND, LOWORD(wParam), 0 );
        return FALSE;

    case WM_HSCROLL:
		{
			HWND const hwndTrackBar = (HWND)lParam;
			INT  const iCtrlId      = GetDlgCtrlID( hwndTrackBar );
			LONG const lLogicalPos  = pStatusBar->GetTrackBarPos( iCtrlId );
			LONG const lValue       = ( iCtrlId == IDM_ZOOM_TRACKBAR ) ? lLogicalPos : ( SPEED_TRACKBAR_MAX - lLogicalPos );

	        (void)SendMessage( GetParent( hwnd ), WM_COMMAND, iCtrlId, trackBar2Value( lValue )  );
		}
        return TRUE;

    default: 
        break;
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void StatusBar::SetRunMode( BOOL const bMode )
{
	SetDlgText( IDM_RUN_STOP, bMode ? SZ_STOP_MODE : SZ_RUN_MODE );
}

HWND WINAPI StatusBar::createControl
(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD   dwStyle,
    HMENU   hMenu
)
{
    PIXEL_X const pixWidth = PIXEL_X(PIXEL(static_cast<int>( wcslen( lpWindowName ) ) * 9));
    HWND    const hwnd     = CreateWindow
    (
        lpClassName,                     // class name 
        lpWindowName,                    // title (caption) 
        WS_CHILD | WS_VISIBLE | dwStyle, // style 
        m_pixPosX.GetBaseValue(),          // x position
		m_pixBorderY.GetBaseValue(),       // y position 
        pixWidth.GetBaseValue(),           // width
		m_pixClientHeight.GetBaseValue(),  // height
        GetWindowHandle( ),              // parent window 
        hMenu,                           // control identifier 
        GetModuleHandle( nullptr ),      // instance 
        nullptr                             // no WM_CREATE parameter 
    );
    m_pixPosX += pixWidth;
    return hwnd;
}

HWND WINAPI StatusBar::createStaticControl( LPCTSTR lpWindowName )
{
    return createControl( WC_STATIC, lpWindowName, 0, nullptr );
}

HWND WINAPI StatusBar::createButton( LPCTSTR const lpWindowName, HMENU const hMenu, DWORD const dwStyle )
{ 
    return createControl( WC_BUTTON, lpWindowName, dwStyle, hMenu );
}

HWND WINAPI StatusBar::createTrackBar( HMENU hMenu )
{ 
    return createControl( TRACKBAR_CLASS, L"   Trackbar Control   ", WS_TABSTOP | WS_BORDER | TBS_NOTICKS, hMenu );
};

void WINAPI StatusBar::createModeControl( )
{ 
    (void)createButton( SZ_SIMU_MODE, (HMENU)IDM_TOGGLE_SIMU_MODE, BS_PUSHBUTTON );  
} 

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
} 

void WINAPI StatusBar::createSimulationControl( )
{ 
    if ( Config::UseHistorySystem( ) )
        createButton  ( L"Backwards ", (HMENU)IDM_BACKWARDS, BS_PUSHBUTTON );

    createButton  ( L"SingleStep", (HMENU)IDM_GENERATION, BS_PUSHBUTTON ); 
    createButton  ( SZ_RUN_MODE,   (HMENU)IDM_RUN_STOP,   BS_PUSHBUTTON ); 
    createTrackBar(                (HMENU)IDM_SIMULATION_SPEED ); 
    createButton  ( L" MaxSpeed ", (HMENU)IDM_MAX_SPEED,  BS_PUSHBUTTON ); 

    SetTrackBarRange( IDM_SIMULATION_SPEED, SPEED_TRACKBAR_MIN, SPEED_TRACKBAR_MAX );
} 

void WINAPI StatusBar::createEditorControl( )
{ 
	if ( Config::UseHistorySystem( ) )
	{
		createButton( L"   Undo   ", (HMENU)IDM_EDIT_UNDO, BS_PUSHBUTTON ); 
		createButton( L"   Redo   ", (HMENU)IDM_EDIT_REDO, BS_PUSHBUTTON ); 
	}
} 

void StatusBar::Start
( 
	HWND const   hwndParent,
	ReadBuffer * pReadBuffer
)
{
	m_pReadBuffer = pReadBuffer;
	HWND hwndStatus = CreateWindow
    (
        STATUSCLASSNAME, 
        nullptr, 
        WS_CHILD | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, STATUS_BAR_HEIGHT.GetBaseValue(),
        hwndParent,
        nullptr, 
        GetModuleHandle( nullptr ), 
        nullptr
    ); 

    SetWindowHandle( hwndStatus );

    static std::array< PIXEL_X, static_cast<int>( tPart::Stop ) + 1> statwidths = 
    { 
        100_PIXEL_X, // Generation 
		200_PIXEL_X, // Mode (Edit/Simu)
        400_PIXEL_X, // Size
        670_PIXEL_X, // Edit/Simu controls
        600_PIXEL_X, // ScriptLine
         -1_PIXEL_X  // Stop
    };

    PIXEL_X pixPartWidth = statwidths[0];
    for ( int i = 1; i < static_cast<int>( tPart::Stop ); ++i )
    {
        statwidths[i] += pixPartWidth;
        pixPartWidth = statwidths[i];
    }
    
    (void)SetWindowSubclass( hwndStatus, OwnerDrawStatusBar, 0, (DWORD_PTR)this ) ;

    m_pixBorderX      = PIXEL_X(PIXEL(GetSystemMetrics( SM_CXSIZEFRAME )));
    m_pixBorderY      = PIXEL_Y(PIXEL(GetSystemMetrics( SM_CYSIZEFRAME )));
    m_pixClientHeight = GetHeight( ) - m_pixBorderY;

    m_pixPosX = statwidths[ static_cast<int>( tPart::Mode ) - 1 ] + m_pixBorderX + 10_PIXEL_X;
    createModeControl( );

    m_pixPosX = statwidths[ static_cast<int>( tPart::Size ) - 1 ] + m_pixBorderX + 10_PIXEL_X;
    createSizeControl( );

    m_pixPosX = statwidths[ static_cast<int>( tPart::SimuEdit ) - 1 ] + m_pixBorderX + 10_PIXEL_X;
    createSimulationControl( );

    m_pixPosX = statwidths[ static_cast<int>( tPart::SimuEdit ) - 1 ] + m_pixBorderX + 10_PIXEL_X;
	createEditorControl( );

    (void)SendMessage( SB_SETPARTS, sizeof( statwidths ) / sizeof( int ), (LPARAM)( &statwidths ) );

    SetSizeTrackBar ( DEFAULT_FIELD_SIZE );
	long lDefaultDelay = Config::GetConfigValue( Config::tId::generationDelay );
    SetSpeedTrackBar( lDefaultDelay );
	PostCommand2Application( IDM_SIMULATION_SPEED, lDefaultDelay );
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

void StatusBar::SetSimuMode( BOOL const bSimuMode )
{
    ShowWindow( GetDlgItem( IDM_BACKWARDS        ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_GENERATION       ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_RUN_STOP         ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_SIMULATION_SPEED ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_MAX_SPEED        ), bSimuMode );

	if ( Config::UseHistorySystem( ) )
	{
		ShowWindow( GetDlgItem( IDM_EDIT_UNDO ), ! bSimuMode );
		ShowWindow( GetDlgItem( IDM_EDIT_REDO ), ! bSimuMode );
	}

	SetDlgText( IDM_TOGGLE_SIMU_MODE, bSimuMode ? SZ_EDIT_MODE : SZ_SIMU_MODE );
}

PIXEL_Y StatusBar::GetHeight( ) const
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
