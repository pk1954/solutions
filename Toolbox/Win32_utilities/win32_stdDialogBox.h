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
	virtual bool Show(HWND const, int const);

protected:

	virtual void OnInitDlg(HWND const, WPARAM const, LPARAM const) = 0;
	virtual bool OnOK(HWND const)                                  = 0; 
	virtual void OnCommand(HWND const, WPARAM const, LPARAM const);

private:


	friend static INT_PTR CALLBACK dialogProc(HWND const, UINT const, WPARAM const, LPARAM const);
};
