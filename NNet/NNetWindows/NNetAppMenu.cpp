// NNetAppMenu.cpp
//
// NNetWindows

module;

#include <Windows.h>

module NNetWin32:NNetAppMenu;

import std;
import Debug;
import Win32_Util_Resource;
import SoundInterface;
import Bitmap;
import IconUndo;
import IconRedo;
import WinManager;
import BaseWindow;
import Win32_Util;
import OnOffPair;
import IoUtil;
import NNetModel;
import NNetPreferences;
import :Compute;
import Resource;

using std::make_unique;

NNetAppMenu::NNetAppMenu()
  : m_upOnOffArrows        (make_unique<OnOffPair>(IDD_ARROWS          )),
    m_upOnOffSound         (make_unique<OnOffPair>(IDD_SOUND           )),
    m_upOnOffAutoOpen      (make_unique<OnOffPair>(IDD_AUTO_OPEN       )),
    m_upOnOffSensorPoints  (make_unique<OnOffPair>(IDD_SENSOR_PNTS     )),
    m_upOnOffPerfMonMode   (make_unique<OnOffPair>(IDD_PERF_MON_MODE   )),
    m_upOnOffColorMenu     (make_unique<OnOffPair>(IDD_COLOR_MENU      )),
    m_upOnOffScanArea      (make_unique<OnOffPair>(IDD_SCAN_AREA       )),
    m_upOnOffAskNotUndoable(make_unique<OnOffPair>(IDD_ASK_NOT_UNDOABLE))
{ }

NNetAppMenu::~NNetAppMenu() = default;

void NNetAppMenu::Start
(
	HWND         const   hwndApp,
	Compute      const & compute,
	CommandStack const & commandStack
) 
{
    HINSTANCE const hInstance = GetModuleHandle(nullptr);

	m_hwndApp       = hwndApp;
	m_pCompute      = & compute;
	m_pCommandStack = & commandStack;

    m_hMenu = CreateMenu();

    ::SetNotifyByPos(m_hMenu);

	HBITMAP hBitmapUndo { CreateBitmapFromIconData(*IconUndo_data.data()) };
	HBITMAP hBitmapRedo { CreateBitmapFromIconData(*IconRedo_data.data()) };

    HMENU hMenuFile = ::PopupMenu(m_hMenu, L"&File");
    {
        ::AddMenu(hMenuFile, MF_STRING, IDM_NEW_MODEL,     L"&New model" );
        ::AddMenu(hMenuFile, MF_STRING, IDM_OPEN_MODEL,    L"&Open model");
        ::AddMenu(hMenuFile, MF_STRING, IDM_ADD_MODULE,    L"&Add module");
        ::AddMenu(hMenuFile, MF_STRING, IDM_SAVE_MODEL,    L"&Save model");
        ::AddMenu(hMenuFile, MF_STRING, IDM_SAVE_MODEL_AS, L"Save model &as");
        ::AddMenu(hMenuFile, MF_STRING, IDM_SAVE_SCAN,     L"Save scan");
        ::AddMenu(hMenuFile, MF_STRING, IDM_RELOAD_MODEL,  L"&Reload model");
        ::AddMenu(hMenuFile, MF_STRING, IDM_SCRIPT_DIALOG, L"&Run script");
        ::AddMenu(hMenuFile, MF_STRING, IDM_DUMP,          L"&Dump");
        ::AddMenu(hMenuFile, MF_STRING, IDM_EXIT,          L"&Exit");
    }

    ::AddMenu(m_hMenu, MF_BITMAP, IDM_UNDO, (LPCTSTR)hBitmapUndo);
    ::AddMenu(m_hMenu, MF_BITMAP, IDM_REDO, (LPCTSTR)hBitmapRedo);

    HMENU hMenuAction = ::PopupMenu(m_hMenu, L"Action");
    {
        ::AddMenu(hMenuAction, MF_STRING, IDM_RESET_DYNAMIC_DATA, L"Reset dynamic data");
        ::AddMenu(hMenuAction, MF_STRING, IDM_UNLOCK,             L"&Unlock model");
        ::AddMenu(hMenuAction, MF_STRING, IDM_FORWARD,            L"&Proceed single step");
        ::AddMenu(hMenuAction, MF_STRING, IDM_CENTER_MODEL,       L"&Center model");
    }

    HMENU hMenuView = ::PopupMenu(m_hMenu, L"&View");
    {
        HMENU hMenuWindows = ::PopupMenu(hMenuView, L"&Windows");
        {
            ::AddMenu(hMenuWindows, MF_STRING, IDM_MINI_WINDOW,    L"Show &mini window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_MONITOR_WINDOW, L"Show m&onitor window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_SIG_DESIGNER,   L"Show &signal designer window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_LUT_DESIGNER,   L"Show color lookup table window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_DESC_WINDOW,    L"Show &description window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_CRSR_WINDOW,    L"Show &cursor window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_PARAM_WINDOW,   L"Show &parameter window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_PERF_WINDOW,    L"Show &performance window");
            ::AddMenu(hMenuWindows, MF_STRING, IDM_VIEWER_WINDOW,  L"Show &scan viewer window");
        }
        HMENU hMenuInputCables = ::PopupMenu(hMenuView, L"&Input cables");
        {
            ::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_ALL,    L"Show all");
            ::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_NONSTD, L"Show non standard");
            ::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_ACTIVE, L"Show active");
            ::AddMenu(hMenuInputCables, MF_STRING, IDM_INPUT_CABLES_NONE,   L"Show none");
        }
        m_upOnOffScanArea    ->AppendOnOffMenu(hMenuView, L"&Scan area");
        m_upOnOffArrows      ->AppendOnOffMenu(hMenuView, L"&Arrows");
        m_upOnOffSensorPoints->AppendOnOffMenu(hMenuView, L"Sensor &points");
        m_upOnOffColorMenu   ->AppendOnOffMenu(hMenuView, L"&Color menu");
    }
    HMENU hMenuOptions = ::PopupMenu(m_hMenu, L"&Options");
    {
        m_upOnOffSound         ->AppendOnOffMenu(hMenuOptions, L"&Sound");
        m_upOnOffAutoOpen      ->AppendOnOffMenu(hMenuOptions, L"Auto&Open");
        m_upOnOffAskNotUndoable->AppendOnOffMenu(hMenuOptions, L"Ask not undoable");
        m_upOnOffPerfMonMode   ->AppendOnOffMenu(hMenuOptions, L"&PerformanceMonitor");
    }
    HMENU hMenuHelp = ::PopupMenu(m_hMenu, L"&Help");
    {
        ::AddMenu(hMenuHelp, MF_STRING, IDM_DOCU,  L"&Documentation");
        ::AddMenu(hMenuHelp, MF_STRING, IDM_ABOUT, L"&Info...");
    }

    bool bRes = SetMenu(m_hwndApp, m_hMenu);
    Assert(bRes);
}

