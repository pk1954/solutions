// SpeedDisplay.h
//
// NNetSimu

#pragma once

#include "util.h"
#include "ObserverInterface.h"
#include "SlowMotionRatio.h"
#include "win32_status.h"

using std::to_wstring;

class SpeedDisplay : public ObserverInterface
{
public:
	SpeedDisplay
	(
		StatusBar       * pStatusBar,
		SlowMotionRatio * pSlowMotionRatio,
		int               iPartInStatusBar
	) 
	  :	m_pStatusBar      (pStatusBar),
		m_pSlowMotionRatio(pSlowMotionRatio),
		m_iPartInStatusBar(iPartInStatusBar)
	{
		static PIXEL const PIX_WIDTH = PIXEL( 9 ) * 18;  // TODO: avoid magic numbers
		m_pSlowMotionRatio->RegisterObserver( this );    // notify me, if model has changed
		m_pStatusBar->AddCustomControl( PIX_WIDTH ); 
		Notify( false );
	}

	virtual void Notify( bool const bImmediately )
	{
		unsigned int const uiRatio = m_pSlowMotionRatio->GetRatio( );
		if ( uiRatio == 1 )
			m_wstrBuffer = L"real time";
		else
			m_wstrBuffer = L"slow motion " + to_wstring(uiRatio) + L" X ";
		m_pStatusBar->DisplayInPart( m_iPartInStatusBar, m_wstrBuffer );
	}

private:
	wstring           m_wstrBuffer;
	StatusBar       * m_pStatusBar;
	SlowMotionRatio * m_pSlowMotionRatio;
	int               m_iPartInStatusBar;
};

