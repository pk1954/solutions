// Evolution.cpp
//
// Evolution

import WinBasics;
import MessagePump;
import EvoAppWindow;
import Resource;

int APIENTRY wWinMain
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow
)
{
	//	SetThreadAffinityMask(GetCurrentThread(), 0x0001);
	StdOutConsole();
	InitCommCtrl();

	EvoAppWindow appWin;
	appWin.Start();

	MessagePump pump;

	return pump.Run([&appWin]() { appWin.DoGameStuff(); });
}
