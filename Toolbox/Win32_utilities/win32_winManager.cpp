// Win32_WinManager.cpp
//
// Win32_utilities

module;

#include <Windows.h>

module WinManager;

import std;
import IoConstants;
import RunTime;
import Commands;
import IoUtil;
import IoConstants;
import WinManager;
import Win32_Util_Resource;
import Win32_Util;
import Win32_PIXEL;
import RootWindow;
import BaseWindow;
import BaseDialog;

using std::endl;
using std::pair;
using std::wcout;
using std::wostream;
using std::wofstream;
using std::to_wstring;
using std::wstring;
using std::make_unique;
using std::map;
using std::out_of_range;

using ::operator==;
using ::operator!=;
using ::operator<<; 

class WrapMoveWindow : public Wrapper
{
public:
    WrapMoveWindow(WinManager::WIN_MAP const &map)
      : Wrapper(NAME),
        m_map(map)
    {}

    void operator() (Script & script) const final
    {
	    RootWinId id(script.ScrReadInt());
		if (id.GetValue() > 0)
		{
			PixelRect pixRect = ScrReadPixelRect(script);
			if (WinManager::IsMoveable(id))
			{
				HWND const hwnd = WinManager::GetHWND(id);
                if (WinManager::IsSizeable(id))
                {
                    bool bRes = ::MoveWindowAbsolute(hwnd, pixRect, true);
                    //assert(bRes);
                }
                else
                {
     				bool bRes = ::MoveWindowAbsolute(hwnd, pixRect.GetStartPoint(), true); 
                	DWORD dwErr = GetLastError();
                	//assert(bRes);
                }
            }
		}
    }

    void Write(wostream& out) const final
    {
        for (const auto& [key, value] : m_map)
        {
            if ((value.m_bTrackPosition) && (value.m_hwnd != nullptr))
                out << NAME << SPACE
                    << value.m_wstr
                    << ::GetWindowLeftPos(value.m_hwnd)
                    << ::GetWindowTop    (value.m_hwnd)
                    << ::GetWindowWidth  (value.m_hwnd)
                    << ::GetWindowHeight (value.m_hwnd)
                    << endl;
        }
    }

private:
    inline static const wstring NAME { L"MoveWindow" };

    WinManager::WIN_MAP const& m_map;
};

class WrapShowWindow : public Wrapper
{
public:
    WrapShowWindow(WinManager::WIN_MAP const& map)
        : Wrapper(NAME),
        m_map(map)
    {}

    void operator() (Script & script) const final
    {
	    RootWinId const id      (script.ScrReadInt());
		INT       const iCmdShow(script.ScrReadInt());  // WM_HIDE, WM_SHOW, ...
		if (id.GetValue() > 0)
			ShowWindow(WinManager::GetHWND(id), iCmdShow);
    }

    void Write(wostream& out) const final
    {
        for (const auto& [key, value] : m_map)
        {
            if ((value.m_bTrackPosition) && (value.m_hwnd != nullptr))
                out << NAME << SPACE
                    << value.m_wstr << SPACE
                    << (
                          IsWindowVisible(value.m_hwnd)
                          ? (IsZoomed(value.m_hwnd) ? L"SW_MAXIMIZE" : L"SW_SHOWNORMAL")
                          : L"SW_HIDE"
                       )
                    << endl;
        }
    }

private:
    inline static const wstring NAME { L"ShowWindow" };

    WinManager::WIN_MAP const& m_map;
};

//
// read monitor configuration from script
//

struct CHECK_MON_STRUCT  // communication between WrapMonitorInfos and CheckMonitorInfo 
{
    int                                 m_iMonCounter    { 0 };
    int                                 m_iMonFromScript { 0 };
    Script                            * m_pScript        { nullptr } ;
    bool                                m_bCheckResult   { true };
    ScriptErrorHandler::ScriptException m_errorInfo      { } ;
};

static MONITORINFO ScrReadMonitorInfo(Script & script)
{
    MONITORINFO monInfo;

    monInfo.cbSize    = sizeof(MONITORINFO);
    monInfo.rcMonitor = ::ScrReadRECT(script);
    monInfo.rcWork    = ::ScrReadRECT(script);
    monInfo.dwFlags   = script.ScrReadUlong();

    return monInfo;
}

static bool operator != (MONITORINFO const & a, MONITORINFO const & b) 
{ 
    return (a.rcMonitor != b.rcMonitor) || (a.rcWork != b.rcWork) || (a.dwFlags != b.dwFlags);
};

