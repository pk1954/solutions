// win32_winManager.h : 
//
// Win32_utilities

#pragma once

#include <string>
#include <unordered_map>
#include "errhndl.h"
#include "win32_util.h"
#include "win32_baseWindow.h"
#include "win32_baseDialog.h"

using std::wstring;
using std::unordered_map;

class RootWindow;

class WinManager
{
public:
    WinManager();
    virtual ~WinManager() { };

	void AddWindow(wstring const, UINT const, HWND,               bool const, bool const);
	void AddWindow(wstring const, UINT const, BaseWindow const &, bool const, bool const);
	void AddWindow(wstring const, UINT const, BaseDialog const &, bool const, bool const);

	void RemoveWindow(UINT const id)
	{
		m_map.erase(id);
	}

	void RemoveAll()
	{
		m_map.clear ();
	}

	std::wstring const GetWindowName(UINT const id) const // can throw out_of_range exception
    {
        return m_map.at(id).m_wstr;
    }

	HWND const GetHWND(UINT const id) const // can throw out_of_range exception
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

	BaseWindow const * const GetBaseWindow(UINT const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_pBaseWindow;
	}

	INT const GetIdFromRootWindow(HWND const hwnd)
	{
		for (auto & pp : m_map)
			if (pp.second.m_hwnd == hwnd)
				return pp.first; 
		return -1;
	}

    bool const IsMoveable(UINT const id) const // can throw out_of_range exception
    {
        return m_map.at(id).m_bTrackPosition;
    }

	bool const IsSizeable(UINT const id) const // can throw out_of_range exception
	{
		return m_map.at(id).m_bTrackSize;
	}

	bool const IsVisible(UINT const id) const // can throw out_of_range exception
	{
		return IsWindowVisible(GetHWND(id));
	}

	void Show(UINT const id, tBoolOp const op) const
    {
        Util::Show(GetHWND(id), op);
    }

    void SetWindowConfigurationFile(std::wstring const fileName) 
    { 
        m_strWindowConfigurationFile = fileName; 
    };

    void IncNrOfMonitorConfigurations() 
    { 
        ++m_iNrOfMonitorConfigurations; 
    };

    bool GetWindowConfiguration();
    void StoreWindowConfiguration();

private:
    wstring const MONITOR_CONFIG_FILE     = L"MonitorConfigurations.cnf";
    wstring const WINDOW_CONFIG_FILE_STUB = L"WindowConfiguration";

    struct MAP_ELEMENT
    {
        wstring    const   m_wstr;
		BaseWindow const * m_pBaseWindow;    // Normally WinManager handles BaseWindows
		HWND       const   m_hwnd;           // but in some cases also naked HWNDs are used
		bool       const   m_bTrackPosition; // if true, winManager sets window position from config file
		bool       const   m_bTrackSize;     // if true, winManager sets window size from config file
    };

    unordered_map< UINT, MAP_ELEMENT > m_map;
    
    wstring m_strWindowConfigurationFile;
    int     m_iNrOfMonitorConfigurations;

	ScriptErrorHandler::ScriptException m_errorInfo {};

    void dumpMonitorConfiguration() const;
    void dumpWindowCoordinates() const;

	void addWindow
	(
		wstring    const,
		UINT       const,
		HWND       const,
		BaseWindow const * const,
		bool       const,
		bool       const
	);
};
