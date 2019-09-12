// win32_gridWindow.cpp
//

#include "stdafx.h"
#include "Resource.h"
#include "config.h"
#include "BoolOp.h"
#include "PixelTypes.h"
#include "GridDimensions.h"
#include "gridSelection.h"
#include "gridPOI.h"
#include "gridPoint24.h"
#include "EvolutionCore.h"
#include "win32_util.h"
#include "win32_clut.h"
#include "EvoReadBuffer.h"
#include "win32_colorManager.h"
#include "win32_focusPoint.h"
#include "win32_crsrWindow.h"
#include "win32_actionTimer.h"
#include "win32_graphicsInterface.h"
#include "win32_EvoWorkThreadInterface.h"
#include "win32_packGridPoint.h"
#include "win32_displayOptions.h"
#include "win32_gridWindow.h"

#include "Commdlg.h"

HWND                     GridWindow::m_hwndApp                 = nullptr;
EvoReadBuffer          * GridWindow::m_pReadBuffer             = nullptr;
EvoWorkThreadInterface * GridWindow::m_pEvoWorkThreadInterface = nullptr;
ActionTimer            * GridWindow::m_pActionTimer            = nullptr;
DspOptWindow           * GridWindow::m_pDspOptWindow           = nullptr;
FocusPoint             * GridWindow::m_pFocusPoint             = nullptr;
ColorManager           * GridWindow::m_pColorManager           = nullptr;
HCURSOR                  GridWindow::m_hCrsrArrow              = nullptr;
HCURSOR                  GridWindow::m_hCrsrMove               = nullptr;
					      
void GridWindow::InitClass
(        
	EvoReadBuffer          * const pReadBuffer, 
    EvoWorkThreadInterface * const pWorkThreadInterface,
    FocusPoint             * const pFocusPoint,
    DspOptWindow           * const pDspOptWindow,
	ActionTimer            * const pActionTimer,
	ColorManager           * const pColorManager
)
{
	m_pReadBuffer             = pReadBuffer;
	m_pEvoWorkThreadInterface = pWorkThreadInterface;
    m_pActionTimer            = pActionTimer;
	m_pDspOptWindow           = pDspOptWindow;
    m_pFocusPoint             = pFocusPoint;
	m_pColorManager           = pColorManager;
	m_hCrsrArrow              = LoadCursor( NULL, IDC_ARROW );
	m_hCrsrMove               = LoadCursor( NULL, IDC_SIZEALL );
}

GridWindow::GridWindow( ) :
    BaseWindow( ),
	m_pGraphics( nullptr ),
	m_hPopupMenu( nullptr ),
    m_pGridWindowObserved( nullptr ),
    m_pObserverInterface( nullptr ),
    m_ptLast( PP_NULL ),
    m_bMoveAllowed( TRUE )
{ }

void GridWindow::Start
( 
	HWND                  const hwndApp, 
	GraphicsInterface   * const pGraphics,
    DWORD                 const dwStyle,
    PIXEL                 const pixFieldSize,
	std::function<bool()> const visibilityCriterion
)
{
    assert( pixFieldSize > 0_PIXEL );
    
	BOOL bHexagonMode = GridDimensions::GetNrOfNeigbors() == 6;

	m_PixelCoordinates.Start( pixFieldSize, bHexagonMode );
	m_pGraphics = pGraphics;

	HWND hwnd = StartBaseWindow
    ( 
        hwndApp,
        CS_OWNDC | CS_DBLCLKS,
        L"ClassGridWindow",
        dwStyle,
		nullptr,
		visibilityCriterion
    );

	m_DrawFrame.Start
	( 
		hwnd, 
		m_pReadBuffer, 
		& m_PixelCoordinates, 
		m_pGraphics,
		m_pDspOptWindow, 
		m_pColorManager
	);

	m_DrawFrame.SetStripMode
	( 
		bHexagonMode     // in hexagon mode do not use strip mode (looks ugly)
		? tBoolOp::opFalse 
		: Config::GetConfigValueBoolOp( Config::tId::stripMode ) 
	);

	m_pReadBuffer->RegisterObserver( this );
}

void GridWindow::Stop( )
{
	DestroyWindow( );
	m_pGraphics = nullptr;
}

GridWindow::~GridWindow( )
{
	m_pReadBuffer          = nullptr;
	m_pEvoWorkThreadInterface = nullptr;
	m_pGridWindowObserved  = nullptr;
}

