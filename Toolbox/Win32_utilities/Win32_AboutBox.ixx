// Win32_AboutBox.ixx
//
// Win32_utilities

module;

#include <string>
#include <Windows.h>
#include <tchar.h>

export module AboutBox;

import StdDialogBox;

using std::wstring;

export inline LPCWSTR COMPILE_TIMESTAMP{ _T(__DATE__) L" " _T(__TIME__) };

export class AboutBox : public StdDialogBox
{
public:
	void SetProductName(wstring const&);

private:
	wstring m_wstrProdName { L"???" };

	void OnInitDlg(HWND const, WPARAM const, LPARAM const) final;
	bool OnOK(HWND const)                                  final;
};
 
