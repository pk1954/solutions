// Win32_EditLineBox.cpp
//
// Win32_utilities

module;

#include <bit>
#include <string>
#include "Windows.h"

module EditLineBox;

import Win32_Util_Resource;
import StdDialogBox;
import Win32_Controls;
import Win32_PIXEL;
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

bool EditLineBox::OnOK(HWND const hDlg)
{
	bool bOK { ::Evaluate(m_hwndEditCtl, m_wstrValue) };
	if (bOK)
		EndDialog(hDlg, IDOK);
	else 
		SetFocus(m_hwndEditCtl);
	return bOK;
}

void EditLineBox::OnInitDlg(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
	::SetWindowSize(hDlg, 340_PIXEL, 180_PIXEL, false);
	m_hwndEditCtl =
	CreateEditField  (hDlg,                      42, 40, 150, 20);
	CreateStaticField(hDlg, m_wstrUnit.c_str(), 195, 40, 200, 20);
	CreateButton     (hDlg, L"OK",              100, 92,  50, 30, IDOK, WS_GROUP);
	CreateButton     (hDlg, L"Cancel",          200, 92,  50, 30, IDOK, WS_GROUP);
	::SetWindowText(m_hwndEditCtl, m_wstrValue.c_str());
	::SetWindowText(hDlg, m_wstrTitle.c_str());
	SendMessage(hDlg, DM_SETDEFID, IDOK, 0);
	SendMessage(GetDlgItem(hDlg, IDCANCEL), BM_SETSTYLE, BS_PUSHBUTTON, 0);
}