static BOOL CALLBACK CheckMonitorInfo(HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData)
{
    auto * const pMonStruct { (CHECK_MON_STRUCT *)dwData };
    bool         bRes       { true };
    try 
    {  
        Script * pScript = pMonStruct->m_pScript;

        pMonStruct->m_iMonFromScript = pScript->ScrReadInt();

        if (pMonStruct->m_iMonFromScript == 0)
        {                                        // m_script describes less monitors than current configuration  
            pMonStruct->m_bCheckResult = false;  // this is not the right configuration
            bRes = false;                        // stop monitor enumeration
        }
        else 
        {
            ++(pMonStruct->m_iMonCounter);
    
            MONITORINFO monInfoScript = ScrReadMonitorInfo(* pScript);
            MONITORINFO monInfoSystem = ::GetMonitorInfo(hMonitor);
            if (
                  (monInfoScript != monInfoSystem) ||
                  (pMonStruct->m_iMonFromScript != pMonStruct->m_iMonCounter)
              )
            {                                        // some monitor infos don't fit
                pMonStruct->m_bCheckResult = false;  // this is not the right configuration
                bRes = false;                        // stop monitor enumeration
            }
        }
    }
    catch (ScriptErrorHandler::ScriptException const & errInfo) // exception handling cannot pass through C function
    {                                                 
        pMonStruct->m_errorInfo = errInfo;                        // have to pass it manually
        bRes = false;
    }

    return bRes;  // true: everything ok so far, continue with monitor enumeration, false: stop monitor enumeration
}

// Syntax of monitor configuration file
// <monitor configuration file> = (0..n)<monitor configuration>
// <monitor configuration> = "MonitorConfiguration" (0..n)<monitor description> "0" <file name>
// <monitor description> = <monitor number> <rectangle> <rectangle> <monitor type>
// <monitor number> = <1..n>
// <rectangle> = (4)<integer number>
// <monitor type> = "0" | "1"

class WrapMonitorConfiguration : public ScriptFunctor
{
public:
    void operator() (Script & script) const final     // process one monitor configuration
    {
        CHECK_MON_STRUCT monStruct;
        monStruct.m_pScript = & script;

        wstring wstrFileName = script.ScrReadString();             // read window configuration file name

        EnumDisplayMonitors(nullptr, nullptr, CheckMonitorInfo, (LPARAM)&monStruct);
        if (monStruct.m_errorInfo.m_sErrNr != 0)                  // exception handling not passed through C function
            throw monStruct.m_errorInfo;                            // have to pass it manually

        if (monStruct.m_iMonFromScript != 0)                      // could be 0, if m_script describes less monitors than current configuration
        {
            monStruct.m_iMonFromScript = script.ScrReadInt();      // read monitor number 0 indicating end of monitor list

            if (monStruct.m_iMonFromScript != 0)                  // wasn't 0: m_script describes more monitors than current configuration
            {                           
                do                                                  // skip remaining monitor descriptions in m_script
                {
                    (void)ScrReadMonitorInfo(script);
                } while (script.ScrReadInt() != 0);
                monStruct.m_bCheckResult = false;                   // this is not the right configuration
            }
        }

        if (monStruct.m_bCheckResult)
            WinManager::SetWindowConfigurationFile(wstrFileName);

        WinManager::IncNrOfMonitorConfigurations();
    }
};

bool WinManager::GetWindowConfiguration()
{
    bool bRes { false };

    Script scriptWindowConfig;
    scriptWindowConfig.SetEcho(false);
	if (! scriptWindowConfig.ScrProcess(MONITOR_CONFIG_FILE))
	{
		wcout << COMMENT_SYMBOL << L" +++ Monitor configuration file " << MONITOR_CONFIG_FILE << L" not found or bad" << endl;
	} 
	else if (m_strWindowConfigurationFile.empty())
	{
		wcout << COMMENT_SYMBOL << L" +++ Monitor configuration unknown" << endl;
	}
    else
    {
	    if (! scriptWindowConfig.ScrProcess(m_strWindowConfigurationFile))
	    {
            wcout << COMMENT_SYMBOL
                  << L" +++ Window configuration file " 
                  << m_strWindowConfigurationFile
                  << L" missing or bad" << endl;
	    }
	    else
	    {
            wcout << COMMENT_SYMBOL
                  << L" *** Window configuration file " 
                  << m_strWindowConfigurationFile
                  << L" sucessfully processed" << endl;
            bRes = true;
	    }
    }

    if (! bRes)
        wcout << COMMENT_START << L"Using default window positions" << endl;

    return bRes;
}

