// win32_gridWindow.cpp
//

#include "stdafx.h"
#include "Resource.h"
#include "config.h"
#include "BoolOp.h"
#include "PixelCore.h"
#include "pixelCoordinates.h"
#include "EvolutionCore.h"
#include "win32_util.h"
#include "win32_draw.h"
#include "win32_focusPoint.h"
#include "win32_crsrWindow.h"
#include "win32_performanceWindow.h"
#include "win32_workThreadInterface.h"
#include "win32_packGridPoint.h"
#include "win32_displayOptions.h"
#include "win32_gridWindow.h"

//lint -esym( 613, GridWindow::m_pPixelCoordinates )   nullptr will not be used

GridWindow::GridWindow( ) :
    BaseWindow( ),
    m_pWorkThreadInterface( nullptr ),
    m_pPixelCoordinates( nullptr ),
    m_pGWObserved( nullptr ),
	m_pCore( nullptr ),
    m_pPerformanceWindow( nullptr ),
	m_pDspOptWindow( nullptr ),
    m_pFocusPoint( nullptr ),
    m_pObserverInterface( nullptr ),
	m_pPixelCore( nullptr ),
    m_pDrawFrame( nullptr ),
    m_ptLast( PixelPoint( LONG_MIN, LONG_MIN ) ),
    m_bMoveAllowed( TRUE )
{ }

void GridWindow::Start
( 
    HWND                  const hwndParent,
    WorkThreadInterface * const pWorkThreadInterface,
    FocusPoint          * const pFocusPoint,
    DspOptWindow        * const pDspOptWindow,
    PerformanceWindow   * const pPerformanceWindow,
    EvolutionCore       * const pCore,
    DWORD                 const dwStyle,
    SHORT                 const sFieldSize
)
{
    assert( sFieldSize > 0 );
	m_pWorkThreadInterface = pWorkThreadInterface;
    m_pPerformanceWindow   = pPerformanceWindow;
	m_pDspOptWindow        = pDspOptWindow;
    m_pFocusPoint          = pFocusPoint;
	m_pCore                = pCore;
    
	BOOL bHexagonMode   = (Config::GetConfigValue( Config::tId::nrOfNeighbors ) == 6);
    m_pPixelCoordinates = new PixelCoordinates( sFieldSize, bHexagonMode );
	m_pPixelCore        = new PixelCore( m_pCore, m_pPixelCoordinates );
    m_pDrawFrame        = new DrawFrame( m_pCore, m_pPixelCoordinates, m_pDspOptWindow );

	m_pDrawFrame->SetStripMode
	( 
		bHexagonMode     // in hexagon mode do not use strip mode (looks ugly)
		? tBoolOp::opFalse 
		: Config::GetConfigValueBoolOp( Config::tId::stripMode ) 
	);

	StartBaseWindow
    ( 
        hwndParent,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        CS_OWNDC | CS_DBLCLKS,
        L"ClassGridWindow",
        dwStyle
    );
}

GridWindow::~GridWindow( )
{
    try
    {
        delete m_pPixelCoordinates;
        delete m_pDrawFrame;
    }
    catch ( ... )
    {
        exit( 1 );
    };

	m_pCore                = nullptr;
    m_pGWObserved          = nullptr;
    m_pWorkThreadInterface = nullptr;
}

BOOL GridWindow::inObservedClientRect( LPARAM const lParam )
{
    if ( m_pGWObserved == nullptr )
        return FALSE;

	PixelCoordinates const pixCoordObserved = *(m_pGWObserved->m_pPixelCoordinates);
    PixelPoint       const ptCrsr           = GetCrsrPosFromLparam( lParam );
    PixelPoint       const ptCrsrCheck      = m_pPixelCoordinates->Pixel2PixelPos( ptCrsr, pixCoordObserved );
	HWND             const hWndObserved     = m_pGWObserved->GetWindowHandle( );

    return Util::PixelPointInClientRect( hWndObserved, ptCrsrCheck );  // Is cursor position in observed client rect?
}

