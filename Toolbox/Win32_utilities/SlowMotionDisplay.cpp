// SlowMotionDisplay.cpp
//
// Win32_utilities
//
// Displays slow motion ratio in status bar field

module;

#include <string>
#include <sstream>
#include <iomanip>
#include <memory>
#include <chrono> 
#include <Windows.h>

module SlowMotionDisplay;

import Util;
import Types;
import SaveCast;
import SlowMotionRatio;
import StatusBar;

using std::setprecision;
using std::fixed;
using std::setw;
using std::setfill;
using std::to_wstring;
using std::make_unique;
using namespace std::chrono;

/////// inner class SlowMotionDisplay::RefreshRate ///////

SlowMotionDisplay::RefreshRate::RefreshRate	
(
	StatusBar             *pStatusBar,
	int             const  iPartInStatusBar,
	SlowMotionRatio const &slowMotionRatio
)
  :	StatusBarRefreshRate(pStatusBar, iPartInStatusBar),
	m_slowMotionRatio(slowMotionRatio)
{ }

void SlowMotionDisplay::RefreshRate::Trigger(bool const bImmediately)
{
	float const fMeasured { m_slowMotionRatio.GetMeasuredSlowMo() };
	float const fNominal  { m_slowMotionRatio.GetNominalSlowMo() };

	m_wstrBuffer.str(L"");
	m_wstrBuffer.clear();
	m_wstrBuffer << L"slowmo ";

	if (fMeasured == 0.0f)
		m_wstrBuffer << L"  -  ";
	else
		m_wstrBuffer << fixed << setprecision(1) << setw(6) << fMeasured;

	m_wstrBuffer << L"/";

	if (m_slowMotionRatio.MaxSpeed())
		m_wstrBuffer << L"max";
	else
		m_wstrBuffer << fixed << setprecision(0) << fNominal;

	m_pStatusBar->DisplayInPart(m_iPartInStatusBar, m_wstrBuffer.str());
}

/////// functions of class SlowMotionDisplay ///////

SlowMotionDisplay::~SlowMotionDisplay() = default;

void SlowMotionDisplay::Initialize
(
	StatusBar             * pStatusBar,
	int                     iPartInStatusBar,
	SlowMotionRatio const & slowMotionRatio
) 
{
	m_upRefreshRate = make_unique<RefreshRate>
	(
		pStatusBar,
		iPartInStatusBar,
		slowMotionRatio
	);
	m_upRefreshRate->SetRefreshRate(300ms);
	pStatusBar->AddCustomControl(16);            // nr of characters 

    HFONT hFont = CreateFontA
    (
        15,  //[in] int    cHeight,
        0,            //[in] int    cWidth,
        0,            //[in] int    cEscapement,
        0,            //[in] int    cOrientation,
        0,            //[in] int    cWeight,
        0,            //[in] DWORD  bItalic,
        0,            //[in] DWORD  bUnderline,
        0,            //[in] DWORD  bStrikeOut,
        0,            //[in] DWORD  iCharSet,
        0,            //[in] DWORD  iOutPrecision,
        0,            //[in] DWORD  iClipPrecision,
        0,            //[in] DWORD  iQuality,
        FIXED_PITCH,  //[in] DWORD  iPitchAndFamily,
        nullptr       //[in] LPCSTR pszFaceName
    );

    pStatusBar->SendMessage(WM_SETFONT, (WPARAM)hFont, true);
}

void SlowMotionDisplay::Notify(bool const bImmediately)
{
	if (m_upRefreshRate)
		m_upRefreshRate->Notify(bImmediately);
}
