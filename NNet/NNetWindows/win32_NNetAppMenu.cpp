// Win32_NNetAppMenu.cpp
//
// NNetWindows

module;

#include <Windows.h>
#include "Resource.h"
#include "CommandStack.h"
#include "AutoOpen.h"
#include "SignalGenerator.h"
#include "win32_util_resource.h"

module NNetAppMenu;

import SoundInterface;
import ComputeThread;
import SignalDesigner;
import WinManager;
import Preferences;
import BaseWindow;
import Win32_Util;

using std::make_unique;

NNetAppMenu::~NNetAppMenu() = default;

class NNetAppMenu::OnOffPair
{
public:
    OnOffPair
    (
        NNetAppMenu      * pMenu,
        unsigned int const iOn, 
        unsigned int const iOff 
    )
      :	m_pAppMenu(pMenu),
        m_iOn     (iOn),
        m_iOff    (iOff)
    { }

    void appendOnOffMenu(HMENU hMenu, LPCTSTR const title) const
    {
        HMENU hMenuPopup = Util::PopupMenu(hMenu, title);
        Util::AddMenu(hMenuPopup, MF_STRING, m_iOn,  L"o&n");
        Util::AddMenu(hMenuPopup, MF_STRING, m_iOff, L"o&ff");
    }

    void enableOnOff(bool const bCrit)
    {
        m_pAppMenu->enable(m_iOn, ! bCrit);
        m_pAppMenu->enable(m_iOff,  bCrit);
    }

private:
    NNetAppMenu * m_pAppMenu;
    unsigned int  m_iOn;
    unsigned int  m_iOff;
};

NNetAppMenu::NNetAppMenu()
  : m_upOnOffArrows      (make_unique<OnOffPair>(this, IDD_ARROWS_ON,        IDD_ARROWS_OFF       )),
    m_upOnOffSound       (make_unique<OnOffPair>(this, IDD_SOUND_ON,         IDD_SOUND_OFF        )),
    m_upOnOffAutoOpen    (make_unique<OnOffPair>(this, IDD_AUTO_OPEN_ON,     IDD_AUTO_OPEN_OFF    )),
    m_upOnOffSensorPoints(make_unique<OnOffPair>(this, IDD_SENSOR_PNTS_ON,   IDD_SENSOR_PNTS_OFF  )),
    m_upOnOffPerfMonMode (make_unique<OnOffPair>(this, IDD_PERF_MON_MODE_ON, IDD_PERF_MON_MODE_OFF))
{ }

void NNetAppMenu::Start
(
	HWND          const   hwndApp,
	ComputeThread const & computeThread,
	WinManager    const & winManager,
	CommandStack  const & commandStack,
	Sound         const & sound,
    Preferences   const & preferences
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
        Util::AddMenu(hMenuFile, MF_STRING, IDM_ADD_MODEL,     L"&Add module");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_SAVE_MODEL,    L"&Save model");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_SAVE_MODEL_AS, L"Save model &as");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_SCRIPT_DIALOG, L"&Run script");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_DUMP,          L"&Dump");
        Util::AddMenu(hMenuFile, MF_STRING, IDM_EXIT,          L"&Exit");
    }

    HMENU hMenuEdit = Util::PopupMenu(m_hMenu, L"&Edit");
    {
        HMENU hMenuSelection = Util::PopupMenu(hMenuEdit, L"&Selection");
        {
            Util::AddMenu(hMenuSelection, MF_STRING, IDM_SELECT_ALL,   L"&Select all");
            Util::AddMenu(hMenuSelection, MF_STRING, IDM_DESELECT_ALL, L"&Deselect all");
        }
    }

    Util::AddMenu(m_hMenu, MF_BITMAP, IDM_UNDO, (LPCTSTR)hBitmapUndo);
    Util::AddMenu(m_hMenu, MF_BITMAP, IDM_REDO, (LPCTSTR)hBitmapRedo);

    m_hMenuSigGen = Util::PopupMenu(m_hMenu, L"&Signal generators");
    addSigGenMenuEntries();

    HMENU hMenuAction = Util::PopupMenu(m_hMenu, L"&Action");
    {
        Util::AddMenu(hMenuAction, MF_STRING, IDM_RESET_DYNAMIC_DATA, L"Reset dynamic data");
        Util::AddMenu(hMenuAction, MF_STRING, IDM_FORWARD,            L"&Proceed single step");
        Util::AddMenu(hMenuAction, MF_STRING, IDM_RUN_STOP,           L"&Run/Stop");
        HMENU hMenuAnalyze = Util::PopupMenu(hMenuAction, L"&Analyze");
        {
            Util::AddMenu(hMenuAnalyze, MF_STRING, IDM_ANALYZE_LOOPS    , L"Find &loops");
            Util::AddMenu(hMenuAnalyze, MF_STRING, IDM_ANALYZE_ANOMALIES, L"Find &anomalies");
        }
        Util::AddMenu(hMenuAction, MF_STRING, IDM_CENTER_MODEL, L"&Center model");
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
        m_upOnOffArrows      ->appendOnOffMenu(hMenuView, L"&Arrows");
        m_upOnOffSensorPoints->appendOnOffMenu(hMenuView, L"&SensorPoints");
    }
    HMENU hMenuOptions = Util::PopupMenu(m_hMenu, L"&Options");
    {
        m_upOnOffSound      ->appendOnOffMenu(hMenuOptions, L"&Sound");
        m_upOnOffAutoOpen   ->appendOnOffMenu(hMenuOptions, L"Auto&Open");
        m_upOnOffPerfMonMode->appendOnOffMenu(hMenuOptions, L"&PerformanceMonitor");
    }
    HMENU hMenuHelp = Util::PopupMenu(m_hMenu, L"&Help");
    {
        Util::AddMenu(hMenuHelp, MF_STRING, IDM_ABOUT, L"&Info...");
    }

    bool bRes = SetMenu(m_hwndApp, m_hMenu);
    assert(bRes);
}

