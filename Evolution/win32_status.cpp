// win32_status.cpp : Verwaltet die Status Bar
//

#include "stdafx.h"
#include <array>
#include "Resource.h"
#include "commctrl.h"
#include "config.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "EvoController.h"
#include "win32_worker_thread.h"
#include "win32_frameBuffer.h"
#include "win32_status.h"

//lint -e1924    C-style casts

using namespace std;

static double trackBar2Value( USHORT ); 
static USHORT value2Trackbar( double );  

static DWORD const DEFAULT_DELAY =   50;
static DWORD const MAX_DELAY     = 2048;    // in msecs

static HMENU const ID_TRACKBAR = (HMENU)1;

static double const TRACKBAR_SCALING_FACTOR = 1000.0;

static USHORT const SPEED_TRACKBAR_MIN = 0; 
static USHORT const SPEED_TRACKBAR_MAX = value2Trackbar( static_cast<double>(MAX_DELAY) ); 

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
        switch( LOWORD( wParam ) )
        {
            case TB_THUMBTRACK:
            case TB_THUMBPOSITION:
            {
                USHORT const usLogicalPos = HIWORD( wParam );
                if ( (HWND)lParam == pStatusBar->m_hWndSize )
                {       
                    short const sFieldSize = pStatusBar->trackBarPos2FieldSize( usLogicalPos );
                    (void)SendMessage( GetParent( hWnd ), WM_COMMAND, IDM_SET_ZOOM, sFieldSize );
                }
                else if ( (HWND)lParam == pStatusBar->m_hWndSpeed )
                {
                    DWORD const dwDelay = pStatusBar->trackBarPos2SpeedDelay( usLogicalPos );
                    pStatusBar->m_pEvoController->SetGenerationDelay( dwDelay );
                }
                else
                {
                    assert( false );
                }
            }
                break;

            default:   // TODO: Check if needed 
//                iPos = SendMessage( pStatusBar->m_hWndSize, TBM_GETPOS, 0, 0 );
                break;
        }
        return TRUE;

    default: 
        break;
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
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

HWND WINAPI StatusBar::createTrackBar( )
{ 
    return createControl( TRACKBAR_CLASS, L"   Trackbar Control   ", 0, ID_TRACKBAR );
};

//lint -e438   symbol not used
//lint -e529   symbol not used

HWND WINAPI StatusBar::createSizeControl( )
{ 
    HWND const hwndTitle       = createStaticControl( L"Size" );
    HWND const hwndMinusButton = createButton       ( L" - ",     (HMENU)IDM_ZOOM_OUT ); 
    HWND const hwndTrackBar    = createTrackBar     (  ); 
    HWND const hwndPlusButton  = createButton       ( L" + ",     (HMENU)IDM_ZOOM_IN  ); 
    HWND const hwndFitButton   = createButton       ( L"  Fit  ", (HMENU)IDM_FIT_ZOOM ); 

    USHORT const usMinPos = fieldSize2TrackBarPos( FrameBuffer::MINIMUM_FIELD_SIZE );
    USHORT const usMaxPos = fieldSize2TrackBarPos( FrameBuffer::MAXIMUM_FIELD_SIZE );

    (void)::SendMessage( hwndTrackBar, TBM_SETRANGE,    TRUE, (LPARAM)MAKELONG( usMinPos, usMaxPos ) );  
    (void)::SendMessage( hwndTrackBar, TBM_SETPAGESIZE,    0, (LPARAM)4 );                    // new page size    
    
    return hwndTrackBar; 
} 

HWND WINAPI StatusBar::createSpeedControl( )
{ 
    if ( Config::UseHistorySystem( ) )
                                createButton  ( L"Backwards ", (HMENU)IDM_BACKWARDS );
    HWND const hwndStopButton = createButton  ( L"   Stop   ", (HMENU)IDM_STOP ); 
    HWND const hwndSnglButton = createButton  ( L"SingleStep", (HMENU)IDM_GENERATION ); 
    HWND const hwndRunButton  = createButton  ( L"   Run    ", (HMENU)IDM_RUN ); 
    HWND const hwndTrackBar   = createTrackBar( ); 
    HWND const hwndMaxButton  = createButton  ( L" MaxSpeed ", (HMENU)IDM_MAX_SPEED ); 

    (void)::SendMessage( hwndTrackBar, TBM_SETRANGE,    TRUE, (LPARAM)MAKELONG( SPEED_TRACKBAR_MIN, SPEED_TRACKBAR_MAX ) );
    (void)::SendMessage( hwndTrackBar, TBM_SETPAGESIZE,    0, (LPARAM)4 );                    // new page size    
    
    return hwndTrackBar; 
} 