void GridWindow::AddContextMenuEntries( HMENU hPopupMenu )
{
    UINT const STD_FLAGS = MF_BYPOSITION | MF_STRING;

    if ( m_pCore->SelectionIsEmpty() )
    {
        (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_FIT_ZOOM, L"Fit Grid Area" );
    }
    else
    {
        (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_ESCAPE,   L"Escape" );
        (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_FIT_ZOOM, L"Fit Selection" );
    }
    (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_SCRIPT_DIALOG, L"Script" );
    if ( m_pFocusPoint->IsInGrid( ) && m_pFocusPoint->IsAlive( ) )
    {
		UINT const uiHistoryFlags = Config::UseHistorySystem( ) ? STD_FLAGS : STD_FLAGS | MF_GRAYED;
		(void)InsertMenu( hPopupMenu, 0, uiHistoryFlags, IDM_GOTO_DEATH,  L"Goto Death" );
		(void)InsertMenu( hPopupMenu, 0, uiHistoryFlags, IDM_GOTO_ORIGIN, L"Goto Origin" );
		(void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_SET_POI, L"POI" );
    }
}

void GridWindow::moveGrid( PixelPoint const & ptDiff )
{
    if ( m_pGWObserved != nullptr )     // I observe someone
    {
        PixelPoint const ptDiffObserved = m_pPixelCoordinates->Pixel2PixelSize( ptDiff, *( m_pGWObserved->m_pPixelCoordinates) );
        m_pGWObserved->m_pPixelCoordinates->MoveGrid( -ptDiffObserved );   // move the observed 
    }

    if ( m_bMoveAllowed )
    {
        m_pPixelCoordinates->MoveGrid( ptDiff );
    }
}

void GridWindow::onMouseMove( LPARAM const lParam, WPARAM const wParam )
{
    PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );

    m_pFocusPoint->SetFocusPoint( m_pPixelCoordinates->Pixel2GridPos( ptCrsr ) );

    if ( wParam & MK_RBUTTON )                // Right mouse button: selection
    {
        if ( m_ptLast.x != LONG_MIN )  // last cursor pos stored in m_ptLast
        {
            PixelPoint ptOther = m_pCore->IsPoiDefined( ) 
				                 ? m_pPixelCore->GetPoiCenter() * 2 - ptCrsr 
				                 : m_ptLast;
            m_pPixelCore->SetSelection( ptOther, ptCrsr ); 
        }
        else                                // first time here after RBUTTON pressed
        {
            m_ptLast = ptCrsr;         // store current cursor pos
        }
		PostCommand2Application( IDM_REFRESH, 0 );
    }
    else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
    {
        if ( (m_pCore->GetBrushMode() != tBrushMode::move) && (ptCrsr != m_ptLast) )
        {
            m_pWorkThreadInterface->PostDoEdit( m_pFocusPoint->GetGridPoint( ) );
        }
        else if ( m_ptLast.x != LONG_MIN )  // last cursor pos stored in m_ptLast
        {
           moveGrid( ptCrsr - m_ptLast );
        }
        m_ptLast = ptCrsr;
		PostCommand2Application( IDM_REFRESH, 0 );
    }
    else
    {
        m_ptLast.x = LONG_MIN;    // make m_ptLast invalid
        // no PostRefresh! It would cause repaint for every mouse move.
    }
}

void GridWindow::doPaint( )
{
    m_pPerformanceWindow->DisplayStart( );

    KGridRect const kgr =  // if I observe someone and cursor is in client area, show his position
        ( ( m_pGWObserved != nullptr ) && CrsrInClientRect( ) )
        ? m_pGWObserved->m_pPixelCoordinates->Pixel2KGridRect( Util::GetClPixelRect( m_pGWObserved->GetWindowHandle( ) ) )
        : KGridRect::KGRID_RECT_EMPTY;

    {
        PAINTSTRUCT ps;
        BeginPaint( &ps );
        (void)m_pDrawFrame->DoPaint( GetWindowHandle(), kgr );
        (void)EndPaint( &ps );
    }

    if ( m_bMoveAllowed && m_pCore->IsPoiDefined( ) )
	{
		if ( ! m_pPixelCore->CenterPoi( GetClRectCenter( ) ) )
		   Invalidate( FALSE );    // repeat if POI is not in center 
	}

    m_pPerformanceWindow->DisplayStop( );
}

