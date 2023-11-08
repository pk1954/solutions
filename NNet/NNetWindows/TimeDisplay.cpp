// TimeDisplay.cpp
//
// NNetWindows
//
// Displays time since start of simulation in status bar field

module;

#include <iostream>
#include <sstream> 
#include <string> 
#include <chrono> 

module NNetWin32:TimeDisplay;

import Types;
import SimulationTime;
import StatusBar;

using std::make_unique;
using std::to_wstring;
using std::wstring;
using std::wostringstream;
using namespace std::chrono;

/////// inner class TimeDisplay::RefreshRate ///////

TimeDisplay::RefreshRate::RefreshRate	
(
	StatusBar * pStatusBar,
	int         iPartInStatusBar
)
  :	m_pStatusBar      (pStatusBar),
	m_iPartInStatusBar(iPartInStatusBar)
{ 
}

void TimeDisplay::RefreshRate::Trigger(bool const bImmediately)
{
	fMicroSecs const time { SimulationTime::Get() };
	m_pStatusBar->DisplayInPart(m_iPartInStatusBar, Format2wstring(time));
}

/////// functions of class TimeDisplay ///////

TimeDisplay::~TimeDisplay() = default;

void TimeDisplay::Initialize
(
	StatusBar * pStatusBar,
	int         iPartInStatusBar
)
{
	m_upRefreshRate = make_unique<RefreshRate>
	(
		pStatusBar,
		iPartInStatusBar
	);
	m_upRefreshRate->SetRefreshRate(300ms);
	pStatusBar->AddCustomControl(8);        // nr of characters 
}

void TimeDisplay::Notify(bool const bImmediately)
{
	if (m_upRefreshRate)
		m_upRefreshRate->Notify(bImmediately);
}
