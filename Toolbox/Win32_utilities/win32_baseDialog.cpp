// win32_baseDialog.cpp
//

#include "stdafx.h"
#include "win32_util.h"
#include "win32_rootWindow.h"
#include "win32_baseDialog.h"

HWND BaseDialog::StartBaseDialog
(
    HWND    const   hwndParent,
    LPCTSTR const   lpTemplateName,
	VisCrit const & visibilityCriterion
)
{
	HWND hwnd = CreateDialogParam
    (
        nullptr,
        lpTemplateName,
        hwndParent,
        BaseDialogProc,
        (LPARAM)this
   );

	SetWindowHandle(hwnd);
	StartRootWindow(visibilityCriterion);

	return hwnd;
}

bool BaseDialog::UserProc(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
	return RootWindow::CommonMessageHandler(message, wParam, lParam);
}

static INT_PTR CALLBACK BaseDialogProc
(
    HWND   const hwnd,
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
	if (message == WM_INITDIALOG)
	{
		Util::SetUserDataPtr(hwnd, lParam);
		return INT_PTR(true);
	}
	else 
	{
		if (auto pBaseDialog = static_cast<BaseDialog *>(GetRootWindow(hwnd)))
			return pBaseDialog->UserProc(message, wParam, lParam);         // normal case
	}

	return INT_PTR(false);
}
