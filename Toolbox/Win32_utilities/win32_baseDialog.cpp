// win32_baseDialog.cpp
//

#include "stdafx.h"
#include "win32_util.h"
#include "win32_rootWindow.h"
#include "win32_baseDialog.h"

HWND BaseDialog::StartBaseDialog
(
    HWND             const hwndParent,
    LPCTSTR          const lpTemplateName,
	function<bool()> const visibilityCriterion
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

	StartRootWindow( visibilityCriterion );

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
	switch (message)
	{
	case WM_INITDIALOG:
		(void)SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)lParam );
		return TRUE;

	default:
		break;
	}
	{
		BaseDialog * pBaseDialog = reinterpret_cast<BaseDialog *>(GetUserDataPtr( hwnd ));

		if ( ! RootWinIsReady( pBaseDialog ) )
			return FALSE;

		return pBaseDialog->RootWindowProc( hwnd, message, wParam, lParam );         // normal case
	}
}
