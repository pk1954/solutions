// win32_messagePump.cpp
//
// Win32_appFramework


#include "commctrl.h"
#include "win32_appWindowInterface.h"
#include "win32_messagePump.h"

import StopWatch;

int MessagePump
(
	HINSTANCE      const hInstance,
	AppWindowInterface & appWindow,
	int            const iAppResourceId
)
{
	//	SetThreadAffinityMask(GetCurrentThread(), 0x0001);

	INITCOMMONCONTROLSEX icex // load common control's DLL 
	{
		sizeof(INITCOMMONCONTROLSEX),
		ICC_STANDARD_CLASSES | 
		ICC_BAR_CLASSES | 
		ICC_TAB_CLASSES | 
		ICC_TREEVIEW_CLASSES  // for tooltips
	};

	//bool bRes = InitCommonControlsEx(&icex);
	//assert(bRes);

	Stopwatch stopwatch;
	stopwatch.Start();
	appWindow.Start();
	stopwatch.Stop(L"App.Start");

	HACCEL const hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(iAppResourceId));
	HWND         hwndApp     = appWindow.GetWindowHandle();
	MSG          msg;
	bool         bRet;

	while (bRet = GetMessage(&msg, nullptr, 0, 0) != 0)    // Main message loop
	{
		assert(bRet != -1);
		if (!TranslateAccelerator(hwndApp, hAccelTable, &msg))
		{
			(void)TranslateMessage(&msg);
			(void)DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}