//lint +e438
//lint +e529

StatusBar::StatusBar( ) : 
    m_pEvoController( nullptr ),
    m_pModelWork( nullptr ),
    m_gp( ),
    m_hWndSize( nullptr ),
    m_hWndSpeed( nullptr )
{}

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
        nullptr,    // IDC_MAIN_STATUS, 
        GetModuleHandle( nullptr ), 
        nullptr
    ); 

    m_pEvoController = pEvoController;
    m_pModelWork = pModel;

    SetWindowHandle( hWndStatus );

    static std::array< int, static_cast<int>( tPart::Stop ) + 1> statwidths = 
    { 
        100, // Generation 
        400, // Size
        670, // Speed
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

    m_iPosX     = statwidths[ static_cast<int>( tPart::Size ) - 1 ] + m_iBorder + 10;
    m_hWndSize  = createSizeControl ( );

    m_iPosX     = statwidths[ static_cast<int>( tPart::Speed ) - 1 ] + m_iBorder + 10;
    m_hWndSpeed = createSpeedControl( );

    ( void )SendMessage( SB_SETPARTS, sizeof( statwidths ) / sizeof( int ), (LPARAM)( &statwidths ) );

    SetSizeTrackBar ( FrameBuffer::DEFAULT_FIELD_SIZE );
    SetSpeedTrackBar( DEFAULT_DELAY );
}

void StatusBar::SetSizeTrackBar( short const sFieldSize  ) const 
{ 
    USHORT const usPos = fieldSize2TrackBarPos( sFieldSize );
    setTrackBarPos( m_hWndSize,  usPos  ); 
}

void StatusBar::SetSpeedTrackBar( DWORD const dwDelay ) const 
{ 
    USHORT const usPos = ( dwDelay == 0 )
                         ? SPEED_TRACKBAR_MAX
                         : speedDelay2TrackBarPos( dwDelay );
    setTrackBarPos( m_hWndSpeed, usPos );                
}

StatusBar::~StatusBar( )
{
    m_pEvoController = nullptr;
    m_hWndSize       = nullptr;
    m_hWndSpeed      = nullptr;
}

static double trackBar2Value( USHORT usX ) // f(x) = 2 power (x/1000)
{
    double const dX = static_cast<double>( usX ) / TRACKBAR_SCALING_FACTOR;
    return pow( 2.0, dX );
}

short StatusBar::trackBarPos2FieldSize( USHORT usTrackBarPos ) const 
{
    return static_cast<short>( trackBar2Value( usTrackBarPos ) );
}

DWORD StatusBar::trackBarPos2SpeedDelay( USHORT usTrackBarPos ) const 
{
    return static_cast<DWORD>( trackBar2Value( SPEED_TRACKBAR_MAX - usTrackBarPos ) );
}

static USHORT value2Trackbar( double dX )  // f(x) = 1000 * log2(x)
{
    static double const dFactor = TRACKBAR_SCALING_FACTOR / log( 2 );
    assert( dX > 0 );
    double const dY = log( dX ) * dFactor;
    return static_cast<USHORT>( dY );
}

USHORT StatusBar::fieldSize2TrackBarPos( short sFieldSize ) const 
{
    return value2Trackbar( static_cast<double>(sFieldSize) );
}

USHORT StatusBar::speedDelay2TrackBarPos( DWORD dwDelay ) const 
{
    return SPEED_TRACKBAR_MAX - value2Trackbar( static_cast<double>(dwDelay) );
}

void StatusBar::setTrackBarPos( HWND const hwndTrackBar, USHORT const usPos ) const
{
    (void)::SendMessage
    (   
        hwndTrackBar, TBM_SETPOS, 
        static_cast<WPARAM>( TRUE ),                   // redraw flag 
        static_cast<LPARAM>( usPos )
    ); 
}

int StatusBar::GetHeight( ) const
{
    return STATUS_BAR_HEIGHT;
}
    
void StatusBar::Resize( ) const 
{
    (void)SendMessage( WM_SIZE, 0, 0 );
}

void StatusBar::ClearStatusLine( )
{
    DisplayStatusLine( L"" );
}

void StatusBar::DisplayStatusLine( wstring const & wstrLine )
{
    (void)SendMessage( SB_SETTEXT, static_cast<int>( tPart::ScriptLine ), (LPARAM)( wstrLine.c_str( ) ) );
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
    (void)SendMessage( SB_SETTEXT, static_cast<int>( tPart::Generation ), (LPARAM)( m_wstrGeneration.c_str() ) );
}

INT_PTR StatusBar::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    return static_cast<INT_PTR>( FALSE );
}

