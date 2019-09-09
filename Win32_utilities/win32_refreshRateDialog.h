// win32_refreshRateDialog.h 
//

#pragma once

#include <chrono>

class RefreshRateDialog
{
public:
	static std::chrono::milliseconds Show( HWND const, std::chrono::milliseconds );

private:
	friend static INT_PTR CALLBACK dialogProc( HWND const, UINT const, WPARAM const, LPARAM const );
};
