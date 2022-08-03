// Win32_StdDialogBox.cpp
//
// Toolbox\Win32_utilities

module;

#include "win32_util.h"
#include "win32_util_resource.h"

module StdDialogBox;

using std::bit_cast;

bool StdDialogBox::Show
(
	HWND const hwndParent,
	int  const idDialog
)
{
	INT_PTR res 
	{ 
		DialogBoxParam
		(
			nullptr, 
			MAKEINTRESOURCE(idDialog), 
			hwndParent, 
			dialogProc, 
			bit_cast<LPARAM>(this)
		) 
	};

	return res == IDOK;
}

void StdDialogBox::OnCommand(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
	int id { LOWORD(wParam) };
	switch (id)
	{
	case IDOK:
		if (OnOK(hDlg))
			EndDialog(hDlg, IDOK);
		break;

	case IDCANCEL:
		EndDialog(hDlg, IDCANCEL);
		break;

	default:
		break;
	}
}

INT_PTR CALLBACK dialogProc
(
	HWND   const hDlg, 
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	auto pDlg = bit_cast<StdDialogBox *>(Util::GetUserDataPtr(hDlg));
	switch (message)
	{
	case WM_INITDIALOG:
		pDlg = bit_cast<StdDialogBox *>(lParam);
		Util::SetUserDataPtr(hDlg, bit_cast<LONG_PTR>(pDlg));
		pDlg->OnInitDlg(hDlg, wParam, lParam);
		break;

	case WM_COMMAND:
		pDlg->OnCommand(hDlg, wParam, lParam);
		break;

	default:
		break;
	}

	return 0;
}

