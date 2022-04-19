// win32_stdDialogBox.h
//
// Win32_utilities

#pragma once

#include <string>

using std::wstring;

static INT_PTR CALLBACK dialogProc(HWND const, UINT const, WPARAM const, LPARAM const);

class StdDialogBox
{
public:
	float Show(HWND const, float, wstring const &, wstring const &);

private:

	float   m_fValue;     // the value to be edited in the dialog
	wstring m_wstrUnit;   // the text to be displayed right of the edit box
	wstring m_wstrTitle;  // the title bar text of the dialog

	virtual void onInitDlg(HWND const, WPARAM const, LPARAM const);
	virtual void onCommand(HWND const, WPARAM const, LPARAM const);

	friend static INT_PTR CALLBACK dialogProc(HWND const, UINT const, WPARAM const, LPARAM const);
};
