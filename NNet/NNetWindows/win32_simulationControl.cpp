// Win32_SimulationControl.cpp
//
// NNetSimu

module;

#include "Resource.h"
#include "ComputeThread.h"
#include "win32_util_resource.h"

module SimulationControl;

void SimulationControl::Initialize
(
	StatusBar     * const pStatusBar,
	ComputeThread * const pComputeThread
) 
{
    m_pStatusBar     = pStatusBar;
	m_pComputeThread = pComputeThread;

	m_pStatusBar->AddButton(L"SingleStep", HMENU(IDM_FORWARD),  BS_PUSHBUTTON); 
	m_pStatusBar->AddButton(L"   Run    ", HMENU(IDM_RUN_STOP), BS_PUSHBUTTON); 

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
