// NNetAppMenu.cpp
//
// NNetWindows

module;

#include <cassert>
#include <memory>
#include <Windows.h>
#include "Resource.h"

module NNetWin32:NNetAppMenu;

import Win32_Util_Resource;
import SoundInterface;
import WinManager;
import BaseWindow;
import Win32_Util;
import OnOffPair;
import Commands;
import AutoOpen;
import NNetModel;
import :ComputeThread;
import :NNetPreferences;

using std::make_unique;

NNetAppMenu::NNetAppMenu()
  : m_upOnOffArrows      (make_unique<OnOffPair>(IDD_ARROWS_ON,        IDD_ARROWS_OFF       )),
    m_upOnOffSound       (make_unique<OnOffPair>(IDD_SOUND_ON,         IDD_SOUND_OFF        )),
    m_upOnOffAutoOpen    (make_unique<OnOffPair>(IDD_AUTO_OPEN_ON,     IDD_AUTO_OPEN_OFF    )),
    m_upOnOffSensorPoints(make_unique<OnOffPair>(IDD_SENSOR_PNTS_ON,   IDD_SENSOR_PNTS_OFF  )),
    m_upOnOffPerfMonMode (make_unique<OnOffPair>(IDD_PERF_MON_MODE_ON, IDD_PERF_MON_MODE_OFF)),
    m_upOnOffColorMenu   (make_unique<OnOffPair>(IDD_COLOR_MENU_ON,    IDD_COLOR_MENU_OFF   ))
{ }

NNetAppMenu::~NNetAppMenu() = default;

void NNetAppMenu::Start
(
	HWND            const   hwndApp,
	ComputeThread   const & computeThread,
	WinManager      const & winManager,
	CommandStack    const & commandStack,
	Sound           const & sound,
    NNetPreferences const & preferences
) 
{
    HINSTANCE const hInstance = GetModuleHandle(nullptr);

	m_hwndApp        = hwndApp;
	m_pComputeThread = & computeThread;
	m_pWinManager    = & winManager;
	m_pCommandStack  = & commandStack;
	m_pSound         = & sound;
    m_pPreferences   = & preferences;

    SendMessage(m_hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NNETSIMU)));
    SendMessage(m_hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL   )));

    m_hMenu = CreateMenu();

    Util::SetNotifyByPos(m_hMenu);

	HBITMAP hBitmapUndo { LoadBitmap(hInstance, L"UNDO_BITMAP") };
	HBITMAP hBitmapRedo { LoadBitmap(hInstance, L"REDO_BITMAP") };

    HMENU hMenuFile = Util::PopupMenu(m_hMenu, L"&File");
    {
        Util::AddMenu(hMenuFile, MF_STRING, IDM_NEW_MODEL,     L"&New model" );
        Util::AddMenu(hMenuFile, MF_STRING, IDM_OPEN_MODEL,    L"&Open model");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_ADD_MODULE,    L"&Add module");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_SAVE_MODEL,    L"&Save model");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_SAVE_MODEL_AS, L"Save model &as");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_RELOAD_MODEL,  L"&Reload model");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_SCRIPT_DIALOG, L"&Run script");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_DUMP,          L"&Dump");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_EXIT,          L"&Exit");
    }

    Util::AddMenu(m_hMenu, MF_BITMAP, IDM_UNDO, (LPCTSTR)hBitmapUndo);
    Util::AddMenu(m_hMenu, MF_BITMAP, IDM_REDO, (LPCTSTR)hBitmapRedo);

    HMENU hMenuAction = Util::PopupMenu(m_hMenu, L"&Action");
    {
        Util::AddMenu(hMenuAction, MF_STRING, IDM_RESET_DYNAMIC_DATA, L"Reset dynamic data");
        Util::AddMenu(hMenuAction, MF_STRING, IDM_FORWARD,            L"&Proceed single step");
        Util::AddMenu(hMenuAction, MF_STRING, IDM_RUN_STOP,           L"&Run/Stop");
        Util::AddMenu(hMenuAction, MF_STRING, IDM_CENTER_MODEL,       L"&Center model");
    }

    HMENU hMenuView = Util::PopupMenu(m_hMenu, L"&View");
    {
        HMENU hMenuWindows = Util::PopupMenu(hMenuView, L"&Windows");
        {
            Util::AddMenu(hMenuWindows, MF_STRING, IDM_MINI_WINDOW,    L"Show &mini window");
            Util::AddMenu(hMenuWindows, MF_STRING, IDM_MONITOR_WINDOW, L"Show m&onitor window");
            Util::AddMenu(hMenuWindows, MF_STRING, IDM_SIG_DESIGNER,   L"Show &signal designer window");
            Util::AddMenu(hMenuWindows, MF_STRING, IDM_DESC_WINDOW,    L"Show &description window");
            Util::AddMenu(hMenuWindows, MF_STRING, IDM_CRSR_WINDOW,    L"Show &cursor window");
            Util::AddMenu(hMenuWindows, MF_STRING, IDM_PARAM_WINDOW,   L"Show &parameter window");
            Util::AddMenu(hMenuWindows, MF_STRING, IDM_PERF_WINDOW,    L"Show &performance window");
        }
        HMENU hMenuInputCables = Util::PopupMenu(hMenuView, L"&Input cables");
        {
            Util::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_ALL,    L"Show all");
            Util::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_NONSTD, L"Show non standard");
            Util::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_ACTIVE, L"Show active");
            Util::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_NONE,   L"Show none");
        }
        m_upOnOffArrows      ->AppendOnOffMenu(hMenuView, L"&Arrows");
        m_upOnOffSensorPoints->AppendOnOffMenu(hMenuView, L"&Sensor points");
        m_upOnOffColorMenu   ->AppendOnOffMenu(hMenuView, L"&Color menu");
    }
    HMENU hMenuOptions = Util::PopupMenu(m_hMenu, L"&Options");
    {
        m_upOnOffSound      ->AppendOnOffMenu(hMenuOptions, L"&Sound");
        m_upOnOffAutoOpen   ->AppendOnOffMenu(hMenuOptions, L"Auto&Open");
        m_upOnOffPerfMonMode->AppendOnOffMenu(hMenuOptions, L"&PerformanceMonitor");
    }
    HMENU hMenuHelp = Util::PopupMenu(m_hMenu, L"&Help");
    {
        Util::AddMenu(hMenuHelp, MF_STRING, IDM_DOCU,  L"&Documentation");
        Util::AddMenu(hMenuHelp, MF_STRING, IDM_ABOUT, L"&Info...");
    }

    bool bRes = SetMenu(m_hwndApp, m_hMenu);
    assert(bRes);
}