void NNetAppMenu::SetModelInterface(NNetModelReaderInterface const * const pNMRI)
{
	m_pNMRI = pNMRI;
}

void NNetAppMenu::Notify(bool const bImmediately)
{
    ::Enable(m_hMenu, IDM_FORWARD,            ! m_pCompute->IsRunning());
    ::Enable(m_hMenu, IDM_RESET_DYNAMIC_DATA, ! m_pCompute->IsRunning());

    ::Enable(m_hMenu, IDM_DESC_WINDOW,    ! WinManager::IsVisible(RootWinId(IDM_DESC_WINDOW   )));
    ::Enable(m_hMenu, IDM_CRSR_WINDOW,    ! WinManager::IsVisible(RootWinId(IDM_CRSR_WINDOW   )));
    ::Enable(m_hMenu, IDM_MINI_WINDOW,    ! WinManager::IsVisible(RootWinId(IDM_MINI_WINDOW   )));
    ::Enable(m_hMenu, IDM_MONITOR_WINDOW, ! WinManager::IsVisible(RootWinId(IDM_MONITOR_WINDOW)));
    ::Enable(m_hMenu, IDM_PARAM_WINDOW,   ! WinManager::IsVisible(RootWinId(IDM_PARAM_WINDOW  )));
    ::Enable(m_hMenu, IDM_PERF_WINDOW,    ! WinManager::IsVisible(RootWinId(IDM_PERF_WINDOW   )));
    ::Enable(m_hMenu, IDM_SIG_DESIGNER,   ! WinManager::IsVisible(RootWinId(IDM_SIG_DESIGNER  )));
    ::Enable(m_hMenu, IDM_LUT_DESIGNER,   ! WinManager::IsVisible(RootWinId(IDM_LUT_DESIGNER  )));
    ::Enable(m_hMenu, IDM_VIEWER_WINDOW,  ! WinManager::IsVisible(RootWinId(IDM_VIEWER_WINDOW )));
    ::Enable(m_hMenu, IDM_SAVE_SCAN, m_pNMRI->ModelLocked());

    m_upOnOffArrows        ->EnableOnOff(m_hMenu, NNetPreferences::m_bArrows.Get());
    m_upOnOffSensorPoints  ->EnableOnOff(m_hMenu, NNetPreferences::m_bSensorPoints.Get());
    m_upOnOffScanArea      ->EnableOnOff(m_hMenu, NNetPreferences::m_bScanArea.Get());
    m_upOnOffAskNotUndoable->EnableOnOff(m_hMenu, NNetPreferences::m_bAskNotUndoable.Get());
    m_upOnOffSound         ->EnableOnOff(m_hMenu, Preferences::m_bSound.Get());
    m_upOnOffAutoOpen      ->EnableOnOff(m_hMenu, Preferences::m_bAutoOpen.Get());
    m_upOnOffColorMenu     ->EnableOnOff(m_hMenu, Preferences::m_bColorMenu.Get());
    m_upOnOffPerfMonMode   ->EnableOnOff(m_hMenu, BaseWindow ::m_bPerfMonMode.Get());

    DrawMenuBar(m_hwndApp);
}

void NNetAppMenu::AdjustUndoRedo()
{
    ::Enable(m_hMenu, IDM_UNDO, ! m_pCommandStack->UndoStackEmpty());
    ::Enable(m_hMenu, IDM_REDO, ! m_pCommandStack->RedoStackEmpty());
    DrawMenuBar(m_hwndApp);
}
