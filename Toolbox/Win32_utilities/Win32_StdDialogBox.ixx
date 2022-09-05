// Win32_StdDialogBox.ixx
//
// ToolBox\Win32_utilities

module;

#include "Windows.h"

export module StdDialogBox;

static INT_PTR CALLBACK dialogProc(HWND const, UINT const, WPARAM const, LPARAM const);

export class StdDialogBox
{
public:
	virtual bool Show     (HWND const, int const);
	virtual void OnCommand(HWND const, WPARAM const, LPARAM const);
	virtual void OnInitDlg(HWND const, WPARAM const, LPARAM const) = 0;
	virtual bool OnOK     (HWND const)                             = 0; 

private:

	friend static INT_PTR CALLBACK dialogProc(HWND const, UINT const, WPARAM const, LPARAM const);
};
