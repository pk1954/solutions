// win32_baseDialog.h : 
//
// win32_utilities

#pragma once

#include <Windows.h>
#include "win32_rootWindow.h"

static INT_PTR CALLBACK BaseDialogProc(HWND const, UINT const, WPARAM const, LPARAM const);

class BaseDialog: public RootWindow
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