void NNetAppMenu::Notify(bool const bImmediately)
{
    Util::Enable(m_hMenu, IDM_FORWARD,            ! m_pComputeThread->IsRunning());
    Util::Enable(m_hMenu, IDM_RESET_DYNAMIC_DATA, ! m_pComputeThread->IsRunning());

    Util::Enable(m_hMenu, IDM_DESC_WINDOW,    ! m_pWinManager->IsVisible(RootWinId(IDM_DESC_WINDOW   )));
    Util::Enable(m_hMenu, IDM_CRSR_WINDOW,    ! m_pWinManager->IsVisible(RootWinId(IDM_CRSR_WINDOW   )));
    Util::Enable(m_hMenu, IDM_MINI_WINDOW,    ! m_pWinManager->IsVisible(RootWinId(IDM_MINI_WINDOW   )));
    Util::Enable(m_hMenu, IDM_MONITOR_WINDOW, ! m_pWinManager->IsVisible(RootWinId(IDM_MONITOR_WINDOW)));
    Util::Enable(m_hMenu, IDM_PARAM_WINDOW,   ! m_pWinManager->IsVisible(RootWinId(IDM_PARAM_WINDOW  )));
    Util::Enable(m_hMenu, IDM_PERF_WINDOW,    ! m_pWinManager->IsVisible(RootWinId(IDM_PERF_WINDOW   )));
    Util::Enable(m_hMenu, IDM_SIG_DESIGNER,   ! m_pWinManager->IsVisible(RootWinId(IDM_SIG_DESIGNER  )));

    m_upOnOffArrows      ->EnableOnOff(m_hMenu, m_pPreferences->ArrowsVisible());
    m_upOnOffSound       ->EnableOnOff(m_hMenu, m_pSound->IsOn());
    m_upOnOffAutoOpen    ->EnableOnOff(m_hMenu, AutoOpen::IsOn());
    m_upOnOffSensorPoints->EnableOnOff(m_hMenu, m_pPreferences->m_bSensorPoints.Get());
    m_upOnOffPerfMonMode ->EnableOnOff(m_hMenu, BaseWindow::m_bPerfMonMode.Get());
    m_upOnOffColorMenu   ->EnableOnOff(m_hMenu, m_pPreferences->ColorMenuVisible());

    DrawMenuBar(m_hwndApp);
}

void NNetAppMenu::AdjustUndoRedo()
{
    Util::Enable(m_hMenu, IDM_UNDO, ! m_pCommandStack->UndoStackEmpty());
    Util::Enable(m_hMenu, IDM_REDO, ! m_pCommandStack->RedoStackEmpty());
    DrawMenuBar(m_hwndApp);
}
