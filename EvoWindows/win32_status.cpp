// win32_status.cpp : Verwaltet die Status Bar
//

#include "stdafx.h"
#include <array>
#include "Resource.h"
#include "commctrl.h"
#include "config.h"
#include "pixelCoordinates.h"
#include "EvolutionCore.h"
#include "win32_performanceWindow.h"
#include "win32_status.h"

//lint -e1924    C-style casts

using namespace std;

static long trackBar2Value( LONG ); 
static LONG value2Trackbar( long );  

static DWORD const DEFAULT_DELAY =   50;
static DWORD const MAX_DELAY     = 2048;    // in msecs

static double const TRACKBAR_SCALING_FACTOR = 1000.0;

static LONG const SPEED_TRACKBAR_MIN = 0; 
static LONG const SPEED_TRACKBAR_MAX = value2Trackbar( MAX_DELAY ); 

static INT const STATUS_BAR_HEIGHT = 22;

//lint -esym( 715, uIdSubclass )    symbol not referenced

static LRESULT CALLBACK OwnerDrawStatusBar( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
    StatusBar * const pStatusBar = (StatusBar *)dwRefData;
    switch ( uMsg )
    {
    case WM_PAINT:
		pStatusBar->DisplayCurrentGeneration( pStatusBar->m_pCore->GetEvoGenerationNr( ) );
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

HWND WINAPI StatusBar::createControl
(
    LPCTSTR lpClassName,
    LPCTSTR lpWindowName,
    DWORD   dwStyle,
    HMENU   hMenu
)
{
    INT  const iWidth = static_cast<int>( wcslen( lpWindowName ) ) * 9;
    HWND const hwnd   = CreateWindow
    (
        lpClassName,                     // class name 
        lpWindowName,                    // title (caption) 
        WS_CHILD | WS_VISIBLE | dwStyle, // style 
        m_iPosX, m_iBorder,              // position 
        iWidth, m_iClientHeight,         // size 
        GetWindowHandle( ),              // parent window 
        hMenu,                           // control identifier 
        GetModuleHandle( nullptr ),      // instance 
        NULL                             // no WM_CREATE parameter 
    );
    m_iPosX += iWidth;
    return hwnd;
}

HWND WINAPI StatusBar::createStaticControl( LPCTSTR lpWindowName )
{
    return createControl( WC_STATIC, lpWindowName, 0, nullptr );
}

HWND WINAPI StatusBar::createButton( LPCTSTR lpWindowName, HMENU hMenu )
{ 
    return createControl( WC_BUTTON, lpWindowName, BS_DEFPUSHBUTTON, hMenu );
}

HWND WINAPI StatusBar::createTrackBar( HMENU hMenu )
{ 
    return createControl( TRACKBAR_CLASS, L"   Trackbar Control   ", WS_TABSTOP | WS_BORDER | TBS_NOTICKS, hMenu );
};

void WINAPI StatusBar::createModeControl( )
{ 
    (void)createButton( L"Switch to Simulation", (HMENU)IDM_TOGGLE_SIMU_MODE ); 
} 

void WINAPI StatusBar::createSizeControl( )
{ 
    createStaticControl( L"Size" );
    createButton       ( L" - ",     (HMENU)IDM_ZOOM_OUT      ); 
    createTrackBar     (             (HMENU)IDM_ZOOM_TRACKBAR ); 
    createButton       ( L" + ",     (HMENU)IDM_ZOOM_IN       ); 
    createButton       ( L"  Fit  ", (HMENU)IDM_FIT_ZOOM      ); 

    LONG const lMinPos = value2Trackbar( PixelCoordinates::MINIMUM_FIELD_SIZE );
    LONG const lMaxPos = value2Trackbar( PixelCoordinates::MAXIMUM_FIELD_SIZE );

    SetTrackBarRange( IDM_ZOOM_TRACKBAR, lMinPos, lMaxPos );  
} 

void WINAPI StatusBar::createSimulationControl( )
{ 
    if ( Config::UseHistorySystem( ) )
        createButton  ( L"Backwards ", (HMENU)IDM_BACKWARDS );

    createButton  ( L"   Stop   ", (HMENU)IDM_STOP ); 
    createButton  ( L"SingleStep", (HMENU)IDM_GENERATION ); 
    createButton  ( L"   Run    ", (HMENU)IDM_RUN ); 
    createTrackBar(                (HMENU)IDM_SIMULATION_SPEED ); 
    createButton  ( L" MaxSpeed ", (HMENU)IDM_MAX_SPEED ); 

    SetTrackBarRange( IDM_SIMULATION_SPEED, SPEED_TRACKBAR_MIN, SPEED_TRACKBAR_MAX );
} 

void WINAPI StatusBar::createEditorControl( )
{ 
	if ( Config::UseHistorySystem( ) )
	{
		createButton( L"   Undo   ", (HMENU)IDM_EDIT_UNDO ); 
		createButton( L"   Redo   ", (HMENU)IDM_EDIT_REDO ); 
	}
} 

StatusBar::StatusBar( )
{ }

void StatusBar::Start
( 
	HWND          const   hwndParent,
	EvolutionCore const * pCore
)
{
	m_pCore = pCore;
	m_hwndParent = hwndParent;
	ThreadStartupFunc( );
//	StartThread( TRUE, "StatusBar" );
}

void StatusBar::ThreadStartupFunc( )
{
//	Continue( );   // trigger mother thread to continue
	HWND hwndStatus = CreateWindow
    (
        STATUSCLASSNAME, 
        nullptr, 
        WS_CHILD | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, STATUS_BAR_HEIGHT,
        m_hwndParent,
        nullptr, 
        GetModuleHandle( nullptr ), 
        nullptr
    ); 

    SetWindowHandle( hwndStatus );
	Continue( );   // trigger mother thread to continue

    static std::array< int, static_cast<int>( tPart::Stop ) + 1> statwidths = 
    { 
        100, // Generation 
		200, // Mode (Edit/Simu)
        400, // Size
        670, // Edit/Simu controls
        600, // ScriptLine
         -1  // Stop
    };

    int iPartWidth = statwidths[0];
    for ( int i = 1; i < static_cast<int>( tPart::Stop ); ++i )
    {
        statwidths[i] += iPartWidth;
        iPartWidth = statwidths[i];
    }
    
    (void)SetWindowSubclass( hwndStatus, OwnerDrawStatusBar, 0, (DWORD_PTR)this ) ;

    m_iBorder       = GetSystemMetrics( SM_CXSIZEFRAME );
    m_iClientHeight = GetHeight( ) - m_iBorder;

    m_iPosX = statwidths[ static_cast<int>( tPart::Mode ) - 1 ] + m_iBorder + 10;
    createModeControl( );

    m_iPosX = statwidths[ static_cast<int>( tPart::Size ) - 1 ] + m_iBorder + 10;
    createSizeControl( );

    m_iPosX = statwidths[ static_cast<int>( tPart::SimuEdit ) - 1 ] + m_iBorder + 10;
    createSimulationControl( );

    m_iPosX = statwidths[ static_cast<int>( tPart::SimuEdit ) - 1 ] + m_iBorder + 10;
	createEditorControl( );

    ( void )SendMessage( SB_SETPARTS, sizeof( statwidths ) / sizeof( int ), (LPARAM)( &statwidths ) );

    SetSizeTrackBar ( PixelCoordinates::DEFAULT_FIELD_SIZE );
    SetSpeedTrackBar( DEFAULT_DELAY );
	PostCommand2Application( IDM_SIMULATION_SPEED, DEFAULT_DELAY );
}

void StatusBar::SetSizeTrackBar( short const sFieldSize  ) const 
{ 
    SetTrackBarPos( IDM_ZOOM_TRACKBAR, value2Trackbar( sFieldSize ) ); 
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
    ShowWindow( GetDlgItem( IDM_STOP             ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_GENERATION       ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_RUN              ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_SIMULATION_SPEED ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_MAX_SPEED        ), bSimuMode );

	if ( Config::UseHistorySystem( ) )
	{
		ShowWindow( GetDlgItem( IDM_EDIT_UNDO ), ! bSimuMode );
		ShowWindow( GetDlgItem( IDM_EDIT_REDO ), ! bSimuMode );
	}

	wchar_t * szButtonText = bSimuMode ? L"Switch to EDITOR" : L"Switch to SIMULATION";

	(void)::SendMessage
	( 
		GetDlgItem( IDM_TOGGLE_SIMU_MODE  ), 
		WM_SETTEXT,
		0, 
		(LPARAM)( szButtonText )
	);
}

int StatusBar::GetHeight( ) const
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

void StatusBar::DisplayStatusLine( wstring const & wstrLine )
{
    (void)SendNotifyMessage( SB_SETTEXT, static_cast<int>( tPart::ScriptLine ), (LPARAM)( wstrLine.c_str( ) ) );
}

void StatusBar::DisplayScriptLine( wstring const & wszPath, int iLineNr, wstring const & wstrLine )
{
    m_wstrScriptLine = wszPath + L"(" + to_wstring( iLineNr ) + L"): " + wstrLine;
    DisplayStatusLine( m_wstrScriptLine );
}

void StatusBar::DisplayCurrentGeneration( EVO_GENERATION const gen )
{
    m_wstrGeneration = L"EvoGen " + to_wstring( gen );
    (void)SendNotifyMessage( SB_SETTEXT, static_cast<int>( tPart::Generation ), (LPARAM)( m_wstrGeneration.c_str() ) );
}
