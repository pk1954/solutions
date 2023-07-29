// Win32_WinManager.cpp
//
// Win32_utilities

#include <cassert>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <map>

import Scanner;
import Symtab;
import Script;
import WrapBase;
import IoUtil;
import WinManager;
import ErrHndl;
import Win32_Util_Resource;
import Win32_Util;
import Win32_PIXEL;
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

using Util::operator==;
using Util::operator!=;
using Util::operator<<; 

class WrapMoveWindow : public ScriptFunctor // WrapBase
{
public:
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
                    bool bRes = Util::MoveWindowAbsolute(hwnd, pixRect, true);
                    assert(bRes);
                }
                else
                {
     				bool bRes = Util::MoveWindowAbsolute(hwnd, pixRect.GetStartPoint(), true); 
                	DWORD dwErr = GetLastError();
                	assert(bRes);
                }
            }
		}
    }

private:
    inline static const wstring NAME { L"MoveWindow" };
};

class WrapShowWindow : public ScriptFunctor
{
public:
    void operator() (Script & script) const final
    {
	    RootWinId const id      (script.ScrReadInt());
		INT       const iCmdShow(script.ScrReadInt());  // WM_HIDE, WM_SHOW, ...
		if (id.GetValue() > 0)
			ShowWindow(WinManager::GetHWND(id), iCmdShow);
    }
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
    monInfo.rcMonitor = Util::ScrReadRECT(script);
    monInfo.rcWork    = Util::ScrReadRECT(script);
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
            MONITORINFO monInfoSystem = Util::GetMonitorInfo(hMonitor);
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
		wcout << Scanner::COMMENT_SYMBOL << L" +++ Monitor configuration file " << MONITOR_CONFIG_FILE << L" not found or bad" << endl;
	} 
	else if (m_strWindowConfigurationFile.empty())
	{
		wcout << Scanner::COMMENT_SYMBOL << L" +++ Monitor configuration unknown" << endl;
	}
    else
    {
	    if (! scriptWindowConfig.ScrProcess(m_strWindowConfigurationFile))
	    {
            wcout << Scanner::COMMENT_SYMBOL
                  << L" +++ Window configuration file " 
                  << m_strWindowConfigurationFile
                  << L" missing or bad" << endl;
	    }
	    else
	    {
            wcout << Scanner::COMMENT_SYMBOL
                  << L" *** Window configuration file " 
                  << m_strWindowConfigurationFile
                  << L" sucessfully processed" << endl;
            bRes = true;
	    }
    }

    if (! bRes)
        wcout << Scanner::COMMENT_START << L"Using default window positions" << endl;

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
    MONITORINFO const monInfo    { Util::GetMonitorInfo(hMonitor) };

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

    ostr << L"MonitorConfiguration  \"" << m_strWindowConfigurationFile << DOUBLE_QUOTE << endl;
    EnumDisplayMonitors(nullptr, nullptr, DumpMonitorInfo, (LPARAM)&monStruct);
    ostr << L"0   # end of MonitorConfiguration" << endl << endl;

    ostr.close();
}

void WinManager::dumpWindowCoordinates
(
    wofstream & ostr,
    MAP_ELEMENT const &elem
)
{
    if (elem.m_bTrackPosition)
    {
        HWND hwnd = elem.m_hwnd;
        if (hwnd != nullptr)
        {
           ostr << L"MoveWindow" << SPACE
                << elem.m_wstr
                << Util::GetWindowLeftPos(hwnd)
                << Util::GetWindowTop(hwnd)
                << Util::GetWindowWidth(hwnd)
                << Util::GetWindowHeight(hwnd)
                << endl;

           ostr << L"ShowWindow" << SPACE
                << elem.m_wstr << SPACE
                << (
                      IsWindowVisible(hwnd)
                      ? (IsZoomed(hwnd) ? L"SW_MAXIMIZE" : L"SW_SHOWNORMAL")
                      : L"SW_HIDE"
                   )
                << endl;
        }
    }
}

void WinManager::StoreWindowConfiguration()
{
    if (m_strWindowConfigurationFile.empty())
    {
        m_strWindowConfigurationFile = WINDOW_CONFIG_FILE_STUB + to_wstring(++m_iNrOfMonitorConfigurations) + L".cnf";
        dumpMonitorConfiguration();
    }

    wofstream ostr(m_strWindowConfigurationFile, wofstream::out);
    for (const auto& [key, value] : *m_upMap.get())
        dumpWindowCoordinates(ostr, value);
    ostr.close();
}

void WinManager::Initialize()
{
    SymbolTable::ScrDefConst(L"MoveWindow",           new WrapMoveWindow());
    SymbolTable::ScrDefConst(L"ShowWindow",           new WrapShowWindow());
    SymbolTable::ScrDefConst(L"MonitorConfiguration", new WrapMonitorConfiguration());

    SymbolTable::ScrDefConst(L"SW_RESTORE",    static_cast<unsigned long>(SW_RESTORE   ));
    SymbolTable::ScrDefConst(L"SW_SHOWNORMAL", static_cast<unsigned long>(SW_SHOWNORMAL));
    SymbolTable::ScrDefConst(L"SW_MAXIMIZE",   static_cast<unsigned long>(SW_MAXIMIZE  ));
    SymbolTable::ScrDefConst(L"SW_SHOW",       static_cast<unsigned long>(SW_SHOW      ));
    SymbolTable::ScrDefConst(L"SW_HIDE",       static_cast<unsigned long>(SW_HIDE      ));

    m_upMap = make_unique<map<RootWinId, MAP_ELEMENT>>();
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
    BaseWindow * const   pBaseWindow,
    bool         const   bTrackPosition,
    bool         const   bTrackSize
)
{
    if (id.GetValue() > 0)
    {
        m_upMap->try_emplace(id, MAP_ELEMENT(pBaseWindow, wstrName, hwnd, bTrackPosition, bTrackSize));
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

RootWinId WinManager::GetIdFromRootWindow(HWND const hwnd)
{
    for (const auto& [key, value] : *m_upMap.get())
        if (value.m_hwnd == hwnd)
            return key;
    return RootWinId(-1);
}

LRESULT WinManager::SendCommand(RootWinId const id, WPARAM const wParam, LPARAM const lParam)
{
    return ::SendMessage(GetHWND(id), WM_COMMAND, wParam, lParam);
}

LRESULT WinManager::PostCommand(RootWinId const id, WPARAM const wParam, LPARAM const lParam)
{
    return ::PostMessage(GetHWND(id), WM_COMMAND, wParam, lParam);
}