void NNetAppMenu::addSigGenMenuEntries()
{
    if (m_pNMRI)
    {
        m_pNMRI->GetSigGenList().Apply2AllC
        (
            [this](SignalGenerator const * pSigGen)
            {
                Util::AddMenu
                (
                    m_hMenuSigGen, 
                    MF_STRING, 
                    IDD_SELECT_SIGNAL_GENERATOR, 
                    pSigGen->GetName().c_str()
                );
            }
        );
    }
    Util::AddMenu(m_hMenuSigGen, MF_STRING, IDD_NEW_SIGNAL_GENERATOR, L"Create &new");
}

void NNetAppMenu::delSigGenMenuEntries()
{
    while (DeleteMenu(m_hMenuSigGen, 0, MF_BYPOSITION)) { };
}

void NNetAppMenu::enable(unsigned int const id, bool const bCrit)
{
    EnableMenuItem(m_hMenu, id, bCrit ? MF_ENABLED : MF_GRAYED);
}

void NNetAppMenu::Notify(bool const bImmediately)
{
    enable(IDM_FORWARD, ! m_pComputeThread->IsRunning());
    enable(IDM_RESET_DYNAMIC_DATA,   ! m_pComputeThread->IsRunning());

    enable(IDM_DESC_WINDOW,    ! m_pWinManager->IsVisible(IDM_DESC_WINDOW   ));
    enable(IDM_CRSR_WINDOW,    ! m_pWinManager->IsVisible(IDM_CRSR_WINDOW   ));
    enable(IDM_MINI_WINDOW,    ! m_pWinManager->IsVisible(IDM_MINI_WINDOW   ));
    enable(IDM_MONITOR_WINDOW, ! m_pWinManager->IsVisible(IDM_MONITOR_WINDOW));
    enable(IDM_PARAM_WINDOW,   ! m_pWinManager->IsVisible(IDM_PARAM_WINDOW  ));
    enable(IDM_PERF_WINDOW,    ! m_pWinManager->IsVisible(IDM_PERF_WINDOW   ));
    enable(IDM_SIG_DESIGNER,   ! m_pWinManager->IsVisible(IDM_SIG_DESIGNER  ));

    m_upOnOffArrows      ->enableOnOff(m_pPreferences->ArrowsVisible());
    m_upOnOffSound       ->enableOnOff(m_pSound->IsOn());
    m_upOnOffAutoOpen    ->enableOnOff(AutoOpen::IsOn());
    m_upOnOffSensorPoints->enableOnOff(m_pPreferences->SensorPointsVisible());
    m_upOnOffPerfMonMode ->enableOnOff(BaseWindow::PerfMonMode());

    delSigGenMenuEntries();
    addSigGenMenuEntries();

    DrawMenuBar(m_hwndApp);
}

void NNetAppMenu::AdjustUndoRedo()
{
    enable(IDM_UNDO, ! m_pCommandStack->UndoStackEmpty());
    enable(IDM_REDO, ! m_pCommandStack->RedoStackEmpty());
    DrawMenuBar(m_hwndApp);
}
