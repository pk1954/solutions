// SlowMotionDisplay.cpp
//
// Win32_utilities
//
// Displays slow motion ratio in status bar field

module;

#include <string>

module SlowMotionDisplay;

import Util;
import SaveCast;
import SlowMotionRatio;
import StatusBar;

using std::to_wstring;

void SlowMotionDisplay::Initialize
(
	StatusBar       * pStatusBar,
	SlowMotionRatio * pSlowMotionRatio,
	int               iPartInStatusBar
) 
{
	m_pStatusBar       = pStatusBar;
	m_pSlowMotionRatio = pSlowMotionRatio;
	m_iPartInStatusBar = iPartInStatusBar;

	pStatusBar->AddCustomControl(11);            // nr of characters 
}

void SlowMotionDisplay::Notify(bool const bImmediately)
{
	unsigned int const uiRatio { Cast2UnsignedInt(m_pSlowMotionRatio->GetRatio()) };
	if (uiRatio == 1)
		m_wstring = L"real time";
	else
		m_wstring = L"slow motion " + to_wstring(uiRatio) + L" X ";
	m_pStatusBar->DisplayInPart(m_iPartInStatusBar, m_wstring);
}
