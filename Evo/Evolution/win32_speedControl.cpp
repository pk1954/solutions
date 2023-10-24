// win32_speedControl.cpp
//
// win32_appFramework


#include "win32_speedControl.h"

StatusBar     * SpeedControl::m_pStatusBar     = nullptr;
HistorySystem * SpeedControl::m_pHistorySystem = nullptr;

void SpeedControl::Add
(
	StatusBar     * const pStatusBar, 
	HistorySystem * const pHistorySystem,
	long            const lTrackBarMin,
	long            const lTrackBarMax,
	long            const lTrackBarInitial
	)
{ 
	m_pStatusBar     = pStatusBar;
	m_pHistorySystem = pHistorySystem;

	if (m_pHistorySystem)
		m_pStatusBar->AddButton  (L"Backwards ", (HMENU)IDM_BACKWARDS, BS_PUSHBUTTON);

	m_pStatusBar->AddButton  (L"SingleStep", (HMENU)IDM_FORWARD,   BS_PUSHBUTTON); 
	m_pStatusBar->AddButton  (L"   Run    ", (HMENU)IDM_RUN,       BS_PUSHBUTTON); 
	m_pStatusBar->AddButton  (L"  Stop    ", (HMENU)IDM_STOP,      BS_PUSHBUTTON); 
	m_pStatusBar->AddTrackBar(               (HMENU)IDM_SIMULATION_SPEED); 
	m_pStatusBar->AddButton  (L" MaxSpeed ", (HMENU)IDM_MAX_SPEED, BS_PUSHBUTTON); 

	m_pStatusBar->SetTrackBarRange(IDM_SIMULATION_SPEED, lTrackBarMin, lTrackBarMax);
	m_pStatusBar->SetTrackBarPos  (IDM_SIMULATION_SPEED, lTrackBarInitial);                
} 

void SpeedControl::Adjust
(
	BOOL                  const bIsRunning,
	WorkThreadInterface * const pWorkThreadInterface
)
{
	EnableWindow(m_pStatusBar->GetDlgItem(IDM_RUN ),    ! bIsRunning);
	EnableWindow(m_pStatusBar->GetDlgItem(IDM_STOP),      bIsRunning);
	EnableWindow(m_pStatusBar->GetDlgItem(IDM_FORWARD), ! bIsRunning);

	if (m_pHistorySystem)
	{
		BOOL bIsFirstGeneration = pWorkThreadInterface->GetCurrentGeneration() == 0;
		EnableWindow(m_pStatusBar-> GetDlgItem(IDM_BACKWARDS), ! (bIsRunning || bIsFirstGeneration));
	}
}