//
// writing monitor information and window coordinates
//

struct DUMP_MON_STRUCT    // communication between DumpWindowCoordinates and DumpMonitorInfo
{
    int         m_iMonCounter;
    wofstream * m_postr;
};

static BOOL CALLBACK DumpMonitorInfo(HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData)
{
    auto      * const pMonStruct { (DUMP_MON_STRUCT*)dwData };
    wofstream * const postr      { pMonStruct->m_postr };
    MONITORINFO const monInfo    { ::GetMonitorInfo(hMonitor) };

    ++(pMonStruct->m_iMonCounter);

    * postr << pMonStruct->m_iMonCounter << L"   # monitor number"      << endl;
    * postr << monInfo.rcMonitor         << L"   # monitor rectangle"   << endl;
    * postr << monInfo.rcWork            << L"   # work area rectangle" << endl;
    * postr << (
                   (monInfo.dwFlags == 0) 
                   ? L"0    # secondary monitor" 
                   : L"1    # primary monitor"
               ) << endl;
    return true;
}

void WinManager::dumpMonitorConfiguration()
{
    wofstream ostr(MONITOR_CONFIG_FILE, wofstream::app);
	
	DUMP_MON_STRUCT monStruct = { 0, & ostr };

    ostr << L"MonitorConfiguration  " << Quoted(m_strWindowConfigurationFile) << endl;
    EnumDisplayMonitors(nullptr, nullptr, DumpMonitorInfo, (LPARAM)&monStruct);
    ostr << L"0   # end of MonitorConfiguration" << endl << endl;

    ostr.close();
}

void WinManager::StoreWindowConfiguration()
{
    if (m_strWindowConfigurationFile.empty())
    {
        m_strWindowConfigurationFile = WINDOW_CONFIG_FILE_STUB + to_wstring(++m_iNrOfMonitorConfigurations) + L".cnf";
        dumpMonitorConfiguration();
    }

    wofstream ostr(m_strWindowConfigurationFile, wofstream::out);
    m_upWrapMoveWindow->Write(ostr);
    m_upWrapShowWindow->Write(ostr);
    ostr.close();
}

void WinManager::Initialize()
{
    m_upMap            = make_unique<map<RootWinId, MAP_ELEMENT>>();
    m_upWrapMoveWindow = make_unique<WrapMoveWindow>(*m_upMap.get());
    m_upWrapShowWindow = make_unique<WrapShowWindow>(*m_upMap.get());

    SymbolTable::ScrDefConst(L"MonitorConfiguration", new WrapMonitorConfiguration());

    SymbolTable::ScrDefConst(L"SW_RESTORE",    static_cast<unsigned long>(SW_RESTORE   ));
    SymbolTable::ScrDefConst(L"SW_SHOWNORMAL", static_cast<unsigned long>(SW_SHOWNORMAL));
    SymbolTable::ScrDefConst(L"SW_MAXIMIZE",   static_cast<unsigned long>(SW_MAXIMIZE  ));
    SymbolTable::ScrDefConst(L"SW_SHOW",       static_cast<unsigned long>(SW_SHOW      ));
    SymbolTable::ScrDefConst(L"SW_HIDE",       static_cast<unsigned long>(SW_HIDE      ));
}

void WinManager::SetCaptions()
{
    for (const auto& [key, value] : *m_upMap.get())
        ::SendMessage(value.m_hwnd, WM_APP_CAPTION, 0, 0);
}

void WinManager::BringToTop(RootWinId const id)
{
    HWND hwnd { GetHWND(id) };
    BringWindowToTop(hwnd);
    ShowWindow(hwnd, SW_SHOWNORMAL);
}

void WinManager::addWindow
(
    wstring      const & wstrName, 
    RootWinId    const   id, 
    HWND         const   hwnd, 
    RootWindow * const   pRootWindow,
    bool         const   bTrackPosition,
    bool         const   bTrackSize
)
{
    if (id.GetValue() > 0)
    {
        m_upMap->try_emplace(id, MAP_ELEMENT(pRootWindow, wstrName, hwnd, bTrackPosition, bTrackSize));
        SymbolTable::ScrDefConst(wstrName, static_cast<unsigned long>(id.GetValue()));
    }
}

