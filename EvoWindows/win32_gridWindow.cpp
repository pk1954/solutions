// win32_gridWindow.cpp
//

#include "stdafx.h"
#include "Windowsx.h"
#include "Resource.h"
#include "config.h"
#include "BoolOp.h"
#include "PixelCore.h"
#include "pixelCoordinates.h"
#include "EvolutionCore.h"
#include "win32_util.h"
#include "win32_status.h"
#include "win32_draw.h"
#include "win32_focusPoint.h"
#include "win32_performanceWindow.h"
#include "win32_worker_thread.h"
#include "win32_aboutBox.h"
#include "win32_gridWindow.h"
#include "win32_displayOptions.h"

//lint -esym( 613, GridWindow::m_pPixelCoordinates )   nullptr will not be used

GridWindow::GridWindow( ) :
    BaseWindow( ),
    m_pWorkThread( nullptr ),
    m_pPixelCoordinates( nullptr ),
    m_pGWObserved( nullptr ),
    m_pEditorWindow( nullptr ),
    m_pPerformanceWindow( nullptr ),
    m_pDrawFrame( nullptr ),
    m_ptLast( PixelPoint( LONG_MIN, LONG_MIN ) ),
    m_bMoveAllowed( TRUE ),
    m_pFocusPoint( nullptr ),
	m_pPixelCore( nullptr )
{ }

void GridWindow::Start
( 
    HWND                 const hWndParent,
    WorkThread         * const pWorkThread,
    EditorWindow       * const pEditorWindow,
    FocusPoint         * const pFocusPoint,
    DspOptWindow       * const pDspOptWindow,
    PerformanceWindow  * const pPerformanceWindow,
    StatusBar          * const pStatusBar,
    EvolutionCore      *       pCore,
    EvolutionModelData *       pModel,
    DWORD                const dwStyle,
    SHORT                const sFieldSize
)
{
	int  const iNrOfNeighbors = Config::GetConfigValue( Config::tId::nrOfNeighbors );
    HWND const hWnd = StartBaseWindow
    ( 
        hWndParent,
        CS_OWNDC | CS_DBLCLKS,
        L"ClassGridWindow",
        dwStyle
    );
    assert( sFieldSize > 0 );
    m_pWorkThread        = pWorkThread;
    m_pPerformanceWindow = pPerformanceWindow;
    m_pEditorWindow      = pEditorWindow;
    m_pFocusPoint        = pFocusPoint;
	m_pStatusBar         = pStatusBar;
	m_pCore              = pCore;
	m_pModelWork         = pModel;

	BOOL bHexagonMode    = iNrOfNeighbors == 6;
    m_pPixelCoordinates  = new PixelCoordinates( sFieldSize, bHexagonMode );
	m_pPixelCore         = new PixelCore( m_pCore, m_pModelWork, m_pPixelCoordinates );
    m_pDrawFrame         = new DrawFrame( hWnd, pCore, pModel, m_pPixelCoordinates, pDspOptWindow );
	m_pDrawFrame->SetStripMode
	( 
		bHexagonMode     // in hexagon mode do not use strip mode (looks ugly)
		? tBoolOp::opFalse 
		: Config::GetConfigValueBoolOp( Config::tId::stripMode ) );
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

	m_pModelWork    = nullptr;
	m_pCore         = nullptr;
    m_pGWObserved   = nullptr;
    m_pEditorWindow = nullptr;
    m_pWorkThread   = nullptr;
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

void GridWindow::contextMenu( LPARAM lParam )
{
    HWND  const hwnd       = GetWindowHandle( );
    HMENU const hPopupMenu = CreatePopupMenu();
    UINT  const STD_FLAGS  = MF_BYPOSITION | MF_STRING;
	POINT pntPos{ GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };
    (void)MapWindowPoints( hwnd, nullptr, &pntPos, 1 );

    if ( m_pModelWork->SelectionIsEmpty() )
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
    (void)SetForegroundWindow( hwnd );

    UINT const uiID = (UINT)TrackPopupMenu( hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, pntPos.x, pntPos.y, 0, hwnd, nullptr ); 	// Result is send as WM_COMMAND to this window
    PostMessage( WM_COMMAND, uiID, lParam );
    (void)DestroyMenu( hPopupMenu );
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
        m_pWorkThread->PostRefresh( );
    }
    else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
    {
        if ( m_pEditorWindow->IsInEditMode() )
        {
            m_pWorkThread->PostDoEdit( m_pFocusPoint->GetGridPoint( ) );
        }
        else if ( m_ptLast.x != LONG_MIN )  // last cursor pos stored in m_ptLast
        {
           moveGrid( ptCrsr - m_ptLast );
        }
        m_ptLast = ptCrsr;
        m_pWorkThread->PostRefresh( );
    }
    else
    {
        m_ptLast.x = LONG_MIN;    // make m_ptLast invalid
        // m_pWorkThread->PostRefresh( );  // +++++++++++++ EXPERIMENTAL +++++++++++++++++
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
        (void)m_pDrawFrame->DoPaint( kgr );
        (void)EndPaint( &ps );
    }

    if ( m_bMoveAllowed && m_pCore->IsPoiDefined( ) )
	{
		if ( ! m_pPixelCore->CenterPoi( GetClRectCenter( ) ) )
		   Invalidate( FALSE );    // repeat if POI is not in center 
	}

    m_pPerformanceWindow->DisplayStop( );
}

