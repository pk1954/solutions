// win32_simulationControl.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Resource.h"
#include "win32_WorkThreadInterface.h"
#include "win32_simulationControl.h"

StatusBar * SimulationControl::m_pStatusBar = nullptr;

void SimulationControl::Add( StatusBar * const pStatusBar )
{ 
	m_pStatusBar = pStatusBar;

	m_pStatusBar->AddButton( L"SingleStep", (HMENU)IDM_FORWARD, BS_PUSHBUTTON ); 
	m_pStatusBar->AddButton( L"   Run    ", (HMENU)IDM_RUN,     BS_PUSHBUTTON ); 
	m_pStatusBar->AddButton( L"  Stop    ", (HMENU)IDM_STOP,    BS_PUSHBUTTON ); 
} 

void SimulationControl::Adjust
(
	BOOL                  const bIsRunning,
	WorkThreadInterface * const pWorkThreadInterface
)
{
	EnableWindow( m_pStatusBar->GetDlgItem( IDM_RUN  ),    ! bIsRunning );
	EnableWindow( m_pStatusBar->GetDlgItem( IDM_STOP ),      bIsRunning );
	EnableWindow( m_pStatusBar->GetDlgItem( IDM_FORWARD ), ! bIsRunning );
}
