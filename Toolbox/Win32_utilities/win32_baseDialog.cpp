// Win32_BaseDialog.cpp
//
// Win32_utilities

module;

#include <bit>
#include <functional>
#include <memory>
#include <Windows.h>

module BaseDialog;

import Win32_Util;
import RootWindow;
import DialogTemplate;

using std::bind_front;
using std::make_unique;
using std::bit_cast;

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
	m_upGraphics = D2D_driver::Create(hwnd);

	return hwnd;
}

bool BaseDialog::UserProc(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
	return RootWindow::CommonMessageHandler(message, wParam, lParam);
}

static INT_PTR CALLBACK BaseDialogProc
(
    HWND   const hDlg,
    UINT   const message, 
    WPARAM const wParam, 
    LPARAM const lParam 
)
{
	auto pDlg = bit_cast<BaseDialog*>(::GetUserDataPtr(hDlg));
	switch (message)
	{
		case WM_INITDIALOG:
			::SetUserDataPtr(hDlg, lParam);
			return true;

		case WM_PAINT:
		{
			D2D_driver* pGraphics { pDlg->m_upGraphics.get() };
			if (pGraphics)
				pGraphics->Display(bind_front(&BaseDialog::PaintGraphics, pDlg));
		}
		break;

		default: 
			if (auto pBaseDialog = static_cast<BaseDialog *>(GetRootWindow(hDlg)))
				return pBaseDialog->UserProc(message, wParam, lParam);         // normal case
	}

	return false;
}