void GridWindow::AddContextMenuEntries( HMENU const hPopupMenu, POINT const pntPos )
{
	UINT const STD_FLAGS = MF_BYPOSITION | MF_STRING;

	if ( m_pEvoWorkThreadInterface->IsRunning() )
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDM_STOP, L"Stop" );
	else
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDM_RUN, L"Run" );

	(void)AppendMenu( hPopupMenu, STD_FLAGS, IDM_SET_POI, L"POI" );

	(void)AppendMenu( hPopupMenu, STD_FLAGS, IDM_SCRIPT_DIALOG, L"Script" );
	(void)AppendMenu( hPopupMenu, STD_FLAGS, IDM_RESET,         L"Reset" );

	if ( GridSelection::SelectionIsEmpty() )
	{
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDM_FIT_ZOOM, L"Fit Grid Area" );
	}
	else
	{
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDM_FIT_ZOOM, L"Fit Selection" );
		(void)AppendMenu( hPopupMenu, STD_FLAGS, IDM_ESCAPE,   L"Escape" );
	}

	EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer( );

	if ( m_pFocusPoint->IsInGrid( ) && m_pFocusPoint->IsAlive( * pCore ) )
	{
		{
			HMENU const hColorMenu = CreatePopupMenu();
			(void)AppendMenu( hColorMenu, STD_FLAGS, IDM_CHOOSE_STRATEGY_COLOR,  L"Choose strategy color" );
			(void)AppendMenu( hColorMenu, STD_FLAGS, IDM_CHOOSE_HIGHLIGHT_COLOR, L"Choose highlight color" );
			(void)AppendMenu( hColorMenu, STD_FLAGS, IDM_CHOOSE_SELECTION_COLOR, L"Choose selection color" );
			(void)AppendMenu( hPopupMenu, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hColorMenu, L"Choose color" );
		}

		if ( Config::UseHistorySystem( ) )
		{
			HMENU const hTimeTravelMenu = CreatePopupMenu();
			(void)AppendMenu( hTimeTravelMenu, STD_FLAGS, IDM_GOTO_DEATH,  L"Goto death of this individual" );
			(void)AppendMenu( hTimeTravelMenu, STD_FLAGS, IDM_GOTO_ORIGIN, L"Goto origin of this individual" );
			(void)AppendMenu( hPopupMenu, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hTimeTravelMenu, L"Time travel" );
		}
	}

	m_DrawFrame.AddContextMenuEntries( pCore, hPopupMenu, pntPos );

	m_pReadBuffer->ReleaseReadBuffer( );
}

void GridWindow::onMouseMove( LPARAM const lParam, WPARAM const wParam )
{
    PixelPoint    const   ptCrsr = GetCrsrPosFromLparam( lParam );  // relative to client area
	EvolutionCore const * pCore  = m_pReadBuffer->LockReadBuffer( );

    m_pFocusPoint->SetFocusPoint( m_PixelCoordinates.Pixel2GridPos( ptCrsr ) );

	if ( m_DrawFrame.SetHighlightPos( pCore, ptCrsr ) )
		PostCommand2Application( IDM_REFRESH, 1 );   // refresh immediatelly

    if ( wParam & MK_RBUTTON )                // Right mouse button: selection
    {
        if ( m_ptLast.IsNotNull() )  // last cursor pos stored in m_ptLast
        {
            PixelPoint ptOther = GridPOI::IsPoiDefined( ) 
				                 ? m_PixelCoordinates.Grid2PixelPosCenter( GridPOI::GetPoi() ) * 2 - ptCrsr 
				                 : m_ptLast;
			GridSelection::SetSelection( m_PixelCoordinates.Pixel2GridRect( PixelRect( ptOther, ptCrsr ) ) );
        }
        else                                // first time here after RBUTTON pressed
        {
            m_ptLast = ptCrsr;         // store current cursor pos
        }
		PostCommand2Application( IDM_REFRESH, 0 );
    }
    else if ( wParam & MK_LBUTTON )  	// Left mouse button: move or edit action
    {
        if ( (pCore->GetBrushMode() != tBrushMode::move) && (ptCrsr != m_ptLast) )
        {
            m_pEvoWorkThreadInterface->PostDoEdit( m_pFocusPoint->GetGridPoint( ) );
        }
        else if ( m_ptLast.IsNotNull() )  // last cursor pos stored in m_ptLast
        {
            moveGrid( ptCrsr - m_ptLast );
        }
        m_ptLast = ptCrsr;
		PostCommand2Application( IDM_REFRESH, 0 );
    }
    else
    {
        m_ptLast = PP_NULL;    // make m_ptLast invalid
        // no refresh! It would cause repaint for every mouse move.
    }

	m_pReadBuffer->ReleaseReadBuffer( );
}

