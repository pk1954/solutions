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
import CommandStack;
import NNetModel;
import Preferences;
import NNetPreferences;
import :ComputeThread;

using std::make_unique;

NNetAppMenu::NNetAppMenu()
  : m_upOnOffArrows      (make_unique<OnOffPair>(IDD_ARROWS       )),
    m_upOnOffSound       (make_unique<OnOffPair>(IDD_SOUND        )),
    m_upOnOffAutoOpen    (make_unique<OnOffPair>(IDD_AUTO_OPEN    )),
    m_upOnOffSensorPoints(make_unique<OnOffPair>(IDD_SENSOR_PNTS  )),
    m_upOnOffPerfMonMode (make_unique<OnOffPair>(IDD_PERF_MON_MODE)),
    m_upOnOffColorMenu   (make_unique<OnOffPair>(IDD_COLOR_MENU   )),
    m_upOnOffScanMode    (make_unique<OnOffPair>(IDD_SCAN_MODE    ))
{ }

NNetAppMenu::~NNetAppMenu() = default;

void NNetAppMenu::Start
(
	HWND            const   hwndApp,
	ComputeThread   const & computeThread,
	CommandStack    const & commandStack,
	Sound           const & sound
) 
{
    HINSTANCE const hInstance = GetModuleHandle(nullptr);

	m_hwndApp        = hwndApp;
	m_pComputeThread = & computeThread;
	m_pCommandStack  = & commandStack;
	m_pSound         = & sound;

    SendMessage(m_hwndApp, WM_SETICON, ICON_BIG,   (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NNETSIMU)));
    SendMessage(m_hwndApp, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL   )));

    m_hMenu = CreateMenu();

    ::SetNotifyByPos(m_hMenu);

	HBITMAP hBitmapUndo { LoadBitmap(hInstance, L"UNDO_BITMAP") };
	HBITMAP hBitmapRedo { LoadBitmap(hInstance, L"REDO_BITMAP") };

    HMENU hMenuFile = ::PopupMenu(m_hMenu, L"&File");
    {
        ::AddMenu(hMenuFile, MF_STRING, IDM_NEW_MODEL,     L"&New model" );
        ::AddMenu(hMenuFile, MF_STRING, IDM_OPEN_MODEL,    L"&Open model");
        ::AddMenu(hMenuFile, MF_STRING, IDM_ADD_MODULE,    L"&Add module");
        ::AddMenu(hMenuFile, MF_STRING, IDM_SAVE_MODEL,    L"&Save model");
        ::AddMenu(hMenuFile, MF_STRING, IDM_SAVE_MODEL_AS, L"Save model &as");
        ::AddMenu(hMenuFile, MF_STRING, IDM_RELOAD_MODEL,  L"&Reload model");
        ::AddMenu(hMenuFile, MF_STRING, IDM_SCRIPT_DIALOG, L"&Run script");
        ::AddMenu(hMenuFile, MF_STRING, IDM_DUMP,          L"&Dump");
        ::AddMenu(hMenuFile, MF_STRING, IDM_EXIT,          L"&Exit");
    }

    ::AddMenu(m_hMenu, MF_BITMAP, IDM_UNDO, (LPCTSTR)hBitmapUndo);
    ::AddMenu(m_hMenu, MF_BITMAP, IDM_REDO, (LPCTSTR)hBitmapRedo);

    HMENU hMenuAction = ::PopupMenu(m_hMenu, L"&Action");
    {
        ::AddMenu(hMenuAction, MF_STRING, IDM_RESET_DYNAMIC_DATA, L"Reset dynamic data");
        ::AddMenu(hMenuAction, MF_STRING, IDM_FORWARD,            L"&Proceed single step");
        ::AddMenu(hMenuAction, MF_STRING, IDM_RUN_STOP,           L"&Run/Stop");
        ::AddMenu(hMenuAction, MF_STRING, IDM_CENTER_MODEL,       L"&Center model");
    }

    HMENU hMenuView = ::PopupMenu(m_hMenu, L"&View");
    {
        HMENU hMenuWindows = ::PopupMenu(hMenuView, L"&Windows");
        {
            ::AddMenu(hMenuWindows, MF_STRING, IDM_MINI_WINDOW,    L"Show &mini window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_MONITOR_WINDOW, L"Show m&onitor window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_SIG_DESIGNER,   L"Show &signal designer window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_DESC_WINDOW,    L"Show &description window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_CRSR_WINDOW,    L"Show &cursor window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_PARAM_WINDOW,   L"Show &parameter window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_PERF_WINDOW,    L"Show &performance window");
        }
        HMENU hMenuInputCables = ::PopupMenu(hMenuView, L"&Input cables");
        {
            ::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_ALL,    L"Show all");
            ::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_NONSTD, L"Show non standard");
            ::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_ACTIVE, L"Show active");
            ::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_NONE,   L"Show none");
        }
        m_upOnOffScanMode    ->AppendOnOffMenu(hMenuView, L"&Scan mode");
        m_upOnOffArrows      ->AppendOnOffMenu(hMenuView, L"&Arrows");
        m_upOnOffSensorPoints->AppendOnOffMenu(hMenuView, L"Sensor &points");
        m_upOnOffColorMenu   ->AppendOnOffMenu(hMenuView, L"&Color menu");
    }
    HMENU hMenuOptions = ::PopupMenu(m_hMenu, L"&Options");
    {
        m_upOnOffSound      ->AppendOnOffMenu(hMenuOptions, L"&Sound");
        m_upOnOffAutoOpen   ->AppendOnOffMenu(hMenuOptions, L"Auto&Open");
        m_upOnOffPerfMonMode->AppendOnOffMenu(hMenuOptions, L"&PerformanceMonitor");
    }
    HMENU hMenuHelp = ::PopupMenu(m_hMenu, L"&Help");
    {
        ::AddMenu(hMenuHelp, MF_STRING, IDM_DOCU,  L"&Documentation");
        ::AddMenu(hMenuHelp, MF_STRING, IDM_ABOUT, L"&Info...");
    }

    bool bRes = SetMenu(m_hwndApp, m_hMenu);
    assert(bRes);
}

