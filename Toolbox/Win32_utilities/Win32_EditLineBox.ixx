// Win32_EditLineBox.ixx 
//
// Win32_utilities

module;

#include <string>
#include "Windows.h"

export module EditLineBox;

import StdDialogBox;

using std::wstring;

export class EditLineBox : public StdDialogBox
{
public:

	EditLineBox(wstring &, wstring const & = L"", wstring const &  = L"");
	virtual ~EditLineBox() = default;

	bool Show(HWND const);

private:
	wstring & m_wstrValue;    // the value to be edited in the dialog
	wstring   m_wstrTitle;    // the title bar text of the dialog
	wstring   m_wstrUnit;     // the text to be displayed right of the edit box	HWND hwndEditCtl { GetDlgItem(hDlg, IDD_EDIT_CTL) };
	HWND      m_hwndEditCtl;  

	void OnInitDlg(HWND const, WPARAM const, LPARAM const) final;
	bool OnOK     (HWND const)                             final; 
};
