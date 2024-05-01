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
import NNetWin32;
import ScanViewerWindow;

using std::make_unique;
using std::unique_ptr;
using std::wstring;
using std::wcout;
using std::endl;
using std::setw;
using std::left;

unique_ptr<ScanViewerWindow> upApp;

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

	INITCOMMONCONTROLSEX icex // load common control's DLL 
	{
		sizeof(INITCOMMONCONTROLSEX),
		ICC_STANDARD_CLASSES | 
		ICC_BAR_CLASSES | 
		ICC_TAB_CLASSES | 
		ICC_TREEVIEW_CLASSES  // for tooltips
	};

	static wstring const PRODUCT_NAME { L"NNetViewer 1.0 " + BUILD_MODE };

	SwitchWcoutTo(L"viewer.out");
	PrintAppStartProtocol(PRODUCT_NAME);

	upApp = make_unique<ScanViewerWindow>(PRODUCT_NAME);

	MessagePump  pump;
	upApp->Start(pump);
	pump.RegisterWindow(upApp->GetWindowHandle(), false);

	wcout << setw(30) << left << COMMENT_START + L"App.Start " << PerfCounter::Ticks2wstring(hrtimer.AfterAction()) << endl;

	int iRetVal = pump.Run([]() { upApp->DoGameStuff(); });
	return iRetVal;
}
