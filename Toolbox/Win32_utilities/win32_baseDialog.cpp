// Win32_BaseDialog.cpp
//
// Win32_utilities

module;

#include <Windows.h>

module BaseDialog;

import Win32_Util;
import RootWindow;

HWND BaseDialog::StartBaseDialog
(
    HWND    const   hwndParent,
	VisCrit const & visibilityCriterion
)
{
    static struct 
    {
        DWORD style;
        DWORD dwExtendedStyle;
        WORD  ccontrols;
        short x;
        short y;
        short cx;
        short cy;
        WORD  menu;              // name or ordinal of a menu resource
        WORD  windowClass;       // name or ordinal of a window class
        WCHAR wszTitle[1];       // title string of the dialog box
    } data = 
    {
       WS_POPUP|WS_CAPTION|WS_SYSMENU|DS_MODALFRAME, 
       0x0,                     // exStyle;
       0,                       // ccontrols
       0, 0, 0, 0,
       0,                       // menu: none
       0,                       // window class: none
       L""                      // Window caption
    };

    HWND hwnd = CreateDialogIndirectParam
    (
        nullptr,
        (LPCDLGTEMPLATEW) & data,
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
