// win32_aboutBox.cpp :
//
// Win32_utilities

#include "stdafx.h"
#include "win32_util_resource.h"
#include "win32_aboutBox.h"

static LPCWSTR COMPILE_TIMESTAMP = _T(__DATE__) L" " _T(__TIME__);

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
		SetDlgItemText( hDlg, IDD_TIMESTAMP, COMPILE_TIMESTAMP );
		return INT_PTR(true);

	case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            (void)EndDialog(hDlg, LOWORD(wParam));
            return INT_PTR(true);
        }
        break;

    default:
        break;
    }

    return INT_PTR(false);
}

void ShowAboutBox( HWND const hwndParent ) 
{
    HINSTANCE const hInstance = GetModuleHandle( nullptr );
    (void)DialogBox( hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hwndParent, About );
}
