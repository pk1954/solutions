// win32_status.cpp : Verwaltet die Status Bar
//

#include "stdafx.h"
#include <array>
#include "Resource.h"
#include "commctrl.h"
#include "config.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "pixelCoordinates.h"
#include "win32_evoController.h"
#include "win32_worker_thread.h"
#include "win32_util.h"
#include "win32_status.h"

//lint -e1924    C-style casts

using namespace std;

static double trackBar2Value( USHORT ); 
static USHORT value2Trackbar( long );  

static DWORD const DEFAULT_DELAY =   50;
static DWORD const MAX_DELAY     = 2048;    // in msecs

static double const TRACKBAR_SCALING_FACTOR = 1000.0;

static USHORT const SPEED_TRACKBAR_MIN = 0; 
static USHORT const SPEED_TRACKBAR_MAX = value2Trackbar( MAX_DELAY ); 

static INT const STATUS_BAR_HEIGHT =  22;

//lint -esym( 715, uIdSubclass )    symbol not referenced

static LRESULT CALLBACK OwnerDrawStatusBar( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData )
{
    StatusBar * const pStatusBar = (StatusBar *)dwRefData;

    switch ( uMsg )
    {

    case WM_COMMAND:
        (void)SendMessage( GetParent( hWnd ), WM_COMMAND, LOWORD(wParam), 0 );
        return TRUE;

    case WM_HSCROLL:
		{
			HWND   hwndTrackBar = (HWND)lParam;
			USHORT usLogicalPos = static_cast<USHORT>(::SendMessage( hwndTrackBar, TBM_GETPOS, 0, 0 ));
			switch ( GetDlgCtrlID( hwndTrackBar ) )
			{
			case IDM_ZOOM_TRACKBAR:
				pStatusBar->setMainGridWndZoom( usLogicalPos );
				break;

			case IDM_SPEED_TRACKBAR:
				pStatusBar->setGenerationDelay( usLogicalPos );
				break;

			default:
				assert( false );
			}
		}
        return TRUE;

    default: 
        break;
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

void StatusBar::setGenerationDelay( USHORT const usLogicalPos ) const
{
	DWORD const dwDelay = static_cast<DWORD>( trackBar2Value( SPEED_TRACKBAR_MAX - usLogicalPos ) );
	m_pEvoController->SetGenerationDelay( dwDelay );
}

void StatusBar::setMainGridWndZoom( USHORT const usLogicalPos ) const
{
	short const sFieldSize = static_cast<short>( trackBar2Value( usLogicalPos ) );
	m_pEvoController->SetZoom( sFieldSize );
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
    return createControl( TRACKBAR_CLASS, L"   Trackbar Control   ", 0, hMenu );
};

void WINAPI StatusBar::createModeControl( )
{ 
    (void)createButton( L"Switch to Simulation", (HMENU)IDM_TOGGLE_EDIT_SIMU_MODE ); 
} 

void WINAPI StatusBar::createSizeControl( short const sMin, short const sMax )
{ 
    createStaticControl( L"Size" );
    createButton       ( L" - ",     (HMENU)IDM_ZOOM_OUT      ); 
    createTrackBar     (             (HMENU)IDM_ZOOM_TRACKBAR ); 
    createButton       ( L" + ",     (HMENU)IDM_ZOOM_IN       ); 
    createButton       ( L"  Fit  ", (HMENU)IDM_FIT_ZOOM      ); 

    USHORT const usMinPos = value2Trackbar( sMin );
    USHORT const usMaxPos = value2Trackbar( sMax );

    SetTrackBarRange( IDM_ZOOM_TRACKBAR, usMinPos, usMaxPos );  
//  (void)SendDlgItemMessage( IDM_ZOOM_TRACKBAR, TBM_SETPAGESIZE,    0, (LPARAM)4 );                    // new page size    
} 

void WINAPI StatusBar::createSimulationControl( )
{ 
    if ( Config::UseHistorySystem( ) )
        createButton  ( L"Backwards ", (HMENU)IDM_BACKWARDS );

    createButton  ( L"   Stop   ", (HMENU)IDM_STOP ); 
    createButton  ( L"SingleStep", (HMENU)IDM_GENERATION ); 
    createButton  ( L"   Run    ", (HMENU)IDM_RUN ); 
    createTrackBar(                (HMENU)IDM_SPEED_TRACKBAR ); 
    createButton  ( L" MaxSpeed ", (HMENU)IDM_MAX_SPEED ); 

    SetTrackBarRange( IDM_SPEED_TRACKBAR, SPEED_TRACKBAR_MIN, SPEED_TRACKBAR_MAX );
//  (void)SendDlgItemMessage( IDM_SPEED_TRACKBAR, TBM_SETPAGESIZE,    0, (LPARAM)4 );                    // new page size    
} 

void WINAPI StatusBar::createEditorControl( )
{ 
	if ( Config::UseHistorySystem( ) )
	{
		createButton( L"   Undo   ", (HMENU)IDM_EDIT_UNDO ); 
		createButton( L"   Redo   ", (HMENU)IDM_EDIT_REDO ); 
	}
} 

//lint +e438
//lint +e529

StatusBar::StatusBar( ) : 
    m_pEvoController( nullptr ),
    m_pModelWork( nullptr )
{ }

void StatusBar::Start
( 
    HWND                 const hWndParent, 
    EvoController      * const pEvoController, 
    EvolutionModelData * const pModel
)
{
    HWND hWndStatus = CreateWindow
    (
        STATUSCLASSNAME, 
        nullptr, 
        WS_CHILD | WS_VISIBLE, // | SBARS_SIZEGRIP, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, STATUS_BAR_HEIGHT,
        hWndParent,
        nullptr, 
        GetModuleHandle( nullptr ), 
        nullptr
    ); 

    m_pEvoController = pEvoController;
    m_pModelWork = pModel;

    SetWindowHandle( hWndStatus );

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
    
    (void)SetWindowSubclass( hWndStatus, OwnerDrawStatusBar, 0, (DWORD_PTR)this ) ;

    m_iBorder       = GetSystemMetrics( SM_CXSIZEFRAME );
    m_iClientHeight = GetHeight( ) - m_iBorder;

    m_iPosX = statwidths[ static_cast<int>( tPart::Mode ) - 1 ] + m_iBorder + 10;
    createModeControl ( );

    m_iPosX = statwidths[ static_cast<int>( tPart::Size ) - 1 ] + m_iBorder + 10;
    createSizeControl( PixelCoordinates::MINIMUM_FIELD_SIZE, PixelCoordinates::MAXIMUM_FIELD_SIZE );

    m_iPosX = statwidths[ static_cast<int>( tPart::SimuEdit ) - 1 ] + m_iBorder + 10;
    createSimulationControl( );

    m_iPosX = statwidths[ static_cast<int>( tPart::SimuEdit ) - 1 ] + m_iBorder + 10;
	createEditorControl( );

    ( void )SendMessage( SB_SETPARTS, sizeof( statwidths ) / sizeof( int ), (LPARAM)( &statwidths ) );

    SetSizeTrackBar ( PixelCoordinates::DEFAULT_FIELD_SIZE );
    SetSpeedTrackBar( DEFAULT_DELAY );
}

void StatusBar::SetSizeTrackBar( short const sFieldSize  ) const 
{ 
    USHORT const usPos = value2Trackbar( sFieldSize );
    SetTrackBarPos( IDM_ZOOM_TRACKBAR, usPos ); 
}

void StatusBar::SetSpeedTrackBar( DWORD const dwDelay ) const 
{ 
    USHORT const usPos = ( dwDelay == 0 )
                         ? 0
                         : value2Trackbar( dwDelay );
    SetTrackBarPos( IDM_SPEED_TRACKBAR, SPEED_TRACKBAR_MAX - usPos );                
}

StatusBar::~StatusBar( )
{
    m_pEvoController = nullptr;
}

static double trackBar2Value( USHORT usX ) // f(x) = 2 power (x/1000)
{
    double const dX = static_cast<double>( usX ) / TRACKBAR_SCALING_FACTOR;
    return pow( 2.0, dX );
}

static USHORT value2Trackbar( long lX )  // f(x) = 1000 * log2(x)
{
    static double const dFactor = TRACKBAR_SCALING_FACTOR / log( 2 );

    assert( lX > 0 );
	double const dX = static_cast<double>( lX );
    double const dY = log( dX ) * dFactor;
    return static_cast<USHORT>( dY );
}

void StatusBar::SetSimuMode( BOOL const bSimuMode )
{
    ShowWindow( GetDlgItem( IDM_BACKWARDS      ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_STOP           ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_GENERATION     ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_RUN            ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_SPEED_TRACKBAR ), bSimuMode );
    ShowWindow( GetDlgItem( IDM_MAX_SPEED      ), bSimuMode );

	if ( Config::UseHistorySystem( ) )
	{
		ShowWindow( GetDlgItem( IDM_EDIT_UNDO ), ! bSimuMode );
		ShowWindow( GetDlgItem( IDM_EDIT_REDO ), ! bSimuMode );
	}

	wchar_t * szButtonText = bSimuMode ? L"Switch to EDITOR" : L"Switch to SIMULATION";

	(void)::SendMessage
	( 
		GetDlgItem( IDM_TOGGLE_EDIT_SIMU_MODE  ), 
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

void StatusBar::DisplayCurrentGeneration( )
{
    assert( m_pModelWork != nullptr );
    m_wstrGeneration = L"EvoGen " + to_wstring( m_pModelWork->GetEvoGenerationNr( ) );
    (void)SendNotifyMessage( SB_SETTEXT, static_cast<int>( tPart::Generation ), (LPARAM)( m_wstrGeneration.c_str() ) );
}

INT_PTR StatusBar::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    return static_cast<INT_PTR>( FALSE );
}