void WinManager::AddWindow
(
    wstring   const & wstrName, 
    RootWinId const   id, 
    HWND      const   hwnd, 
	bool      const   bTrackPosition,
	bool      const   bTrackSize
)
{
	if (hwnd)
        addWindow(wstrName, id, hwnd, nullptr, bTrackPosition, bTrackSize);
}

void WinManager::AddWindow
(
	wstring   const & wstrName, 
	RootWinId const   id, 
	BaseWindow      & baseWindow,
	bool      const   bTrackPosition,
	bool      const   bTrackSize
)
{
    addWindow(wstrName, id, baseWindow.GetWindowHandle(), & baseWindow, bTrackPosition, bTrackSize);
}

void WinManager::AddWindow
(
	wstring   const & wstrName, 
	RootWinId const   id, 
	BaseDialog      & baseDialog,
	bool      const   bTrackPosition,
	bool      const   bTrackSize
)
{
    addWindow(wstrName, id, baseDialog.GetWindowHandle(), nullptr, bTrackPosition, bTrackSize);
}

RootWinId WinManager::GetIdFromHWND(HWND const hwnd)
{
    for (const auto& [key, value] : *m_upMap.get())
        if (value.m_hwnd == hwnd)
            return key;
    return RootWinId(-1);
}

RootWinId WinManager::GetIdFromRootWindow(RootWindow const & RootWin)
{
    return GetIdFromHWND(RootWin.GetWindowHandle());
}

RootWindow* WinManager::GetRootWindow(RootWinId const id) 
{ 
    try
    { 
        return m_upMap->at(id).m_pRootWindow;
    }
    catch (const out_of_range&)
    {
        return nullptr;
    }
}

LRESULT WinManager::SendMessage(RootWinId const id, UINT const msg, WPARAM const wParam, LPARAM const lParam)
{
    return ::SendMessage(GetHWND(id), msg, wParam, lParam);
}

LRESULT WinManager::PostMessage(RootWinId const id, UINT const msg, WPARAM const wParam, LPARAM const lParam)
{
    return ::PostMessage(GetHWND(id), msg, wParam, lParam);
}

LRESULT WinManager::SendCommand(RootWinId const id, WPARAM const wParam, LPARAM const lParam)
{
    return SendMessage(id, WM_COMMAND, wParam, lParam);
}

LRESULT WinManager::PostCommand(RootWinId const id, WPARAM const wParam, LPARAM const lParam)
{
    return PostMessage(id, WM_COMMAND, wParam, lParam);
}

LRESULT WinManager::SendCommand2App(WPARAM const wParam, LPARAM const lParam)
{
    return SendCommand(RootWinId(IDM_APPL_WINDOW), wParam, lParam);
}

LRESULT WinManager::PostCommand2App(WPARAM const wParam, LPARAM const lParam)
{
    return PostCommand(RootWinId(IDM_APPL_WINDOW), wParam, lParam);
}

LRESULT WinManager::PostMessage2App(UINT const msg, WPARAM const wParam, LPARAM const lParam)
{
    return PostMessage(RootWinId(IDM_APPL_WINDOW), msg, wParam, lParam);
}

LRESULT WinManager::SendMessage2App(UINT const msg, WPARAM const wParam, LPARAM const lParam)
{
    return SendMessage(RootWinId(IDM_APPL_WINDOW), msg, wParam, lParam);
}

LRESULT WinManager::PostCommand2MainWin(WPARAM const wParam, LPARAM const lParam)
{
    return PostCommand(RootWinId(IDM_MAIN_WINDOW), wParam, lParam);
}

LRESULT WinManager::SendCommand2MainWin(WPARAM const wParam, LPARAM const lParam)
{
    return SendCommand(RootWinId(IDM_MAIN_WINDOW), wParam, lParam);
}

LRESULT WinManager::PostMessage2MainWin(UINT const msg, WPARAM const wParam, LPARAM const lParam)
{
    return PostMessage(RootWinId(IDM_MAIN_WINDOW), msg, wParam, lParam);
}

LRESULT WinManager::SendMessage2MainWin(UINT const msg, WPARAM const wParam, LPARAM const lParam)
{
    return SendMessage(RootWinId(IDM_MAIN_WINDOW), msg, wParam, lParam);
}
