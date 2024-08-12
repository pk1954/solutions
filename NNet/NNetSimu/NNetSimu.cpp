// NNetSimu.cpp
//
// NNetSimu

#include <Windows.h>
#include "Resource.h"
#include "CommCtrl.h"

import std;
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
import small_icon;
import NNetSimu_icon;

using std::make_unique;
using std::unique_ptr;
using std::wstring;
using std::wcout;
using std::endl;
using std::setw;
using std::left;

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

	//HICON hIconBig   = CreateIconFromResourceEx((PBYTE)NNetSimu_icon_data.data(), (DWORD)NNetSimu_icon_data.size(), TRUE, 0x00030000, 0, 0, LR_DEFAULTSIZE);
	//DWORD res = GetLastError ();
	//HICON hIconSmall = CreateIconFromResourceEx((PBYTE)small_icon_data   .data(), (DWORD)small_icon_data   .size(), TRUE, 0x00030000, 0, 0, LR_DEFAULTSIZE);
 //   ::SendMessage(upApp->GetWindowHandle(), WM_SETICON, ICON_BIG,   (LPARAM)hIconBig);
 //   ::SendMessage(upApp->GetWindowHandle(), WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall);

	pump.RegisterWindow(upApp->GetWindowHandle(), false);

	wcout << setw(30) << left << COMMENT_START + L"App.Start " << PerfCounter::Ticks2wstring(hrtimer.AfterAction()) << endl;

	return pump.Run([&upApp]() { upApp->DoGameStuff(); });
}
