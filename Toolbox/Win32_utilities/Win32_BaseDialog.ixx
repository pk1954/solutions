// Win32_BaseDialog.ixx
//
// Win32_utilities

export module BaseDialog;

import WinBasics;
import GraphicsWindow;

static INT_PTR __stdcall BaseDialogProc(HWND const, UINT const, WPARAM const, LPARAM const);

export class BaseDialog: public GraphicsWindow
{
public:

	HWND StartBaseDialog(HWND const, VisCrit const &);

	virtual bool UserProc(UINT const, WPARAM const, LPARAM const);

private:

	friend static INT_PTR __stdcall BaseDialogProc(HWND const, UINT const, WPARAM const, LPARAM const);
};
