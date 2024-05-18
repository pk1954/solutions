// Win32_BaseDialog.ixx
//
// Win32_utilities

module;

#include <memory>
#include <Windows.h>

export module BaseDialog;

import Direct2D;
import GraphicsWindow;

using std::unique_ptr;

static INT_PTR CALLBACK BaseDialogProc(HWND const, UINT const, WPARAM const, LPARAM const);

export class BaseDialog: public GraphicsWindow
{
public:

	HWND StartBaseDialog(HWND const, VisCrit const &);

	virtual bool UserProc(UINT const, WPARAM const, LPARAM const);

private:

	friend static INT_PTR CALLBACK BaseDialogProc(HWND const, UINT const, WPARAM const, LPARAM const);
};
