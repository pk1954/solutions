// Win32_BaseDialog.cpp
//
// Win32_utilities

module;

#include <Windows.h>

module BaseDialog;

import Win32_Util;
import RootWindow;
import DialogTemplate;

HWND BaseDialog::StartBaseDialog
(
    HWND    const   hwndParent,
	VisCrit const & visibilityCriterion
)
{
    HWND hwnd = CreateDialogIndirectParam
    (
        nullptr,
        (LPCDLGTEMPLATEW) & EMPTY_TEMPLATE,
        hwndParent,
        BaseDialogProc,
        (LPARAM)this
   );

    DWORD err = GetLastError();

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
