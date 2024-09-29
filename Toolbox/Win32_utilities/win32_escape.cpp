// Win32_Escape.cpp
//
// Win32_utilities

module Win32_Util;

import WinBasics;

// EscapeKeyPressed - Peek message queue of current thread for ESC key pressed
//
// Use case: long running tasks blocking UI, when the only meaningful 
//	         user action is to abort task.

bool ::EscapeKeyPressed()
{
	MSG msg;
	if (PeekMessageW(&msg, nullptr, WM_KEYDOWN, WM_KEYDOWN, PM_REMOVE | PM_QS_INPUT) != 0)
	{
		if (msg.wParam == VK_ESCAPE)
			return true;
	}
	return false;
}
