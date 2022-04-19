// win32_stdDialogBox.h
//
// Win32_utilities

#pragma once

#include <string>

using std::wstring;

static INT_PTR CALLBACK dialogProc(HWND const,	UINT const, WPARAM const, LPARAM const);

class StdDialogBox
{
public:
	static float Show(HWND const, float, wstring const &, wstring const &);
	static void  SetParameterValue(HWND const, float const);
	static void  SetParameterValue(HWND const, wstring const);
	static bool  Evaluate(HWND const, float &);

private:

	friend static INT_PTR CALLBACK dialogProc(HWND const,	UINT const, WPARAM const, LPARAM const);
};
