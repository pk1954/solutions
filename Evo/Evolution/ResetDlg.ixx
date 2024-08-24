// ResetDialog.ixx
//
// Evolution

export module ResetDlg;

import WinBasics;

export class ResetDialog
{
public:
	static int Show(HWND const);

	static int GetNewWidth();
	static int GetNewHeight();
	static int GetNewNrOfNeighbors();

private:
	friend static INT_PTR CALLBACK dialogProc(HWND const, UINT const, WPARAM const, LPARAM const);
};
