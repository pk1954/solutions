// win32_gridWindow.cpp
//

#include "stdafx.h"
#include "Windowsx.h"
#include "Resource.h"
#include "config.h"
#include "win32_util.h"
#include "win32_status.h"
#include "win32_draw.h"
#include "win32_focusPoint.h"
#include "win32_frameBuffer.h"
#include "win32_performanceWindow.h"
#include "win32_worker_thread.h"
#include "win32_aboutBox.h"
#include "win32_gridWindow.h"
#include "win32_displayOptions.h"

//lint -esym( 613, GridWindow::m_pFrameBuffer )   nullptr will not be used

GridWindow::GridWindow( ) :
    BaseWindow( ),
    m_pWorkThread( nullptr ),
    m_pFrameBuffer( nullptr ),
    m_pGWObserved( nullptr ),
    m_pEditorWindow( nullptr ),
    m_pPerformanceWindow( nullptr ),
    m_pDrawFrame( nullptr ),
    m_pGridRectSel( nullptr ),
    m_ptLast( PixelPoint( LONG_MIN, LONG_MIN ) ),
    m_bMoveAllowed( TRUE ),
    m_pFocusPoint( nullptr )
{ }

void GridWindow::Start
( 
    HWND                const hWndParent,
    WorkThread        * const pWorkThread,
    GridRect          * const pSelection,
    EditorWindow      * const pEditorWindow,
    FocusPoint        * const pFocusPoint,
    DspOptWindow      * const pDspOptWindow,
    PerformanceWindow * const pPerformanceWindow,
    StatusBar         * const pStatusBar,
    EvolutionCore     *       pCore,
    ModelData         *       pModel,
    DWORD               const dwStyle,
    short               const sFieldSize
)
{
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
    m_pGridRectSel       = pSelection;
    m_pFocusPoint        = pFocusPoint;
    m_pFrameBuffer       = new FrameBuffer( hWnd, sFieldSize, pStatusBar, pCore, pModel );
    m_pDrawFrame         = new DrawFrame( hWnd, pCore, pModel, m_pFrameBuffer, pDspOptWindow, m_pGridRectSel );
    m_pDrawFrame->SetStripMode( tBoolOp::opTrue );
}

GridWindow::~GridWindow( )
{
    try
    {
        delete m_pFrameBuffer;
        delete m_pDrawFrame;
    }
    catch ( ... )
    {
        exit( 1 );
    };

    m_pGWObserved   = nullptr;
    m_pEditorWindow = nullptr;
    m_pGridRectSel  = nullptr;
    m_pWorkThread   = nullptr;
}

void GridWindow::setSelection( PixelPoint const & pt1, PixelPoint const & pt2 )
{
    PixelRect const rectSelection( pt1, pt2 );	   // Current selection
    GridRect        rect( m_pFrameBuffer->Pixel2GridRect( rectSelection ) );
    rect.ClipToGrid( );
    * m_pGridRectSel = rect;
}

void GridWindow::ResetSelection( )
{
    * m_pGridRectSel = GridRect::GRID_RECT_EMPTY;
    m_pWorkThread->PostRefresh( );
}

BOOL GridWindow::inObservedClientRect( LPARAM const lParam )
{
    if ( m_pGWObserved == nullptr )
        return FALSE;

    PixelPoint const ptCrsr       = GetCrsrPosFromLparam( lParam );
    PixelPoint const ptCrsrCheck  = m_pFrameBuffer->Pixel2PixelPos( ptCrsr, *(m_pGWObserved->m_pFrameBuffer) );
    PixelRect  const rectObserved = Util::GetClRect( m_pGWObserved->GetWindowHandle( ) );                     

    return PtInRect( &rectObserved, ptCrsrCheck );  // Is cursor position in observed client rect?
}

