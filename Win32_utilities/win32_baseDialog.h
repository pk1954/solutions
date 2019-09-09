// win32_baseDialog.h : 
//
// win32_utilities

#pragma once

#include "win32_rootWindow.h"

class BaseDialog: public RootWindow
{
public:

    HWND StartBaseDialog
	( 
		HWND                  const, 
		LPCTSTR               const, 
		std::function<bool()> const 
	);

private:

	friend static INT_PTR CALLBACK BaseDialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
