// SimulationControl.cpp
//
// NNetSimu

module;

#include <compare>
#include "Resource.h"
#include <Windows.h>

module NNetWin32:SimulationControl;

import Win32_Util_Resource;
import :ComputeThread;
import :StatusBar;

void SimulationControl::Initialize
(
	StatusBar     * const pStatusBar,
	ComputeThread * const pComputeThread
) 
{
    m_pStatusBar     = pStatusBar;
	m_pComputeThread = pComputeThread;

	m_pStatusBar->AddButton(L"   Scan   ", IDM_SCAN,     BS_PUSHBUTTON);
	m_pStatusBar->AddButton(L"SingleStep", IDM_FORWARD,  BS_PUSHBUTTON);
	m_pStatusBar->AddButton(L"   Run    ", IDM_RUN_STOP, BS_PUSHBUTTON);

	Notify(true);
}

void SimulationControl::Notify(bool const bImmediate)
{
	bool const bIsRunning = m_pComputeThread->IsRunning();

	if (bIsRunning)
		SetWindowText(m_pStatusBar->GetDlgItem(IDM_RUN_STOP), L"Stop");
	else
		SetWindowText(m_pStatusBar->GetDlgItem(IDM_RUN_STOP), L"Run");
	EnableWindow(m_pStatusBar->GetDlgItem(IDM_FORWARD), ! bIsRunning);
}
