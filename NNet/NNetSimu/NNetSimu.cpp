// NNetSimu.cpp
//
// NNetSimu

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <Windows.h>
#include "Resource.h"
#include "CommCtrl.h"

import AppStartProtocol;
import Win32_Util_Resource;
import Win32_Util;
import SaveCast;
import Util;
import IoConstants;
import Trace;
import HiResTimer;
import MessagePump;
import Script;
import Scanner;
import Accelerators;
import NNetAppWindow;
import ScanViewer;

using std::make_unique;
using std::unique_ptr;
using std::wstring;
using std::wcout;
using std::endl;
using std::setw;
using std::left;

unique_ptr<NNetAppWindow> upApp;

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

	PerfCounter::Initialize();

	HiResTimer hrtimer;
	hrtimer.BeforeAction();

	SetThreadAffinityMask(GetCurrentThread(), 0x0001);

	INITCOMMONCONTROLSEX icex // load common control's DLL 
	{
		sizeof(INITCOMMONCONTROLSEX),
		ICC_STANDARD_CLASSES | 
		ICC_BAR_CLASSES | 
		ICC_TAB_CLASSES | 
		ICC_TREEVIEW_CLASSES  // for tooltips
	};

	static wstring const PRODUCT_NAME { L"NNetSimu 5.5 " + BUILD_MODE };

	SwitchWcoutTo(L"main_trace.out");

	PrintAppStartProtocol(PRODUCT_NAME);

	upApp = make_unique<NNetAppWindow>(PRODUCT_NAME);

	Accelerators acc;
	MessagePump  pump;
	pump.SetAccelTable(acc.Get());
	upApp->Start(pump);
	pump.RegisterWindow(upApp->GetWindowHandle(), false);

	wcout << setw(30) << left << COMMENT_START + L"App.Start " << PerfCounter::Ticks2wstring(hrtimer.AfterAction()) << endl;

	int iRetVal = pump.Run([]() { upApp->DoGameStuff(); });
	return iRetVal;
}
