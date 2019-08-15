// win32_baseWindow.cpp
//
// win32_utilities

#include "stdafx.h"
#include "win32_util_resource.h"
#include "win32_refreshRateDialog.h"
#include "win32_baseWindow.h"

using namespace std::chrono;

BaseWindow::BaseWindow( ) : 
	RootWindow( ),
	m_visibilityMode( tOnOffAuto::on ),
	m_visibilityCriterion( nullptr )
{}

BaseWindow::~BaseWindow( )    
{}

HWND BaseWindow::StartBaseWindow
( 
    HWND                  const   hwndParent,
    UINT                  const   uiClassStyle,
    LPCTSTR               const   szClass,
    DWORD                 const   dwWindowStyle,
	PixelRect             const * pRect,
	std::function<bool()> const   visibilityCriterion
)
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );
    WNDCLASSEX      wcex;

    assert( szClass != nullptr );

	m_visibilityCriterion = visibilityCriterion;

    wcex.cbSize = sizeof( WNDCLASSEX );

    wcex.style		   = uiClassStyle;
    wcex.lpfnWndProc   = BaseWndProc;
    wcex.cbClsExtra	   = 0;
    wcex.cbWndExtra	   = sizeof( void * );	
    wcex.hInstance	   = hInstance;
    wcex.hIcon		   = nullptr; 
    wcex.hCursor	   = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = szClass;
    wcex.hIconSm	   = nullptr; 

    ATOM const atom = RegisterClassEx( &wcex );

    if ( atom == 0 )
    {
        DWORD const dwErr = GetLastError( );
        assert( dwErr == ERROR_CLASS_ALREADY_EXISTS );
    }

    HWND hwnd = CreateWindow
    (
        szClass,
        nullptr,
        dwWindowStyle,
		pRect ? pRect->GetLeft  ().GetValue() : CW_USEDEFAULT,
		pRect ? pRect->GetTop   ().GetValue() : CW_USEDEFAULT,
		pRect ? pRect->GetWidth ().GetValue() : CW_USEDEFAULT,
		pRect ? pRect->GetHeight().GetValue() : CW_USEDEFAULT,
        hwndParent,
        nullptr,
        hInstance, 
        this
    );
    assert( hwnd != nullptr );
    SetWindowHandle( hwnd );
	m_visibilityMode = m_visibilityCriterion 
		               ? tOnOffAuto::automatic 
		               : dwWindowStyle & WS_VISIBLE 
						  ? tOnOffAuto::on 
		                  : tOnOffAuto::off;
    return hwnd;
}

void BaseWindow::AddWinMenu( HMENU const hMenuParent, std::wstring const strTitle ) const
{
	UINT  const STD_FLAGS = MF_BYPOSITION | MF_STRING;
	HMENU const hMenu = CreatePopupMenu();
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_WINDOW_AUTO, L"auto" );
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_WINDOW_ON,   L"on"   );
	(void)AppendMenu( hMenu, STD_FLAGS, IDM_WINDOW_OFF,  L"off"  );
	(void)AppendMenu( hMenuParent, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hMenu, strTitle.c_str() );
}

void BaseWindow::AdjustWinMenu( HMENU const hMenu ) const
{
	EnableMenuItem( hMenu, IDM_WINDOW_AUTO, ((m_visibilityMode == tOnOffAuto::automatic ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_WINDOW_ON,   ((m_visibilityMode == tOnOffAuto::on        ) ? MF_GRAYED : MF_ENABLED) );
	EnableMenuItem( hMenu, IDM_WINDOW_OFF,  ((m_visibilityMode == tOnOffAuto::off       ) ? MF_GRAYED : MF_ENABLED) );
}

void BaseWindow::contextMenu( LPARAM lParam )
{
	HMENU const hPopupMenu{ CreatePopupMenu() };
	POINT       pntPos{ GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };

	AddContextMenuEntries( hPopupMenu, pntPos );

	if ( m_visibilityCriterion )
	{
		AddWinMenu( hPopupMenu, L"Show window" );
		AdjustWinMenu( hPopupMenu );
	}

	if ( GetRefreshRate( ) > 0ms )
	{
		(void)AppendMenu( hPopupMenu, MF_STRING, IDM_REFRESH_RATE_DIALOG, L"Refresh Rate" );
	}

	(void)ClientToScreen( GetWindowHandle(), & pntPos );
    (void)SetForegroundWindow( GetWindowHandle( ) );

    UINT const uiID = (UINT)TrackPopupMenu
	                  ( 
						  hPopupMenu, 
						  TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, 
						  pntPos.x, pntPos.y, 
						  0, 
						  GetWindowHandle( ), 
						  nullptr 
					  );         	// Result is send as WM_COMMAND to this window

	if ( uiID != 0 )
	    SendMessage( WM_COMMAND, uiID, lParam );

	(void)DestroyMenu( hPopupMenu );
}

static LRESULT CALLBACK BaseWndProc
( 
    HWND   const hwnd,
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
	BaseWindow * pBaseWin = reinterpret_cast<BaseWindow *>(GetWindowLongPtr( hwnd, GWLP_USERDATA ));

    switch (message)
    {

	case WM_NCCREATE:    // retrieve Window instance from window creation data and associate    
        (void)SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)( (LPCREATESTRUCT)lParam )->lpCreateParams );
        return TRUE;

	case WM_ERASEBKGND:
        return TRUE;			// Do not erase background

    case WM_RBUTTONDOWN:
        pBaseWin->SetCapture( );
        pBaseWin->SetFocus( );
        return FALSE;

    case WM_RBUTTONUP:
        (void)ReleaseCapture( );
        pBaseWin->contextMenu( lParam );
        return FALSE;

    case WM_COMMAND:
        {
            UINT uiCmdId = LOWORD( wParam );
            switch ( uiCmdId )
            {

			case IDM_WINDOW_ON:
				pBaseWin->Show( TRUE );
				return FALSE;

			case IDM_WINDOW_OFF:
				pBaseWin->Show( FALSE );
				return FALSE;

			case IDM_WINDOW_AUTO:
				pBaseWin->AdjustVisibility( tOnOffAuto::automatic, pBaseWin->m_visibilityCriterion );
				return FALSE;

			case IDM_REFRESH_RATE_DIALOG:
			{
				milliseconds msRefreshRateOld = pBaseWin->GetRefreshRate( );
				milliseconds msRefreshRateNew = RefreshRateDialog::Show( hwnd, msRefreshRateOld );
				pBaseWin->SetRefreshRate( msRefreshRateNew );
				return FALSE;
			}

            default:
 				break;
           }
        }
		break;

	default:
 			break;
	}

	if ( RootWinIsReady( pBaseWin ) )
		return pBaseWin->UserProc( message, wParam, lParam );         // normal case
	else
		return DefWindowProc( hwnd, message, wParam, lParam );
}
