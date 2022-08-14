// Win32_BaseDialog.h : 
//
// Win32_utilities

module;

#include <Windows.h>
#include "win32_rootWindow.h"

export module BaseDialog;

static INT_PTR CALLBACK BaseDialogProc(HWND const, UINT const, WPARAM const, LPARAM const);

export class BaseDialog: public RootWindow
{
public:

	HWND StartBaseDialog
	(
		HWND    const, 
		LPCTSTR const, 
		VisCrit const &
	);

protected:

	virtual bool UserProc(UINT const, WPARAM const, LPARAM const);

private:

	friend static INT_PTR CALLBACK BaseDialogProc(HWND const, UINT const, WPARAM const, LPARAM const);
};
