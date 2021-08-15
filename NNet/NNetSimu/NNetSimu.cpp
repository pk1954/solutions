// NNetSimu.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Resource.h"
#include "commctrl.h"
#include "win32_fatalError.h"
#include "win32_stopwatch.h"
#include "win32_messagePump.h"
#include "win32_NNetAppWindow.h"

int APIENTRY wWinMain
(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     int       nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

	NNetAppWindow App;

	// SetThreadAffinityMask(GetCurrentThread(), 0x0001);

	INITCOMMONCONTROLSEX icex // load common control's DLL 
	{
		sizeof(INITCOMMONCONTROLSEX),
		ICC_STANDARD_CLASSES | 
		ICC_BAR_CLASSES | 
		ICC_TAB_CLASSES | 
		ICC_TREEVIEW_CLASSES  // for tooltips
	};

	Stopwatch   stopwatch;
	MessagePump pump;
	stopwatch.Start();
	pump.SetAccelTable(hInstance, IDC_NNET_SIMU_MAIN);
	App.Start(pump);
	pump.RegisterWindow(App.GetWindowHandle(), false);
	stopwatch.Stop(Scanner::COMMENT_START + L"App.Start");
	int iRetVal = pump.Run();

	return iRetVal;
}
