// win32_baseWindow.cpp
//

#include "stdafx.h"
#include "win32_baseWindow.h"

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
    DWORD   const dwWindowStyle
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
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
        hwndParent,
        nullptr,
        hInstance, 
        this
    );
    assert( hwnd != nullptr );
    SetWindowHandle( hwnd );
    return hwnd;
}

static LRESULT CALLBACK BaseWndProc
( 
    HWND   const hwnd,
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
    if ( message == WM_NCCREATE )    // retrieve Window instance from window creation data and associate    
    {
        (void)SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)( (LPCREATESTRUCT)lParam )->lpCreateParams );
        return TRUE;
    }
    else if ( message == WM_ERASEBKGND )
    {
        return TRUE;			// Do not erase background
    }
    else
    {
        BaseWindow * pBaseWin = (BaseWindow *)GetWindowLongPtr( hwnd, GWLP_USERDATA );

        return ( RootWinIsReady( pBaseWin ) )
            ? pBaseWin->UserProc( message, wParam, lParam )         // normal case
            : DefWindowProc( hwnd, message, wParam, lParam );
    }
}