BOOL GridWindow::inObservedClientRect( LPARAM const lParam )
{
    if ( m_pGridWindowObserved == nullptr )
        return FALSE;

	PixelCoordinates const * pixCoordObserved = & m_pGridWindowObserved->m_PixelCoordinates;
    PixelPoint       const   ptCrsr           = GetCrsrPosFromLparam( lParam );
    PixelPoint       const   ptCrsrCheck      = Pixel2PixelPos( ptCrsr, & m_PixelCoordinates, pixCoordObserved );
	HWND             const   hwndObserved     = m_pGridWindowObserved->GetWindowHandle( );

    return Util::IsInClientRect( hwndObserved, ptCrsrCheck );  // Is cursor position in observed client rect?
}

void GridWindow::moveGrid( PixelPoint const ptDiff )
{
    if ( m_pGridWindowObserved != nullptr )     // I observe someone
    {
		PixelCoordinates * const pixCoordObserved = & m_pGridWindowObserved->m_PixelCoordinates;
        PixelPoint         const ptDiffObserved   = Pixel2PixelSize( ptDiff, & m_PixelCoordinates, pixCoordObserved );
        pixCoordObserved->MoveGrid( -ptDiffObserved );   // move the observed in opposite direction 
    }

    if ( m_bMoveAllowed )
    {
        m_PixelCoordinates.MoveGrid( ptDiff );
    }
}

void GridWindow::doPaint( )
{
	m_pActionTimer->TimerStart( );

    if ( IsWindowVisible() )
	{
        PAINTSTRUCT ps;
        HDC const hdc = BeginPaint( &ps );
		if ( m_pGraphics->StartFrame( GetWindowHandle(), hdc ) )
		{
			EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer( );
			(void)m_DrawFrame.DoPaint( pCore );
			m_pReadBuffer->ReleaseReadBuffer( );

			COLORREF const color = m_pColorManager->GetColor( tColorObject::selection );

			if ( GridSelection::SelectionIsNotEmpty() )
				m_pGraphics->RenderTranspRect( m_PixelCoordinates.Grid2PixelRect( GridSelection::GetSelection() ), 64, color );  

			if ((m_pGridWindowObserved != nullptr) && CrsrInClientRect())   // if I observe someone and cursor is in client area, show its position
			{
				PixelCoordinates * const pixCoordObserved = & m_pGridWindowObserved->m_PixelCoordinates;
				PixelRect          const pixRectObserved  = Util::GetClPixelRect( m_pGridWindowObserved->GetWindowHandle( ) );
				PixelRect          const pixRectTarget    = Pixel2PixelRect( pixRectObserved, pixCoordObserved, & m_PixelCoordinates );
				m_pGraphics->RenderTranspRect( pixRectTarget, 128, color );  
			}
			m_pGraphics->EndFrame( GetWindowHandle() );
		}
        (void)EndPaint( &ps );
    }

    if ( m_bMoveAllowed && GridPOI::IsPoiDefined( ) )
	{
		bool bCentered = m_PixelCoordinates.CenterPoi( GetClRectCenter( ), GridPOI::GetPoi( ) );
		if ( ! bCentered )
		   Invalidate( FALSE );    // repeat if POI is not in center 
	}

	m_pActionTimer->TimerStop( );
}

void GridWindow::mouseWheelAction( WPARAM const wParam )
{
	int        iDelta     = GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA;
	BOOL const bDirection = ( iDelta > 0 );
	PIXEL      pixNewFieldSize;

	iDelta = abs( iDelta );
			
	while ( --iDelta >= 0 )
	{
		pixNewFieldSize = m_PixelCoordinates.ComputeNewFieldSize( bDirection );
	}

	PostCommand2Application( IDM_SET_ZOOM, pixNewFieldSize.GetValue() );
}

