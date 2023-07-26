// Win32_WinManager.ixx
//
// Win32_utilities

module;

#include <string>
#include <fstream>
#include <map>
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
using std::map;

export using RootWinId = NamedType<int, struct RootWinId_Parameter>;

export class WinManager
{
public:

	WinManager();

	void AddWindow(wstring const &, RootWinId const, HWND,         bool const, bool const);
	void AddWindow(wstring const &, RootWinId const, BaseWindow &, bool const, bool const);
	void AddWindow(wstring const &, RootWinId const, BaseDialog &, bool const, bool const);

	void RemoveWindow(RootWinId const id) { m_map.erase(id); }
	void RemoveAll   ()                   { m_map.clear(); }
	void SetCaptions () 
	{
		for (const auto& [key, value] : m_map)
			::SendMessage(value.m_hwnd, WM_APP_CAPTION, 0, 0);
	}

	void Apply2All(auto const & f) const
	{
		for (const auto & [key, value] : m_map)
			f(key, value);
	}                        

	wstring const & GetWindowName(RootWinId const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_wstr;
	}

	HWND GetHWND(RootWinId const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_hwnd;
	}

	void BringToTop(RootWinId const id) const
	{
		HWND hwnd { GetHWND(id) };
		BringWindowToTop(hwnd);
		ShowWindow(hwnd, SW_SHOWNORMAL);
	}

	void AdjustRight(RootWinId const id, PIXEL const pixYpos = 0_PIXEL) const
	{
		Util::AdjustRight(GetHWND(id), pixYpos);
	}

	void AdjustLeft(RootWinId const id, PIXEL const pixYpos = 0_PIXEL) const
	{
		Util::AdjustLeft(GetHWND(id), pixYpos);
	}

	BaseWindow * GetBaseWindow(RootWinId const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_pBaseWindow;
	}

	RootWinId GetIdFromRootWindow(HWND const hwnd) const
	{
		for (const auto & [key, value] : m_map)
			if (value.m_hwnd == hwnd)
				return key; 
		return RootWinId(-1);
	}

	LRESULT SendCommand(RootWinId const id, WPARAM const wParam, LPARAM const lParam = 0) const
	{ 
		return GetBaseWindow(id)->SendCommand(wParam, lParam);
	}

	bool IsMoveable(RootWinId const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_bTrackPosition;
	}

	bool IsSizeable(RootWinId const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_bTrackSize;
	}

	bool IsVisible(RootWinId const id) const // can throw out_of_range exception
	{
		return IsWindowVisible(GetHWND(id));
	}

	void Show(RootWinId const id, tBoolOp const op) const
	{
		Util::Show(GetHWND(id), op);
	}

	void SetWindowConfigurationFile(wstring const & fileName) 
	{ 
		m_strWindowConfigurationFile = fileName; 
	};

	void IncNrOfMonitorConfigurations() 
	{ 
		++m_iNrOfMonitorConfigurations; 
	};

	bool GetWindowConfiguration() const;
	void StoreWindowConfiguration();

	struct MAP_ELEMENT
	{
		BaseWindow  * m_pBaseWindow;    // Normally WinManager handles BaseWindows
		wstring const m_wstr;
		HWND    const m_hwnd;           // but in some cases also naked HWNDs are used
		bool    const m_bTrackPosition; // if true, winManager sets window position from config file
		bool    const m_bTrackSize;     // if true, winManager sets window size from config file
	};

private:
	wstring const MONITOR_CONFIG_FILE     = L"MonitorConfigurations.cnf";
	wstring const WINDOW_CONFIG_FILE_STUB = L"WindowConfiguration";

	map<RootWinId, MAP_ELEMENT> m_map;

	wstring m_strWindowConfigurationFile { L"" };
	int     m_iNrOfMonitorConfigurations { 0 };

	ScriptErrorHandler::ScriptException m_errorInfo {};

	void dumpMonitorConfiguration() const;
	void dumpWindowCoordinates(wofstream &, MAP_ELEMENT const&) const;

	void addWindow
	(
		wstring      const &,
		RootWinId    const,
		HWND         const,
		BaseWindow * const,
		bool         const,
		bool         const
	);
};
