// win32_simulationControl.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Resource.h"
#include "ComputeThread.h"
#include "win32_simulationControl.h"

void SimulationControl::Initialize
(
	StatusBar     * const pStatusBar,
	ComputeThread * const pComputeThread
) 
{
    m_pStatusBar     = pStatusBar;
	m_pComputeThread = pComputeThread;

	m_pStatusBar->AddButton( L"SingleStep", (HMENU)IDM_FORWARD, BS_PUSHBUTTON ); 
	m_pStatusBar->AddButton( L"   Run    ", (HMENU)IDM_RUN,     BS_PUSHBUTTON ); 
	m_pStatusBar->AddButton( L"  Stop    ", (HMENU)IDM_HALT,    BS_PUSHBUTTON ); 

	m_pComputeThread->AddRunObserver( this );
}

void SimulationControl::Notify( bool const bImmediate )
{
	bool const bIsRunning = m_pComputeThread->IsRunning();

	EnableWindow( m_pStatusBar->GetDlgItem( IDM_RUN  ),    ! bIsRunning );
	EnableWindow( m_pStatusBar->GetDlgItem( IDM_HALT ),      bIsRunning );
	EnableWindow( m_pStatusBar->GetDlgItem( IDM_FORWARD ), ! bIsRunning );
}
