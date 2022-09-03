// win32_aboutBox.cpp :
//
// Win32_utilities

module;

#include "Windows.h"
#include "win32_util_resource.h"

module AboutBox;

void AboutBox::OnInitDlg(HWND const hDlg, WPARAM const wParam, LPARAM const lParam)
{
    SetDlgItemText(hDlg, IDD_TIMESTAMP, COMPILE_TIMESTAMP);
}

bool AboutBox::OnOK(HWND const hDlg)
{
    (void)EndDialog(hDlg, 0);
    return true;
}

void AboutBox::Show(HWND const hwndParent) 
{
    StdDialogBox::Show(hwndParent, IDD_ABOUTBOX);
}
