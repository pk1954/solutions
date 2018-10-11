// win32_baseDialog.cpp
//

#include "stdafx.h"
#include "win32_util.h"
#include "win32_rootWindow.h"
#include "win32_baseDialog.h"

//lint -e1924                C-style cast

BaseDialog::BaseDialog( )
    : RootWindow( )
{ }

HWND BaseDialog::StartBaseDialog
(
    HWND    const hwndParent,
    LPCTSTR const lpTemplateName
)
{
    HWND hwnd = CreateDialogParam
    (
        nullptr,
        lpTemplateName,
        hwndParent,
        BaseDialogProc,
        ( LPARAM )this
    );

    SetWindowHandle( hwnd );
    return hwnd;
}

static INT_PTR CALLBACK BaseDialogProc
( 
    HWND   const hwnd,
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
    if ( message == WM_INITDIALOG )
    {
        (void)SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)lParam );
        return TRUE;
    }
    else
    {
        BaseDialog * pBaseDialog = (BaseDialog *)GetWindowLongPtr( hwnd, GWLP_USERDATA );

        return ( RootWinIsReady( pBaseDialog ) )
            ? pBaseDialog->UserProc( message, wParam, lParam )         // normal case
            : FALSE;
    }
}
