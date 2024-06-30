// ViewerApp.cpp
//
// ViewerApp

#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <Windows.h>
#include "CommCtrl.h"

import AppStartProtocol;
import Win32_Util_Resource;
import Win32_Util;
import WinCommand;
import Commands;
import SaveCast;
import IoUtil;
import Util;
import IoConstants;
import Trace;
import MessagePump;
import Script;
import Scanner;
import NNetSignals;
import NNetModelIO;
import NNetPreferences;
import NNetViewerWindow;
import ScanViewer;

using std::wstring;
using std::wcout;

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

	//PerfCounter::Initialize();

	INITCOMMONCONTROLSEX icex // load common control's DLL 
	{
		sizeof(INITCOMMONCONTROLSEX),
		ICC_STANDARD_CLASSES | 
		ICC_BAR_CLASSES | 
		ICC_TAB_CLASSES | 
		ICC_TREEVIEW_CLASSES  // for tooltips
	};

	static wstring const PRODUCT_NAME { L"ViewerApp 1.0 " + BUILD_MODE };

	CommandStack     cmdStack;
	MessagePump      pump;
	NNetViewerWindow viewerWindow;
	bool             bViewerMode { true };

	SwitchWcoutTo(L"main_trace.out");

	PrintAppStartProtocol(PRODUCT_NAME);
	DefineUtilityWrapperFunctions();
	NNetModelIO::AddModelWrapper<MonitorScrollState>(L"MonitorScrollState");  // Every model contains MonitorScrollState
	NNetModelIO::Initialize();
	NNetPreferences::Initialize(bViewerMode);    
	WinCommand::Initialize(&cmdStack);
	cmdStack.Initialize(nullptr);                // DeleteScan is a command
	Preferences::ReadPreferences();              // colorLut

	pump.RegisterWindow(viewerWindow.GetWindowHandle(), false);

	return pump.Run([&viewerWindow]() {});
}
