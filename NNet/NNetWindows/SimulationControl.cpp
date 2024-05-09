// SimulationControl.cpp
//
// NNetWindows

module;

#include <compare>
#include "Resource.h"
#include <Windows.h>

module NNetWin32:SimulationControl;

import NNetPreferences;
import Win32_Util_Resource;
import Win32_Util;
import StatusBar;
import SlowMotionControl;
import :Compute;

void SimulationControl::Initialize
(
	StatusBar * const pStatusBar,
	Compute   * const pCompute
) 
{
	m_pCompute = pCompute;

	m_hwndRunStop = pStatusBar->AddButton(L"   Run    ", IDM_RUN,  BS_PUSHBUTTON);
	m_hwndScan    = pStatusBar->AddButton(L"   Scan   ", IDM_SCAN, BS_PUSHBUTTON);

	Notify(true);
}

void SimulationControl::Notify(bool const bImmediate)
{
	bool const bIsRunning       { m_pCompute->IsRunning() };
	bool const bIsScanRunning   { m_pCompute->IsScanRunning() };
	bool const bModelLocked     { m_pCompute->ModelLocked() };
	bool const bScanAreaVisible { NNetPreferences::ScanAreaVisible() };

	if (bIsRunning)
        ::SetDlgItemState(m_hwndRunStop, L"Stop", IDM_STOP, true);
	else
        ::SetDlgItemState(m_hwndRunStop, L"Run", IDM_RUN, true);

	if (bScanAreaVisible)
	{
		if (bModelLocked)
		{
			if (bIsRunning)  // Run mode (locked)
			{
				if (bIsScanRunning)  // Scan mode
					::SetDlgItemState(m_hwndScan, L" Scanning ", IDM_UNLOCK, false);
				else
					::SetDlgItemState(m_hwndScan, L"  Locked  ", IDM_UNLOCK, false);
			}
			else  // Locked mode - not running
				::SetDlgItemState(m_hwndScan, L"  Unlock  ", IDM_UNLOCK, true);
		}
		else  // Not locked
		{
			::SetDlgItemState(m_hwndScan, L"   Scan   ", IDM_SCAN, bIsRunning);
		}
		Show(m_hwndScan, true);
	}
	else
	{
		Show(m_hwndScan, false);
	}
}
