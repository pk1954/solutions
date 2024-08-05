// TimeDisplay.cpp
//
// Toolbox\Win32_utilities
//
// Displays time since start of simulation in status bar field

module TimeDisplay;

import std;
import Types;
import SimulationTime;
import StatusBar;

using std::make_unique;
using std::to_wstring;
using std::wstring;
using std::wostringstream;
using namespace std::chrono;

/////// inner class TimeDisplay::RefreshRate ///////

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
