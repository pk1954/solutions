// Win32_AboutBox.ixx
//
// Win32_utilities

module;

#include <Windows.h>

export module AboutBox;

import StdDialogBox;

export class AboutBox : public StdDialogBox
{
public:
    void Show(HWND);

private:
	void OnInitDlg(HWND const, WPARAM const, LPARAM const) final;
	bool OnOK(HWND const)                                  final;
};
 
