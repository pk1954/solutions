// Blokus.cpp
//
// Blokus

import std;
import IoConstants;
import WinBasics;
import FatalErrorMB;
import AppStartProtocol;
import Win32_Util_Resource;
import Win32_Util;
import PerfCounter;
import HiResTimer;
import Accelerators;
import MessagePump;
import BlokusAppWindow;
import BlokusCore;

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
	static wstring const PRODUCT_NAME { L"Blokus 0.1 " + BUILD_MODE };

	PerfCounter::Initialize();

	HiResTimer hrtimer;
	hrtimer.BeforeAction();

	InitCommCtrl();
	SwitchWcoutTo(L"main_trace.out");
	PrintAppStartProtocol(PRODUCT_NAME);

	Accelerators acc;
	MessagePump  pump;
	pump.SetAccelTable(acc.Get());

	PlayerTypes::Initialize();
	Components::Initialize();
	BlokusPreferences::Initialize();

	unique_ptr<BlokusAppWindow> upApp { make_unique<BlokusAppWindow>(PRODUCT_NAME, pump) };

	pump.RegisterWindow(upApp->GetWindowHandle(), false);

	wcout << setw(30) << left << COMMENT_START + L"App.Start " << PerfCounter::Ticks2wstring(hrtimer.AfterAction()) << endl;

	int iRes { 0 };
	try 
	{
		iRes = pump.Run([&upApp]() { upApp->DoGameStuff(); });
	} catch (const std::exception& e) 
	{
		FatalErrorMB::Happened(3, L"Caught C++ exception: " + ConvertToWideString(e.what()));
		std::cerr << "Caught C++ exception: " << e.what() << std::endl;
		iRes = -3;
	}
	return iRes;
}
