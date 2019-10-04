// SpeedDisplay.cpp
//
// NNetSimu

#include "stdafx.h"
#include "util.h"
#include "win32_baseRefreshRate.h"
#include "SlowMotionRatio.h"
#include "win32_status.h"
#include "SpeedDisplay.h"

using std::to_wstring;
using namespace std::chrono;

/////// inner class SpeedDisplay::RefreshRate ///////

class SpeedDisplay::RefreshRate : public BaseRefreshRate
{
public:
	RefreshRate	
	(
		StatusBar       * pStatusBar,
		SlowMotionRatio * pSlowMotionRatio,
		int               iPartInStatusBar
	)
	  :	m_pStatusBar      (pStatusBar),
		m_pSlowMotionRatio(pSlowMotionRatio),
		m_iPartInStatusBar(iPartInStatusBar)
	{}

	virtual void Trigger( )
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
	int               m_iPartInStatusBar;
	SlowMotionRatio * m_pSlowMotionRatio;
};

/////// functions of class TimeDisplay ///////

SpeedDisplay::SpeedDisplay
(
	StatusBar       * pStatusBar,
	SlowMotionRatio * pSlowMotionRatio,
	int               iPartInStatusBar
) 
  :	m_pRefreshRate( nullptr )
{
	static PIXEL const PIX_WIDTH = PIXEL( 9 ) * 18;  // TODO: avoid magic numbers
	m_pRefreshRate = new RefreshRate
	( 
		pStatusBar,
		pSlowMotionRatio,
		iPartInStatusBar
	);
	m_pRefreshRate->SetRefreshRate( 300ms );
	pSlowMotionRatio->RegisterObserver( this );    // notify me, if model has changed
	pStatusBar->AddCustomControl( PIX_WIDTH ); 
}

SpeedDisplay::~SpeedDisplay( )
{
	delete m_pRefreshRate;
	m_pRefreshRate = nullptr;
}

void SpeedDisplay::Notify( bool const bImmediately )
{
	m_pRefreshRate->Notify( bImmediately );
}
