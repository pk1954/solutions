// win32_baseWindow.cpp
//
// win32_utilities

#include "stdafx.h"
#include "win32_baseWindow.h"

HWND BaseWindow::StartBaseWindow
( 
    HWND             const   hwndParent,
    UINT             const   uiClassStyle,
    LPCTSTR          const   szClass,
    DWORD            const   dwWindowStyle,
	PixelRect        const * pRect,
	function<bool()> const   visibilityCriterion
)
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );
    WNDCLASSEX      wcex;

    assert( szClass != nullptr );

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

	StartRootWindow( visibilityCriterion );

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
    switch (message)
    {
	case WM_NCCREATE:    // retrieve Window instance from window creation data and associate    
        (void)SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)( (LPCREATESTRUCT)lParam )->lpCreateParams );
        return true;

	case WM_ERASEBKGND:
		return true;			// Do not erase background

	default:
 		break;
	}
	{
		BaseWindow * pBaseWin = reinterpret_cast<BaseWindow *>(GetUserDataPtr( hwnd ));

		if ( ! RootWinIsReady( pBaseWin ) )
			return DefWindowProc( hwnd, message, wParam, lParam );
	
		return pBaseWin->RootWindowProc( hwnd, message, wParam, lParam );         // normal case
	}
}
