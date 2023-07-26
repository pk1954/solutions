// Win32_WinManager.ixx
//
// Win32_utilities

module;

#include <string>
#include <fstream>
#include <map>
#include <memory>
#include <Windows.h>

export module WinManager;

import Types;
import BoolOp;
import Win32_Util_Resource;
import ErrHndl;
import BaseDialog;
import BaseWindow;
import Win32_Util;
import Win32_PIXEL;
import RootWindow;

using std::wstring;
using std::wofstream;
using std::unique_ptr;
using std::map;

export using RootWinId = NamedType<int, struct RootWinId_Parameter>;

export class WinManager
{
public:

	static void Initialize();

	static void AddWindow(wstring const &, RootWinId const, HWND,         bool const, bool const);
	static void AddWindow(wstring const &, RootWinId const, BaseWindow &, bool const, bool const);
	static void AddWindow(wstring const &, RootWinId const, BaseDialog &, bool const, bool const);

	static void RemoveWindow(RootWinId const id) { m_upMap->erase(id); }
	static void RemoveAll   ()                   { m_upMap->clear(); }
	static void SetCaptions ()
	{
		for (const auto& [key, value] : *m_upMap.get())
			::SendMessage(value.m_hwnd, WM_APP_CAPTION, 0, 0);
	}

	static void Apply2All(auto const & f)
	{
		for (const auto & [key, value] : *m_upMap.get())
			f(key, value);
	}                        

	static void BringToTop(RootWinId const id)
	{
		HWND hwnd { GetHWND(id) };
		BringWindowToTop(hwnd);
		ShowWindow(hwnd, SW_SHOWNORMAL);
	}

	static void AdjustRight(RootWinId const id, PIXEL const p = 0_PIXEL) { Util::AdjustRight(GetHWND(id), p); }
	static void AdjustLeft (RootWinId const id, PIXEL const p = 0_PIXEL) { Util::AdjustLeft(GetHWND(id), p);	}

	static wstring const& GetWindowName(RootWinId const id) { return m_upMap->at(id).m_wstr; }
	static HWND           GetHWND      (RootWinId const id) { return m_upMap->at(id).m_hwnd; }
	static BaseWindow   * GetBaseWindow(RootWinId const id) { return m_upMap->at(id).m_pBaseWindow; }
	static bool           IsMoveable   (RootWinId const id) { return m_upMap->at(id).m_bTrackPosition; }
	static bool           IsSizeable   (RootWinId const id) { return m_upMap->at(id).m_bTrackSize; }
	static bool           IsVisible    (RootWinId const id) { return IsWindowVisible(GetHWND(id)); }

	static RootWinId GetIdFromRootWindow(HWND const hwnd)
	{
		for (const auto & [key, value] : *m_upMap.get())
			if (value.m_hwnd == hwnd)
				return key; 
		return RootWinId(-1);
	}

	static LRESULT SendCommand(RootWinId const id, WPARAM const wParam, LPARAM const lParam = 0)
	{
		return GetBaseWindow(id)->SendCommand(wParam, lParam);
	}

	static LRESULT PostCommand(RootWinId const id, WPARAM const wParam, LPARAM const lParam = 0)
	{
		return GetBaseWindow(id)->PostCommand(wParam, lParam);
	}

	static void Show(RootWinId const id, tBoolOp const op) { Util::Show(GetHWND(id), op);	}

	static void SetWindowConfigurationFile(wstring const& n) { m_strWindowConfigurationFile = n; };

	static void IncNrOfMonitorConfigurations() { ++m_iNrOfMonitorConfigurations; };

	static bool GetWindowConfiguration();
	static void StoreWindowConfiguration();

	struct MAP_ELEMENT
	{
		BaseWindow  * m_pBaseWindow;    // Normally WinManager handles BaseWindows
		wstring const m_wstr;
		HWND    const m_hwnd;           // but in some cases also naked HWNDs are used
		bool    const m_bTrackPosition; // if true, winManager sets window position from config file
		bool    const m_bTrackSize;     // if true, winManager sets window size from config file
	};

private:
	static inline wstring const MONITOR_CONFIG_FILE     = L"MonitorConfigurations.cnf";
	static inline wstring const WINDOW_CONFIG_FILE_STUB = L"WindowConfiguration";

	static inline unique_ptr<map<RootWinId, MAP_ELEMENT>> m_upMap;

	static inline wstring m_strWindowConfigurationFile { L"" };
	static inline int     m_iNrOfMonitorConfigurations { 0 };

	static inline ScriptErrorHandler::ScriptException m_errorInfo {};

	static void dumpMonitorConfiguration();
	static void dumpWindowCoordinates(wofstream &, MAP_ELEMENT const&);

	static void addWindow
	(
		wstring      const &,
		RootWinId    const,
		HWND         const,
		BaseWindow * const,
		bool         const,
		bool         const
	);
};