void GridWindow::resize()
{
	if ( m_pStatusBar != nullptr )
		m_pStatusBar->SetSizeTrackBar( m_pPixelCoordinates->GetFieldSize() );
	m_pDrawFrame->Resize( );
}

void GridWindow::zoom( BOOL const bZoomIn )
{
    setZoom( m_pPixelCoordinates->GetNewFieldSize( bZoomIn ) );
}

void GridWindow::setZoom( SHORT const fieldSize )
{
    m_pPixelCore->SetFieldSize( fieldSize, GetClRectCenter( ) );
	resize();
}

void GridWindow::fit( )
{
    m_pPixelCore->FitToRect( GetClRectSize( ) );
	m_pModelWork->ResetSelection( );
	resize();
    m_pWorkThread->PostRefresh( );
}

void GridWindow::mouseWheelAction( int iDelta )
{
	BOOL const bDirection = ( iDelta > 0 );
	short      sNewFieldSize;

	iDelta = abs( iDelta );
			
	while ( --iDelta >= 0 )
	{
		sNewFieldSize = m_pPixelCoordinates->GetNewFieldSize( bDirection );
	}

	m_pPixelCoordinates->SetFieldSize( sNewFieldSize, GetClRectCenter( ) );
	m_pDrawFrame->Resize( );
    m_pWorkThread->PostRefresh( );
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
            case IDM_ZOOM_OUT:
                zoom( FALSE );
                break;

            case IDM_ZOOM_IN:
                zoom( TRUE );
                break;

            case IDM_FIT_ZOOM:
				fit();
                break;

            case IDM_SET_ZOOM:
				setZoom( static_cast<SHORT>(lParam) );
				break;

            case IDD_TOGGLE_STRIP_MODE:
                m_pDrawFrame->SetStripMode( tBoolOp::opToggle );
                break;
                 
            case IDD_TOGGLE_CLUT_MODE:
                m_pDrawFrame->SetIndDimmMode( tBoolOp::opToggle );
                break;

            case IDM_SET_POI:
				m_pPixelCore->SetPOI( GetCrsrPosFromLparam( lParam ) );
				Post2Application( WM_COMMAND, IDM_STOP, 0 );
                break;

			case IDM_ESCAPE:
				m_pModelWork->ResetSelection( );
				m_pWorkThread->PostRefresh( );
				break;

            case IDM_GOTO_ORIGIN:  // commands using cursor pos are handled here
            case IDM_GOTO_DEATH:
            {
                PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );
                GridPoint  const gpCrsr = m_pPixelCoordinates->Pixel2GridPos( ptCrsr );
				Post2Application( WM_COMMAND, wParam, Pack2LParam( gpCrsr ) );
			}
            break;

            default:
                Post2Application( message, wParam, lParam ); // not handled here, delegate to application
            }
        }

        m_pWorkThread->PostRefresh( );
        return 1;  //  TODO clarify return code

    case WM_MOUSEWHEEL:
		mouseWheelAction(GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA);
        return 0;

    case WM_LBUTTONDOWN:
        if ( m_pModelWork->SelectionIsEmpty() )
        {
            SetCapture( );
            if ( inObservedClientRect( lParam ) || m_bMoveAllowed )
                onMouseMove( lParam, wParam );
        }
		else
		{
			m_pModelWork->ResetSelection( );
			m_pWorkThread->PostRefresh( );
		}
        SetFocus( );
        return 1;

    case WM_LBUTTONUP:
        (void)ReleaseCapture( );
        return 0;

    case WM_MOUSEMOVE:
        onMouseMove( lParam, wParam );
        return 0;

    case WM_RBUTTONDOWN:
        SetCapture(  );
        SetFocus( );
        return 0;

    case WM_RBUTTONUP:
        (void)ReleaseCapture( );
        contextMenu( lParam );
        return 0;

    case WM_PAINT:
        doPaint( );
        return 0;

    default:
        break;
    }

    return DefWindowProc( message, wParam, lParam );
}

void GridWindow::Observe( GridWindow * const hgw )
{
    m_pGWObserved  = hgw;
    m_bMoveAllowed = FALSE; 
}

void GridWindow::Size( )
{
    PixelPoint const ptSize = m_pPixelCoordinates->Grid2PixelSize( GridPoint::GRID_SIZE );
    DWORD      const dwStyle = (DWORD)GetWindowLongPtr( GetWindowHandle( ), GWL_STYLE );	
	RECT             rect{ 0, 0, ptSize.x, ptSize.y };
    (void)AdjustWindowRect( &rect, dwStyle, FALSE );	
    Move( 0, 0, rect.right - rect.left, rect.bottom - rect.top, FALSE );
}
