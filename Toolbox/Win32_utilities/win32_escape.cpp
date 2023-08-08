// Win32_Escape.cpp
//
// Win32_utilities

module;

#include "Windows.h"

module Win32_Util;

// EscapeKeyPressed - Peek message queue of current thread for ESC key pressed
//
// Use case: long running tasks blocking UI, when the only meaningful 
//	         user action is to abort task.

bool ::EscapeKeyPressed()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, WM_KEYDOWN, WM_KEYDOWN, PM_REMOVE | PM_QS_INPUT) != 0)
	{
		if (msg.wParam == VK_ESCAPE)
			return true;
	}
	return false;
}
