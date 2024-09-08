// ViewerApp.cpp
//
// ViewerApp

import std;
import AppStartProtocol;
import Win32_Util_Resource;
import Win32_Util;
import WinBasics;
import Commands;
import WinCommand;
import NNetModelIO;
import NNetPreferences;
import MessagePump;
import NNetViewerWindow;
import IoUtil;
import NNetSignals;

using std::wstring;
using std::wcout;

int wWinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow
)
{
	static wstring const PRODUCT_NAME { L"ViewerApp 1.1 " + BUILD_MODE };

	//PerfCounter::Initialize();

	CommandStack     cmdStack;
	MessagePump      pump;
	NNetViewerWindow viewerWindow;
	bool             bViewerMode { true };

	InitCommCtrl();
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