void GridWindow::mouseWheelAction( int iDelta )
{
	BOOL const bDirection = ( iDelta > 0 );
	short      sNewFieldSize;

	iDelta = abs( iDelta );
			
	while ( --iDelta >= 0 )
	{
		sNewFieldSize = m_pPixelCoordinates->ComputeNewFieldSize( bDirection );
	}

	PostCommand2Application( IDM_SET_ZOOM, sNewFieldSize );
}

LRESULT GridWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    switch (message)
    {

    case WM_COMMAND:
        {
            UINT uiCmdId = LOWORD( wParam );
            switch ( uiCmdId )
            {
            case IDM_SET_POI:
            case IDM_GOTO_ORIGIN:  
            case IDM_GOTO_DEATH:      // commands using cursor pos are handled here
            {
                PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );
                GridPoint  const gpCrsr = m_pPixelCoordinates->Pixel2GridPos( ptCrsr );
				PostCommand2Application( wParam, Pack2LParam( gpCrsr ) );
			}
            break;

            default:
                PostCommand2Application( wParam, lParam ); // not handled here, delegate to application
            }
        }

		PostCommand2Application( IDM_REFRESH, 0 );
        return FALSE;

    case WM_MOUSEWHEEL:
		mouseWheelAction(GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA);
        return FALSE;

    case WM_LBUTTONDOWN:
        if ( m_pCore->SelectionIsEmpty() )
        {
            SetCapture( );
            if ( inObservedClientRect( lParam ) || m_bMoveAllowed )
                onMouseMove( lParam, wParam );
        }
		else
		{
			m_pCore->ResetSelection( );
			PostCommand2Application( IDM_REFRESH, 0 );
		}
        SetFocus( );
        return TRUE;

    case WM_LBUTTONUP:
        (void)ReleaseCapture( );
        return FALSE;

    case WM_MOUSEMOVE:
        onMouseMove( lParam, wParam );
        return FALSE;

    case WM_PAINT:
        doPaint( );
        return FALSE;

    default:
        break;
    }

    return DefWindowProc( message, wParam, lParam );
}

void GridWindow::Size( )
{
    PixelPoint const ptSize = m_pPixelCoordinates->Grid2PixelSize( GridPoint::GRID_SIZE );
    DWORD      const dwStyle = (DWORD)GetWindowLongPtr( GetWindowHandle( ), GWL_STYLE );	
	RECT             rect{ 0, 0, ptSize.x, ptSize.y };
    (void)AdjustWindowRect( &rect, dwStyle, FALSE );	
    Move( 0, 0, rect.right - rect.left, rect.bottom - rect.top, FALSE );
}

void GridWindow::SetFieldSize( SHORT const fieldSize )
{
	m_pPixelCore->SetFieldSize( fieldSize, GetClRectCenter( ) );
	m_pDrawFrame->Resize( );
}

void GridWindow::Fit2Rect( )
{
	m_pPixelCore->FitToRect( GetClRectSize( ) ); 
	m_pDrawFrame->Resize( );
}

void GridWindow::Zoom( bool const bZoomIn )	
{ 
	SetFieldSize( m_pPixelCoordinates->ComputeNewFieldSize( bZoomIn ) ); 
}

void GridWindow::ToggleStripMode( ) 
{ 
	m_pDrawFrame->SetStripMode( tBoolOp::opToggle ); 
}

void GridWindow::ToggleClutMode( ) 
{ 
	m_pDrawFrame->SetIndDimmMode( tBoolOp::opToggle ); 
}

void GridWindow::Escape( ) 
{ 
	m_pCore->ResetSelection( ); 
}
