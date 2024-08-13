// Win32_AboutBox.ixx
//
// Win32_utilities

export module AboutBox;

import std;
import WinBasics;
import StdDialogBox;

using std::wstring;

export class AboutBox : public StdDialogBox
{
public:
	void SetProductName(wstring const & wstrProdName)
	{
		m_pwstrProdName = &wstrProdName;
	}

private:
	wstring const * m_pwstrProdName { nullptr };

	void OnInitDlg(HWND const, WPARAM const, LPARAM const) final;
	bool OnOK(HWND const)                                  final;
};
 
