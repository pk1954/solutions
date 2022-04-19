// win32_stdDialogBox.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "win32_util.h"
#include "win32_util_resource.h"
#include "win32_stdDialogBox.h"

using std::bit_cast;

float StdDialogBox::Show
(
	HWND    const   hwndParent, 
	float           fValue,
	wstring const & wstrTitle,
	wstring const & wstrUnit
)
{
	m_wstrTitle = wstrTitle;
	m_wstrUnit  = wstrUnit;
	m_fValue    = fValue;

	if (IDOK == DialogBoxParam(nullptr, MAKEINTRESOURCE(IDD_STD_EDIT_DIALOG), hwndParent, dialogProc, bit_cast<LPARAM>(this)))
		fValue = m_fValue;

	return fValue;
}

void StdDialogBox::onInitDlg(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
	::SetWindowText(hDlg, m_wstrTitle.c_str());
	Util::SetEditField(GetDlgItem(hDlg, IDD_EDIT_CTL), m_fValue);
	::SetWindowText(GetDlgItem(hDlg, IDC_STATIC), m_wstrUnit.c_str());
	SendMessage(hDlg, DM_SETDEFID, IDOK, 0);
	SendMessage(GetDlgItem(hDlg, IDCANCEL), BM_SETSTYLE, BS_PUSHBUTTON, 0);
}

void StdDialogBox::onCommand(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
	int id { LOWORD(wParam) };
	switch (id)
	{
	case IDOK:
	{
		HWND hwndEditCtl { GetDlgItem(hDlg, IDD_EDIT_CTL) };
		if (Util::Evaluate(hwndEditCtl, m_fValue))
			EndDialog(hDlg, IDOK);
		else 
			SetFocus(hwndEditCtl);
	}
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
		pDlg->onInitDlg(hDlg, wParam, lParam);
		Util::SetUserDataPtr(hDlg, bit_cast<LONG_PTR>(pDlg));
		break;

	case WM_COMMAND:
		pDlg->onCommand(hDlg, wParam, lParam);
		break;

	default:
		break;
	}

	return 0;
}

