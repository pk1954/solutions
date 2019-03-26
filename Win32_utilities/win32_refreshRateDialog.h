// win32_refreshRateDialog.h 
//

#pragma once

#include <chrono>
#include "Windowsx.h"

using namespace std::chrono;

class RefreshRateDialog
{
public:
	static milliseconds Show( HWND const, milliseconds );

private:
	friend static INT_PTR CALLBACK dialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
