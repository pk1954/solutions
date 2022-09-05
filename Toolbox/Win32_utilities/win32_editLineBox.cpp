// Win32_EditLineBox.cpp
//
// Win32_utilities

module;

#include <string>
#include "Windows.h"
#include "win32_util_resource.h"

module EditLineBox;

import StdDialogBox;
import Win32_Util;

using std::wstring;
using std::to_wstring;
using std::bit_cast;

EditLineBox::EditLineBox
(
	wstring       & wstrValue,
	wstring const & wstrTitle,
	wstring const & wstrUnit
)
  : m_wstrValue(wstrValue),
    m_wstrUnit (wstrUnit),
    m_wstrTitle(wstrTitle)
{}

bool EditLineBox::Show(HWND const hwndParent)
{
	return StdDialogBox::Show(hwndParent, IDD_STD_EDIT_DIALOG);
}

bool EditLineBox::OnOK(HWND const hDlg)
{
	HWND hwndEditCtl { GetDlgItem(hDlg, IDD_EDIT_CTL) };
	bool bOK         { Util::Evaluate(hwndEditCtl, m_wstrValue) };
	if (bOK)
		EndDialog(hDlg, IDOK);
	else 
		SetFocus(hwndEditCtl);
	return bOK;
}

void EditLineBox::OnInitDlg(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
	{
		HWND const hwndEditCtl { GetDlgItem(hDlg, IDD_EDIT_CTL) };
		LONG_PTR   style       { GetWindowLongPtr(hwndEditCtl, GWL_STYLE) };
		style &= ~ES_RIGHT;
		SetWindowLongPtr(hwndEditCtl, GWL_STYLE, style);
		Util::SetEditField(hwndEditCtl, m_wstrValue.c_str());
	}
	::SetWindowText(hDlg, m_wstrTitle.c_str());
	::SetWindowText(GetDlgItem(hDlg, IDC_STATIC), m_wstrUnit.c_str());
	SendMessage(hDlg, DM_SETDEFID, IDOK, 0);
	SendMessage(GetDlgItem(hDlg, IDCANCEL), BM_SETSTYLE, BS_PUSHBUTTON, 0);
}
