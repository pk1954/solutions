// BaseAppWindow.cpp
//
// EvoWindows

module BaseAppWindow;

import std;
import Trace;
import HistoryLib;
import WinBasics;
import Win32_Util_Resource;
//import UtilityWrappers;
import Resource;
import AboutBox;
import BaseWindow;
import AppMenu;
import BaseWindow;
import WinManager;
import WinHistLib;
import StatusBar;
import WorkThread;

using namespace std::literals::chrono_literals;

BaseAppWindow::BaseAppWindow()
 : 	m_hwndApp(nullptr),
	m_bStarted(FALSE),
	m_bUseHistorySystem(FALSE),
	m_hwndConsole(nullptr),
	m_pAppMenu(nullptr),
	m_pModelWindow(nullptr),
	m_pHistorySystem(nullptr),
	//m_pWorkThread(nullptr),
	m_traceStream()
{
}

void BaseAppWindow::Initialize
(
	//WorkThread * const pWorkThread,
	bool                  const bUseHistorySystem
)
{
	//	_CrtSetAllocHook(MyAllocHook);

	//m_pWorkThread = pWorkThread;
	m_bUseHistorySystem = bUseHistorySystem;

	m_hwndConsole = GetConsoleWindow();
	SetWindowPos(m_hwndConsole, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	DefineUtilityWrapperFunctions();

	m_traceStream = OpenTraceFile(L"main_trace.out");

	m_pWorkThread->Initialize(& m_traceStream);

	m_StatusBar     .SetRefreshRate(300ms);
	m_HistWindow    .SetRefreshRate(200ms); 
	m_HistInfoWindow.SetRefreshRate(300ms);

	m_hwndApp = StartBaseWindow
	(
		nullptr, 
		L"ClassAppWindow", 
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);
}

BaseAppWindow::~BaseAppWindow() 
{
};

void BaseAppWindow::Start(BaseWindow * const pModelWindow)
{
	m_pModelWindow = pModelWindow;

	m_pAppMenu->Start();
	m_StatusBar.Start
	(
		m_hwndApp //, m_pWorkThread
	);

	m_WinManager.AddWindow(L"IDM_CONS_WINDOW", IDM_CONS_WINDOW, m_hwndConsole,                 TRUE,  TRUE );
	m_WinManager.AddWindow(L"IDM_APPL_WINDOW", IDM_APPL_WINDOW, m_hwndApp,                     TRUE,  TRUE );
	m_WinManager.AddWindow(L"IDM_STATUS_BAR",  IDM_STATUS_BAR,  m_StatusBar.GetWindowHandle(), FALSE, FALSE);

	if (m_bUseHistorySystem)
	{
		m_pHistorySystem = HistorySystem::CreateHistorySystem();  // deleted in Stop function
		m_HistWindow    .Start(m_hwndApp, m_pHistorySystem); //, m_pWorkThread);
		m_HistInfoWindow.Start(m_hwndApp, nullptr);
		m_WinManager.AddWindow(L"IDM_HIST_WINDOW", IDM_HIST_WINDOW, m_HistWindow,     FALSE, FALSE); 
		m_WinManager.AddWindow(L"IDM_HIST_INFO",   IDM_HIST_INFO,   m_HistInfoWindow, TRUE,  FALSE);
		m_HistInfoWindow.SetHistorySystem(m_pHistorySystem);
	}

	m_StatusBar.Show(TRUE);
	pModelWindow->Show(TRUE);

	adjustChildWindows();

	m_bStarted = TRUE;
}

void BaseAppWindow::Stop()
{
	m_bStarted = FALSE;

	m_StatusBar.Stop();
	m_pAppMenu->Stop();

	if (m_bUseHistorySystem)
	{
		//m_HistInfoWindow.Stop();
		//m_HistWindow    .Stop();
		delete m_pHistorySystem;   
		m_pHistorySystem = nullptr;
	}
}

void BaseAppWindow::adjustChildWindows()
{
	HWND hwndApp = GetParent(m_pModelWindow->GetWindowHandle());

	PixelRectSize pntAppClientSize(GetClRectSize(hwndApp));
	PIXEL pixAppClientWinWidth  = pntAppClientSize.GetX();
	PIXEL pixAppClientWinHeight = pntAppClientSize.GetY();

	if (pntAppClientSize.IsNotZero())
	{
		m_StatusBar.Resize();
		pixAppClientWinHeight -= m_StatusBar.GetHeight();

		if (m_bUseHistorySystem)
		{
			static PIXEL const HIST_WINDOW_HEIGHT = 30_PIXEL;
			pixAppClientWinHeight -= HIST_WINDOW_HEIGHT, 
			m_HistWindow.Move   // adapt history window to new size
			(
				0_PIXEL, 
				pixAppClientWinHeight, 
				pixAppClientWinWidth, 
				HIST_WINDOW_HEIGHT, 
				TRUE 
			); 
		}

		m_pModelWindow->Move  // use all available space for model window
		(
			0_PIXEL, 
			0_PIXEL, 
			pixAppClientWinWidth, 
			pixAppClientWinHeight, 
			TRUE 
		);
	}
}

bool BaseAppWindow::UserProc
(
	UINT   const message, 
	WPARAM const wParam, 
	LPARAM const lParam 
)
{
	switch (message)
	{

	case WM_ENTERMENULOOP:
		if (wParam == FALSE)
			m_pAppMenu->AdjustVisibility();
		break;

	case WM_COMMAND:
		ProcessAppCommand(wParam, lParam);
		return FALSE;

	case WM_PAINT:
	{
		static COLORREF const CLR_GREY = MakeRGB(128, 128, 128);
		PAINTSTRUCT   ps;
		HDC           hDC = BeginPaint(&ps);
		FillBackground(hDC, CLR_GREY);
		(void)EndPaint(&ps);
		return FALSE;
	}

	case WM_SIZE:
	case WM_MOVE:
		adjustChildWindows();
		break;

	case WM_CLOSE:
		if (m_bStarted)
		{
			m_WinManager.StoreWindowConfiguration();
			Stop();
		}
		DestroyWindow();        
		return TRUE;  

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(message, wParam, lParam);
}

bool BaseAppWindow::ProcessFrameworkCommand(WPARAM const wParam, LPARAM const lParam)
{
	int const wmId = LoWord(wParam);

	switch (wmId)
	{
	case IDM_ABOUT:
		ShowAboutBox(GetWindowHandle());
		break;

	case IDM_EXIT:
		PostMessageW(WM_CLOSE, 0, 0);
		break;

	case IDM_MAIN_WINDOW:
		::SendMessageW(m_WinManager.GetHWND(wmId), WM_COMMAND, IDM_WINDOW_ON, 0);
		break;

	case IDM_HIST_INFO:
	case IDM_HIST_WINDOW:
		Assert(m_bUseHistorySystem);
		::SendMessageW(m_WinManager.GetHWND(wmId), WM_COMMAND, IDM_WINDOW_ON, 0);
		break;

	case IDM_FORWARD:
		m_pWorkThread->PostGenerationStep();
		break;

	case IDM_BACKWARDS:
		Assert(m_bUseHistorySystem);
		m_pWorkThread->PostPrevGeneration();
		break;

	case IDM_EDIT_UNDO:
		Assert(m_bUseHistorySystem);
		m_pWorkThread->PostUndo();
		break;

	case IDM_EDIT_REDO:
		Assert(m_bUseHistorySystem);
		m_pWorkThread->PostRedo();
		break;

	case IDM_RUN:
		m_pWorkThread->PostRunGenerations(true);
		break;

	case IDM_STOP:
		m_pWorkThread->PostStopComputation();
		break;

	case IDM_HIST_BUFFER_FULL:
		Assert(m_bUseHistorySystem);
		std::wcout << L"History buffer is full" << std::endl;
		(void)MessageBeep(MB_ICONWARNING);
		ProcessFrameworkCommand(IDM_STOP);
		break;

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}
