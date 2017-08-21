// win32_aboutBox.cpp :
//

#include "stdafx.h"
#include "resource.h"
#include "win32_aboutBox.h"

//lint -e1924                C-style cast
//lint -esym( 550, lParam )  lParam nut used   

static INT_PTR CALLBACK About
( 
    HWND   const hDlg, 
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
    UNREFERENCED_PARAMETER( lParam );

    switch (message)
    {
    case WM_INITDIALOG:
        return static_cast<INT_PTR>(TRUE);

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            (void)EndDialog(hDlg, LOWORD(wParam));
            return static_cast<INT_PTR>(TRUE);
        }
        break;

    default:
        break;
    }

    return static_cast<INT_PTR>(FALSE);
}

void ShowAboutBox( HWND const hWndParent )
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );
    (void)DialogBox( hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWndParent, About );
}
