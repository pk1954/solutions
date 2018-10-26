// win32_refreshRateDialog.h 
//

#pragma once

#include "Windowsx.h"

class RefreshRateDialog
{
public:
	static int Show( HWND const, int );

private:
	friend static INT_PTR CALLBACK dialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