void NNetAppMenu::Notify(bool const bImmediately)
{
    ::Enable(m_hMenu, IDM_FORWARD,            ! m_pComputeThread->IsRunning());
    ::Enable(m_hMenu, IDM_RESET_DYNAMIC_DATA, ! m_pComputeThread->IsRunning());

    ::Enable(m_hMenu, IDM_DESC_WINDOW,    ! WinManager::IsVisible(RootWinId(IDM_DESC_WINDOW   )));
    ::Enable(m_hMenu, IDM_CRSR_WINDOW,    ! WinManager::IsVisible(RootWinId(IDM_CRSR_WINDOW   )));
    ::Enable(m_hMenu, IDM_MINI_WINDOW,    ! WinManager::IsVisible(RootWinId(IDM_MINI_WINDOW   )));
    ::Enable(m_hMenu, IDM_MONITOR_WINDOW, ! WinManager::IsVisible(RootWinId(IDM_MONITOR_WINDOW)));
    ::Enable(m_hMenu, IDM_PARAM_WINDOW,   ! WinManager::IsVisible(RootWinId(IDM_PARAM_WINDOW  )));
    ::Enable(m_hMenu, IDM_PERF_WINDOW,    ! WinManager::IsVisible(RootWinId(IDM_PERF_WINDOW   )));
    ::Enable(m_hMenu, IDM_SIG_DESIGNER,   ! WinManager::IsVisible(RootWinId(IDM_SIG_DESIGNER  )));

    m_upOnOffArrows      ->EnableOnOff(m_hMenu, NNetPreferences::m_bArrows.Get());
    m_upOnOffSensorPoints->EnableOnOff(m_hMenu, NNetPreferences::m_bSensorPoints.Get());
    m_upOnOffSound       ->EnableOnOff(m_hMenu, Preferences::m_bSound.Get());
    m_upOnOffAutoOpen    ->EnableOnOff(m_hMenu, Preferences::m_bAutoOpen.Get());
    m_upOnOffColorMenu   ->EnableOnOff(m_hMenu, Preferences::m_bColorMenu.Get());
    m_upOnOffPerfMonMode ->EnableOnOff(m_hMenu, BaseWindow::m_bPerfMonMode.Get());

    DrawMenuBar(m_hwndApp);
}

void NNetAppMenu::AdjustUndoRedo()
{
    ::Enable(m_hMenu, IDM_UNDO, ! m_pCommandStack->UndoStackEmpty());
    ::Enable(m_hMenu, IDM_REDO, ! m_pCommandStack->RedoStackEmpty());
    DrawMenuBar(m_hwndApp);
}
