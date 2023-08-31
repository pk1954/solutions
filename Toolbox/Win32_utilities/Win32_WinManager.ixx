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
import ErrHndl;
import Wrapper;
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

export class WinManager  // static class
{
public:
	WinManager();

	static void Apply2All(auto const & f)
	{
		for (const auto & [key, value] : *m_upMap.get())
			f(key, value);
	}                        

	static void AddWindow(wstring const &, RootWinId const, HWND,         bool const, bool const);
	static void AddWindow(wstring const &, RootWinId const, BaseWindow &, bool const, bool const);
	static void AddWindow(wstring const &, RootWinId const, BaseDialog &, bool const, bool const);

	static void RemoveWindow(RootWinId const id) { m_upMap->erase(id); }
	static void RemoveAll   ()                   { m_upMap->clear(); }
	static void SetCaptions();

	static void BringToTop (RootWinId const);
	static void AdjustRight(RootWinId const id, PIXEL const p = 0_PIXEL) { ::AdjustRight(GetHWND(id), p); }
	static void AdjustLeft (RootWinId const id, PIXEL const p = 0_PIXEL) { ::AdjustLeft(GetHWND(id), p);	}

	static wstring const& GetWindowName(RootWindow const& w) { return GetWindowName(GetIdFromRootWindow(w)); }

	static wstring const& GetWindowName(RootWinId const id) { return m_upMap->at(id).m_wstr; }
	static HWND           GetHWND      (RootWinId const id) { return m_upMap->at(id).m_hwnd; }
	static bool           IsMoveable   (RootWinId const id) { return m_upMap->at(id).m_bTrackPosition; }
	static bool           IsSizeable   (RootWinId const id) { return m_upMap->at(id).m_bTrackSize; }
	static bool           IsVisible    (RootWinId const id) { return IsWindowVisible(GetHWND(id)); }

	static RootWindow* GetRootWindow(RootWinId const);
	static RootWinId   GetIdFromRootWindow(RootWindow const &);
	static RootWinId   GetIdFromHWND(HWND const);

	static LRESULT SendCommand2MainWin(WPARAM const, LPARAM const = 0);
	static LRESULT PostCommand2MainWin(WPARAM const, LPARAM const = 0);

	static LRESULT SendMessage2MainWin(UINT const, WPARAM const, LPARAM const = 0);
	static LRESULT PostMessage2MainWin(UINT const, WPARAM const, LPARAM const = 0);

	static LRESULT SendCommand2App(WPARAM const, LPARAM const = 0);
	static LRESULT PostCommand2App(WPARAM const, LPARAM const = 0);

	static LRESULT SendMessage2App(UINT const, WPARAM const, LPARAM const = 0);
	static LRESULT PostMessage2App(UINT const, WPARAM const, LPARAM const = 0);

	static LRESULT SendCommand(RootWinId const, WPARAM const, LPARAM const = 0);
	static LRESULT PostCommand(RootWinId const, WPARAM const, LPARAM const = 0);

	static LRESULT SendMessage(RootWinId const, UINT const, WPARAM const, LPARAM const = 0);
	static LRESULT PostMessage(RootWinId const, UINT const, WPARAM const, LPARAM const = 0);

	static void Show(RootWinId const id, tBoolOp const op) { ::Show(GetHWND(id), op);	}

	static void SetWindowConfigurationFile(wstring const& n) { m_strWindowConfigurationFile = n; };

	static void IncNrOfMonitorConfigurations() { ++m_iNrOfMonitorConfigurations; };

	static bool GetWindowConfiguration();
	static void StoreWindowConfiguration();

	struct MAP_ELEMENT
	{
		RootWindow  * m_pRootWindow;    // Normally WinManager handles RootWindows
		wstring const m_wstr;
		HWND    const m_hwnd;           // but in some cases also naked HWNDs are used
		bool    const m_bTrackPosition; // if true, winManager sets window position from config file
		bool    const m_bTrackSize;     // if true, winManager sets window size from config file
	};

	using WIN_MAP = map<RootWinId, WinManager::MAP_ELEMENT>;

private:

	static inline wstring const MONITOR_CONFIG_FILE     = L"MonitorConfigurations.cnf";
	static inline wstring const WINDOW_CONFIG_FILE_STUB = L"WindowConfiguration";

	static inline unique_ptr<WIN_MAP> m_upMap;
	static inline unique_ptr<Wrapper> m_upWrapMoveWindow;
	static inline unique_ptr<Wrapper> m_upWrapShowWindow;

	static inline wstring m_strWindowConfigurationFile { L"" };
	static inline int     m_iNrOfMonitorConfigurations { 0 };

	static inline ScriptErrorHandler::ScriptException m_errorInfo {};

	static void dumpMonitorConfiguration();
	static void addWindow
	(
		wstring      const &,
		RootWinId    const,
		HWND         const,
		RootWindow * const,
		bool         const,
		bool         const
	);
};
