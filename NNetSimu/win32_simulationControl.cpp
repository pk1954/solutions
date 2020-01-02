// win32_simulationControl.cpp
//
// NNetSimu

#include "stdafx.h"
#include "Resource.h"
#include "win32_NNetWorkThreadInterface.h"
#include "win32_simulationControl.h"

SimulationControl::SimulationControl
(
	StatusBar               * const pStatusBar,
	NNetWorkThreadInterface * const pWorkThreadInterface
) 
  : m_pStatusBar          ( pStatusBar ),
	m_pWorkThreadInterface( pWorkThreadInterface )
{
	m_pStatusBar->AddButton( L"SingleStep", (HMENU)IDM_FORWARD, BS_PUSHBUTTON ); 
	m_pStatusBar->AddButton( L"   Run    ", (HMENU)IDM_RUN,     BS_PUSHBUTTON ); 
	m_pStatusBar->AddButton( L"  Stop    ", (HMENU)IDM_STOP,    BS_PUSHBUTTON ); 

	m_pWorkThreadInterface->AddRunObserver( this );
}

void SimulationControl::Notify( bool const bImmediate )
{
	BOOL const bIsRunning = m_pWorkThreadInterface->IsRunning();

	EnableWindow( m_pStatusBar->GetDlgItem( IDM_RUN  ),    ! bIsRunning );
	EnableWindow( m_pStatusBar->GetDlgItem( IDM_STOP ),      bIsRunning );
	EnableWindow( m_pStatusBar->GetDlgItem( IDM_FORWARD ), ! bIsRunning );
}
