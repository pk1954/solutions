// win32_aboutBox.cpp :
//
// Win32_utilities

module;

#include <string>
#include "Windows.h"

module AboutBox;

import Win32_Util_Resource;
import Win32_Util;
import Win32_Controls;
import Win32_PIXEL;
import Types;

using std::wstring;

void AboutBox::OnInitDlg(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
    ::SetWindowSize(hDlg, 340_PIXEL, 180_PIXEL, false);
    CreateStaticField  (hDlg, m_pwstrProdName->c_str(), 42, 14, 200, 20);
    CreateStaticField  (hDlg, L"Compiled:",             42, 40, 200, 20);
    CreateStaticField  (hDlg, COMPILE_TIMESTAMP,        42, 66, 200, 20);
    CreateButton       (hDlg, L"OK",                   200, 92,  50, 30, IDOK,  WS_GROUP);
}

bool AboutBox::OnOK(HWND const hDlg)
{
    (void)EndDialog(hDlg, 0);
    return true;
}