bool GridWindow::IsFullGridVisible() const
{
	return IsInClientRect( m_PixelCoordinates.Grid2PixelRect( GridDimensions::GridRectFull() ) );
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
			case IDM_CHOOSE_STRATEGY_COLOR:
			{
				PixelPoint    const   ptCrsr = GetCrsrPosFromLparam( lParam );
				GridPoint     const   gpCrsr = m_PixelCoordinates.Pixel2GridPos( ptCrsr );
				EvolutionCore const * pCore  = m_pReadBuffer->LockReadBuffer( );
				Strategy::Id  const   strat  = pCore->GetStrategyId( gpCrsr );
				m_pReadBuffer->ReleaseReadBuffer( );
				m_DrawFrame.CallStrategyColorDialog( GetWindowHandle(), strat );
			}
			break;

			case IDM_CHOOSE_SELECTION_COLOR:
			{
				LOGFONT lf; 
				CHOOSEFONT cf;
				cf.lStructSize = sizeof(CHOOSEFONT); 
				cf.hwndOwner = (HWND)nullptr; 
				cf.hDC = (HDC)nullptr; 
				cf.lpLogFont = &lf; 
				cf.iPointSize = 0; 
				cf.Flags = CF_SCREENFONTS | CF_FIXEDPITCHONLY; 
				cf.rgbColors = RGB(0,0,0); 
				cf.lCustData = 0L; 
				cf.lpfnHook = (LPCFHOOKPROC)nullptr; 
				cf.lpTemplateName = (LPCTSTR)nullptr; 
				cf.hInstance = (HINSTANCE) nullptr; 
				cf.lpszStyle = (LPTSTR)nullptr; 
				cf.nFontType = SCREEN_FONTTYPE; 
				cf.nSizeMin = 0; 
				cf.nSizeMax = 0; 
				BOOL bRes = ChooseFont( & cf );

				m_DrawFrame.CallSelectionColorDialog( GetWindowHandle() );
			}
			break;

			case IDM_CHOOSE_HIGHLIGHT_COLOR:
				m_DrawFrame.CallHighlightColorDialog( GetWindowHandle() );
			break;

			case IDM_SET_POI:
            case IDM_GOTO_ORIGIN:  
            case IDM_GOTO_DEATH:      // commands using cursor pos are handled here
            {
                PixelPoint const ptCrsr = GetCrsrPosFromLparam( lParam );
                GridPoint  const gpCrsr = m_PixelCoordinates.Pixel2GridPos( ptCrsr );
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
		mouseWheelAction( wParam );
        return FALSE;

    case WM_LBUTTONDOWN:
        if ( GridSelection::SelectionIsEmpty() )
        {
            SetCapture( );
            if ( inObservedClientRect( lParam ) || m_bMoveAllowed )
                onMouseMove( lParam, wParam );
        }
		else
		{
			GridSelection::ResetSelection( );
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

	case WM_SETCURSOR:
	{
		EvolutionCore const * pCore   = m_pReadBuffer->LockReadBuffer( );
		tBrushMode    const   mode    = pCore->GetBrushMode();
		BOOL          const   keyDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		HCURSOR       const   hCrsr   = ( (mode == tBrushMode::move) && keyDown ) ? m_hCrsrMove : m_hCrsrArrow;
		m_pReadBuffer->ReleaseReadBuffer( );
		SetCursor( hCrsr );
	}
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
	Move
	(
		Util::CalcWindowRect
		( 
			m_PixelCoordinates.Grid2PixelRect( GridDimensions::GridRectFull() ),
			(DWORD)GetWindowLongPtr( GetWindowHandle( ), GWL_STYLE ) 
		), 
		FALSE 
	);
}

void GridWindow::newFieldSize
( 
	PIXEL     const pixfieldSize, 
	GridPoint const gpCenter 
)
{
	if ( m_PixelCoordinates.SetGridFieldSize( pixfieldSize ) )
	{
		EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer( );
		m_PixelCoordinates.CenterGrid( gpCenter, GetClRectSize( ) ); // center grid around gpCenter
		m_DrawFrame.ResizeDrawFrame( pCore );                        // trigger DrawFrame to adapt font size etc.
		PixelPoint const ppCrsr = GetRelativeCrsrPosition( );
		if ( IsInClientRect( ppCrsr ) )
			m_DrawFrame.SetHighlightPos( pCore, ppCrsr );  
		m_pReadBuffer->ReleaseReadBuffer( );
		Trigger( );   // cause repaint
	}
	else
	{
		MessageBeep( MB_ICONWARNING );
	}
}

void GridWindow::SetFieldSize( PIXEL const pixFieldSize )
{
	GridPoint const gpCenter = GridPOI::IsPoiDefined( ) 
			       		      ? GridPOI::GetPoi() 
							  : m_PixelCoordinates.Pixel2GridPos( GetClRectCenter( ) );
	newFieldSize( pixFieldSize, gpCenter );
}

void GridWindow::Fit2Rect( )
{
	GridRect const gridRect = GridSelection::GetSelection();
	newFieldSize
	( 
		m_PixelCoordinates.CalcMaximumFieldSize( gridRect.GetSize(), GetClRectSize( ) ), 
		gridRect.GetCenter() 
	);
	GridSelection::ResetSelection( );
}

void GridWindow::Zoom( bool const bZoomIn )	
{ 
	SetFieldSize( m_PixelCoordinates.ComputeNewFieldSize( bZoomIn ) ); 
}

void GridWindow::ToggleStripMode( ) 
{ 
	m_DrawFrame.SetStripMode( tBoolOp::opToggle );
	Trigger( );   // cause repaint
}

void GridWindow::ToggleClutMode( ) 
{ 
	m_pColorManager->ToggleClutMode();
	Trigger( );   // cause repaint
}

void GridWindow::Escape( ) 
{ 
	GridSelection::ResetSelection( ); 
}

PIXEL GridWindow::GetFieldSize( ) const
{ 
	return m_PixelCoordinates.GetFieldSize( ); 
};
