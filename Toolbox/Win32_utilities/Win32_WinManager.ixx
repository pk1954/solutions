// win32_winManager.ixx
//
// Win32_utilities

module;

#include <string>
#include <unordered_map>
#include <Windows.h>
#include "win32_util.h"
#include "win32_util_resource.h"
#include "win32_rootWindow.h"

export module WinManager;

import ErrHndl;
import BaseDialog;
import BaseWindow;

using std::wstring;
using std::unordered_map;

export class WinManager
{
public:

	WinManager();

	void AddWindow(wstring const &, UINT const, HWND,         bool const, bool const);
	void AddWindow(wstring const &, UINT const, BaseWindow &, bool const, bool const);
	void AddWindow(wstring const &, UINT const, BaseDialog &, bool const, bool const);

	void RemoveWindow(UINT const id) { m_map.erase(id);	}
	void RemoveAll   ()              { m_map.clear(); }
	void SetCaptions () 
	{
		Apply2All
		(
			[](BaseWindow &b)
			{ ::SendMessage(b.GetWindowHandle(), WM_APP_CAPTION, 0, 0); }
		);
	}

	void Apply2All(auto const & f) const
	{
		for (const auto & [key, value] : m_map)
		{
			BaseWindow * const pBaseWindow { value.m_pBaseWindow };
			if (pBaseWindow)
				f(*pBaseWindow);
		}
	}                        

	wstring GetWindowName(UINT const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_wstr;
	}

	HWND GetHWND(UINT const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_hwnd;
	}

	void BringToTop(UINT const id) const
	{
		HWND hwnd { GetHWND(id) };
		BringWindowToTop(hwnd);
		ShowWindow(hwnd, SW_SHOWNORMAL);
	}

	void AdjustRight(UINT const id, PIXEL const pixYpos = 0_PIXEL) const
	{
		Util::AdjustRight(GetHWND(id), pixYpos);
	}

	void AdjustLeft(UINT const id, PIXEL const pixYpos = 0_PIXEL) const
	{
		Util::AdjustLeft(GetHWND(id), pixYpos);
	}

	BaseWindow const * GetBaseWindow(UINT const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_pBaseWindow;
	}

	INT GetIdFromRootWindow(HWND const hwnd) const
	{
		for (const auto & [key, value] : m_map)
			if (value.m_hwnd == hwnd)
				return key; 
		return -1;
	}

	bool IsMoveable(UINT const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_bTrackPosition;
	}

	bool IsSizeable(UINT const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_bTrackSize;
	}

	bool IsVisible(UINT const id) const // can throw out_of_range exception
	{
		return IsWindowVisible(GetHWND(id));
	}

	void Show(UINT const id, tBoolOp const op) const
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

private:
	wstring const MONITOR_CONFIG_FILE     = L"MonitorConfigurations.cnf";
	wstring const WINDOW_CONFIG_FILE_STUB = L"WindowConfiguration";

	struct MAP_ELEMENT
	{
		BaseWindow  * m_pBaseWindow;    // Normally WinManager handles BaseWindows
		wstring const m_wstr;
		HWND    const m_hwnd;           // but in some cases also naked HWNDs are used
		bool    const m_bTrackPosition; // if true, winManager sets window position from config file
		bool    const m_bTrackSize;     // if true, winManager sets window size from config file
	};

	unordered_map<UINT, MAP_ELEMENT> m_map;

	wstring m_strWindowConfigurationFile { L"" };
	int     m_iNrOfMonitorConfigurations { 0 };

	ScriptErrorHandler::ScriptException m_errorInfo {};

	void dumpMonitorConfiguration() const;
	void dumpWindowCoordinates() const;

	void addWindow
	(
		wstring      const &,
		UINT         const,
		HWND         const,
		BaseWindow * const,
		bool         const,
		bool         const
	);
};
