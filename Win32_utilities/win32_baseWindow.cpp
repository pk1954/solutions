// win32_baseWindow.cpp
//

#include "stdafx.h"
#include "win32_refreshRateDialog.h"
#include "win32_baseWindow.h"

#define IDM_REFRESH_RATE_DIALOG  10

//lint -e1924                C-style cast

BaseWindow::BaseWindow( )    
    : RootWindow( )
{}

BaseWindow::~BaseWindow( )    
{}

HWND BaseWindow::StartBaseWindow
( 
    HWND    const hwndParent,
    UINT    const uiClassStyle,
    LPCTSTR const szClass,
    DWORD   const dwWindowStyle,
	PIXEL_X	const xPos, 
	PIXEL_Y	const yPos, 
	PIXEL_X	const width, 
	PIXEL_Y	const height
)
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );
    WNDCLASSEX      wcex;

    assert( szClass != nullptr );
	m_szClass = szClass;

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
        xPos.GetBaseValue(), 
		yPos.GetBaseValue(), 
		width.GetBaseValue(), 
		height.GetBaseValue(), 
        hwndParent,
        nullptr,
        hInstance, 
        this
    );
    assert( hwnd != nullptr );
    SetWindowHandle( hwnd );
    return hwnd;
}

void BaseWindow::contextMenu( LPARAM lParam )
{
	HMENU const hPopupMenu{ CreatePopupMenu() };
	POINT       pntPos{ GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };

    (void)InsertMenu( hPopupMenu, 0, MF_STRING, IDM_REFRESH_RATE_DIALOG, L"Refresh Rate" );
    (void)ClientToScreen( GetWindowHandle(), & pntPos );
	AddContextMenuEntries( hPopupMenu, pntPos );  
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

	        case IDM_REFRESH_RATE_DIALOG:
			{
				DWORD dwRefreshRateOld = pBaseWin->GetRefreshRate( );
				DWORD dwRefreshRateNew = RefreshRateDialog::Show( hwnd, dwRefreshRateOld );
				pBaseWin->SetRefreshRate( dwRefreshRateNew );
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
	{
		BOOL bReady = RootWinIsReady( pBaseWin );
		if ( bReady )
			return pBaseWin->UserProc( message, wParam, lParam );         // normal case
		else
			return DefWindowProc( hwnd, message, wParam, lParam );
	}
}
