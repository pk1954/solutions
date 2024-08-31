// NNetSimu.cpp
//
// NNetSimu

#include <Windows.h>
#include "CommCtrl.h"

import std;
import WinBasics;
import FatalErrorMB;
import AppStartProtocol;
import Win32_Util_Resource;
import Win32_Util;
import SaveCast;
import IoUtil;
import Util;
import IoConstants;
import Trace;
import HiResTimer;
import MessagePump;
import RunTime;
import Accelerators;
import NNetModelIO;
import NNetAppWindow;
import NNetViewerWindow;
import ScanViewer;
import Resource;

using std::make_unique;
using std::unique_ptr;
using std::wstring;
using std::wcout;
using std::endl;
using std::setw;
using std::left;

int wWinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow
)
{
	PerfCounter::Initialize();

	HiResTimer hrtimer;
	hrtimer.BeforeAction();

	//SetThreadAffinityMask(GetCurrentThread(), 0x0001);

	INITCOMMONCONTROLSEX icex // load common control's DLL 
	{
		sizeof(INITCOMMONCONTROLSEX),
		ICC_STANDARD_CLASSES | 
		ICC_BAR_CLASSES | 
		ICC_TAB_CLASSES | 
		ICC_TREEVIEW_CLASSES  // for tooltips
	};

	static wstring const PRODUCT_NAME { L"NNetSimu 6.2 " + BUILD_MODE };

	SwitchWcoutTo(L"main_trace.out");

	PrintAppStartProtocol(PRODUCT_NAME);
	DefineUtilityWrapperFunctions();
	NNetModelIO::Initialize();

	Accelerators acc;
	MessagePump  pump;
	pump.SetAccelTable(acc.Get());

	bool bViewerMode { true };

	unique_ptr<NNetAppWindow> upApp { make_unique<NNetAppWindow>(PRODUCT_NAME, pump) };

	pump.RegisterWindow(upApp->GetWindowHandle(), false);

	wcout << setw(30) << left << COMMENT_START + L"App.Start " << PerfCounter::Ticks2wstring(hrtimer.AfterAction()) << endl;

	int iRes { 0 };
	try 
	{
		iRes = pump.Run([&upApp]() { upApp->DoGameStuff(); });
	} catch (const std::exception& e) 
	{
		FatalErrorMB::Happened(3, L"Caught C++ exception: " + ConvertToWideString(e.what()));
		//std::cerr << "Caught C++ exception: " << e.what() << std::endl;
		iRes = -3;
	}
	return iRes;
}