void GridWindow::contextMenu( LPARAM lParam )
{
    HWND  const hwnd       = GetWindowHandle( );
    HMENU const hPopupMenu = CreatePopupMenu();
    UINT  const STD_FLAGS  = MF_BYPOSITION | MF_STRING;
    PixelPoint  ptPos( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
    (void)MapWindowPoints( hwnd, nullptr, &ptPos, 1 );

    if ( m_pFocusPoint->IsInGrid( ) && m_pFocusPoint->IsAlive( ) )
    {
        UINT const uiHistoryFlags = Config::UseHistorySystem( ) ? STD_FLAGS : STD_FLAGS | MF_GRAYED;
        (void)InsertMenu( hPopupMenu, 0, uiHistoryFlags, IDM_GOTO_DEATH,  L"Goto Death" );
        (void)InsertMenu( hPopupMenu, 0, uiHistoryFlags, IDM_GOTO_ORIGIN, L"Goto Origin" );
    }
    if ( m_pGridRectSel->IsEmpty( ) )
    {
        (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_FIT_ZOOM, L"Fit Grid Area" );
    }
    else
    {
        (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_ESCAPE,   L"Escape" );
        (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_FIT_ZOOM, L"Fit Selection" );
    }
    (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_SCRIPT_DIALOG, L"Script" );
    (void)InsertMenu( hPopupMenu, 0, STD_FLAGS, IDM_SET_POI,       L"POI" );
    (void)SetForegroundWindow( hwnd );

    UINT const uiID = (UINT)TrackPopupMenu( hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, ptPos.x, ptPos.y, 0, hwnd, nullptr ); 	// Result is send as WM_COMMAND to this window
    PostMessage( WM_COMMAND, uiID, lParam );
    (void)DestroyMenu( hPopupMenu );
}

void GridWindow::moveGrid( PixelPoint const & ptDiff )
{
    if ( m_pGWObserved != nullptr )     // I observe someone
    {
        PixelPoint const ptDiffObserved = m_pFrameBuffer->Pixel2PixelSize( ptDiff, *( m_pGWObserved->m_pFrameBuffer) );
        m_pGWObserved->m_pFrameBuffer->MoveGrid( -ptDiffObserved );   // move the observed 
    }

    if ( m_bMoveAllowed )
    {
        m_pFrameBuffer->MoveGrid( ptDiff );
    }
}

void GridWindow::onMouseMove( LPARAM const lParam, WPARAM const wParam )
{
    PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );

    m_pFocusPoint->SetGridPoint( m_pFrameBuffer->Pixel2GridPos( ptCrsr ) );

    if ( wParam & MK_RBUTTON )                // Right mouse button: selection
    {
        if ( m_ptLast.x != LONG_MIN )  // last cursor pos stored in m_ptLast
        {
            PixelPoint ptOther = m_ptLast;
            if ( m_pFrameBuffer->IsPoiDefined() )
            {
                PixelPoint const pixPointPoi = m_pFrameBuffer->FindPoiCenter( );
                ptOther = pixPointPoi + pixPointPoi - ptCrsr;
            }
            setSelection( ptOther, ptCrsr ); 
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
        // no PostRefresh! It would cause repaint for every mouse move.
    }
}

void GridWindow::doPaint( )
{
    m_pPerformanceWindow->DisplayStart( );

    KGridRect const kgr =  // if I observe someone and cursor is in client area, show his position
        ( ( m_pGWObserved != nullptr ) && CrsrInClientRect( ) )
        ? m_pGWObserved->m_pFrameBuffer->Pixel2KGridRect( Util::GetClRect( m_pGWObserved->GetWindowHandle( ) ) )
        : KGridRect::KGRID_RECT_EMPTY;
    {
        PAINTSTRUCT ps;
        BeginPaint( &ps );
        (void)m_pDrawFrame->DoPaint( kgr );
        (void)EndPaint( &ps );
    }

    if ( m_bMoveAllowed && m_pFrameBuffer->IsPoiDefined( ) && !m_pFrameBuffer->CenterPoi( ) )
        Invalidate( FALSE );    // repeat if POI is not in center 

    m_pPerformanceWindow->DisplayStop( );
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
                (void)m_pFrameBuffer->Zoom( FALSE );
                break;

            case IDM_ZOOM_IN:
                (void)m_pFrameBuffer->Zoom( TRUE );
                break;

            case IDM_FIT_ZOOM:
                (void)m_pFrameBuffer->FitToRect( m_pGridRectSel->IsEmpty() ? GridRect::GRID_RECT_FULL : *m_pGridRectSel );
                break;

            case IDM_SET_ZOOM:
                (void)m_pFrameBuffer->SetFieldSize( static_cast<short>(lParam) );
                break;

            case IDD_TOGGLE_STRIP_MODE:
                m_pDrawFrame->SetStripMode( tBoolOp::opToggle );
                break;
                 
            case IDD_TOGGLE_CLUT_MODE:
                m_pDrawFrame->SetIndDimmMode( tBoolOp::opToggle );
                break;

            case IDM_SET_POI:
                m_pFrameBuffer->SetPoi( GetCrsrPosFromLparam( lParam ) );
                m_pWorkThread->PostHistoryAction( IDM_STOP );
                break;

            case IDM_GOTO_ORIGIN:  // commands using cursor pos are handled here
            case IDM_GOTO_DEATH:
            {
                PixelPoint const ptCrsr( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
                GridPoint  const gpCrsr = m_pFrameBuffer->Pixel2GridPos( ptCrsr );
                m_pWorkThread->PostHistoryAction( uiCmdId, gpCrsr );
            }
            break;

            default:
                ::PostMessage( GetAncestor( GetWindowHandle( ), GA_ROOTOWNER ), message, wParam, lParam );
            }
        }

        m_pWorkThread->PostRefresh( );
        return 1;  //  TODO clarify return code

    case WM_LBUTTONDOWN:
        if ( m_pGridRectSel->IsEmpty() )
        {
            SetCapture( );
            if ( inObservedClientRect( lParam ) || m_bMoveAllowed )
                onMouseMove( lParam, wParam );
        }
        else
            ResetSelection( );
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
    PixelPoint const ptSize = m_pFrameBuffer->Grid2PixelSize( GridPoint::GRID_SIZE );
    PixelRect        rect( 0, 0, ptSize.x, ptSize.y );
    DWORD      const dwStyle = (DWORD)GetWindowLongPtr( GetWindowHandle( ), GWL_STYLE );	
    (void)AdjustWindowRect( &rect, dwStyle, FALSE );	
    Move( 0, 0, rect.right - rect.left, rect.bottom - rect.top, FALSE );
}
