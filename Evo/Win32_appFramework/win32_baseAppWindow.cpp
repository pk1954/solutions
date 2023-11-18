// win32_baseAppWindow.cpp
//
// Win32_appFramework


#include <chrono>
#include "HistorySystem.h"
#include "win32_util_resource.h"
#include "UtilityWrappers.h"
#include "win32_aboutBox.h"
#include "win32_modelWindow.h"
#include "win32_appMenu.h"
#include "win32_baseWindow.h"
#include "win32_winManager.h"
#include "win32_histWindow.h"
#include "win32_historyInfo.h"
#include "win32_status.h"
#include "win32_WorkThreadInterface.h"
#include "win32_baseAppWindow.h"

import Trace;

using namespace std::literals::chrono_literals;

BaseAppWindow::BaseAppWindow()
 : 	m_hwndApp(nullptr),
	m_bStarted(FALSE),
	m_bUseHistorySystem(FALSE),
	m_hwndConsole(nullptr),
	m_pAppMenu(nullptr),
	m_pModelWindow(nullptr),
	m_pHistorySystem(nullptr),
	m_pWorkThreadInterface(nullptr),
	m_traceStream()
{
}

void BaseAppWindow::Initialize
(
	WorkThreadInterface * const pWorkThreadInterface,
	BOOL                  const bUseHistorySystem
)
{
	//	_CrtSetAllocHook(MyAllocHook);

	m_pWorkThreadInterface = pWorkThreadInterface;
	m_bUseHistorySystem = bUseHistorySystem;

	m_hwndConsole = GetConsoleWindow();
	SetWindowPos(m_hwndConsole, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	DefineUtilityWrapperFunctions();

	m_traceStream = OpenTraceFile(L"main_trace.out");

	m_pWorkThreadInterface->Initialize(& m_traceStream);

	m_StatusBar     .SetRefreshRate(300ms);
	m_HistWindow    .SetRefreshRate(200ms); 
	m_HistInfoWindow.SetRefreshRate(300ms);

	m_hwndApp = StartBaseWindow
	(
		nullptr, 
		CS_HREDRAW | CS_VREDRAW, 
		L"ClassAppWindow", 
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
		nullptr,
		nullptr
	);
}

BaseAppWindow::~BaseAppWindow() 
{
};

void BaseAppWindow::Start(ModelWindow * const pModelWindow)
{
	m_pModelWindow = pModelWindow;

	m_pAppMenu->Start();
	m_StatusBar.Start(m_hwndApp, m_pWorkThreadInterface);

	m_WinManager.AddWindow(L"IDM_CONS_WINDOW", IDM_CONS_WINDOW, m_hwndConsole,                 TRUE,  TRUE );
	m_WinManager.AddWindow(L"IDM_APPL_WINDOW", IDM_APPL_WINDOW, m_hwndApp,                     TRUE,  TRUE );
	m_WinManager.AddWindow(L"IDM_STATUS_BAR",  IDM_STATUS_BAR,  m_StatusBar.GetWindowHandle(), FALSE, FALSE);

	if (m_bUseHistorySystem)
	{
		m_pHistorySystem = HistorySystem::CreateHistorySystem();  // deleted in Stop function
		m_HistWindow    .Start(m_hwndApp, m_pHistorySystem, m_pWorkThreadInterface);
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
		m_HistInfoWindow.Stop();
		m_HistWindow    .Stop();
		delete m_pHistorySystem;   
		m_pHistorySystem = nullptr;
	}
}

void BaseAppWindow::adjustChildWindows()
{
	HWND hwndApp = GetParent(m_pModelWindow->GetWindowHandle());

	PixelRectSize pntAppClientSize(Util::GetClRectSize(hwndApp));
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

LRESULT BaseAppWindow::UserProc
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
		static COLORREF const CLR_GREY = RGB(128, 128, 128);
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
	int const wmId = LOWORD(wParam);

	switch (wmId)
	{
	case IDM_ABOUT:
		ShowAboutBox(GetWindowHandle());
		break;

	case IDM_EXIT:
		PostMessage(WM_CLOSE, 0, 0);
		break;

	case IDM_MAIN_WINDOW:
		::SendMessage(m_WinManager.GetHWND(wmId), WM_COMMAND, IDM_WINDOW_ON, 0);
		break;

	case IDM_HIST_INFO:
	case IDM_HIST_WINDOW:
		assert(m_bUseHistorySystem);
		::SendMessage(m_WinManager.GetHWND(wmId), WM_COMMAND, IDM_WINDOW_ON, 0);
		break;

	case IDM_FORWARD:
		m_pWorkThreadInterface->PostGenerationStep();
		break;

	case IDM_BACKWARDS:
		assert(m_bUseHistorySystem);
		m_pWorkThreadInterface->PostPrevGeneration();
		break;

	case IDM_EDIT_UNDO:
		assert(m_bUseHistorySystem);
		m_pWorkThreadInterface->PostUndo();
		break;

	case IDM_EDIT_REDO:
		assert(m_bUseHistorySystem);
		m_pWorkThreadInterface->PostRedo();
		break;

	case IDM_RUN:
		m_pWorkThreadInterface->PostRunGenerations(true);
		break;

	case IDM_STOP:
		m_pWorkThreadInterface->PostStopComputation();
		break;

	case IDM_HIST_BUFFER_FULL:
		assert(m_bUseHistorySystem);
		std::wcout << L"History buffer is full" << std::endl;
		(void)MessageBeep(MB_ICONWARNING);
		ProcessFrameworkCommand(IDM_STOP);
		break;

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}
