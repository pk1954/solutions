// TimeDisplay.cpp
//
// NNetSimu
//
// Displays time since start of simulation in status bar field

#include "stdafx.h"
#include <iostream>
#include <sstream> 
#include <string> 
#include "win32_status.h"
#include "win32_baseRefreshRate.h"
#include "NNetModelReaderInterface.h"
#include "TimeDisplay.h"

using std::to_wstring;
using std::wstring;
using std::wostringstream;

/////// inner class TimeDisplay::RefreshRate ///////

class TimeDisplay::RefreshRate : public BaseRefreshRate
{
public:
	RefreshRate	
	(
		StatusBar                      * pStatusBar,
		NNetModelReaderInterface const * pModelInterface,
		int                              iPartInStatusBar
	)
	:	m_pStatusBar           (pStatusBar),
		m_pModelReaderInterface(pModelInterface),
		m_iPartInStatusBar     (iPartInStatusBar)
	{ 
	}

	virtual void Trigger( )
	{
		fMicroSecs const time = m_pModelReaderInterface->GetSimulationTime( );
		m_wstrBuffer.str( wstring() );
		m_wstrBuffer.clear();
		m_wstrBuffer << std::fixed << std::setprecision(2);
		if ( time > 1.e6_MicroSecs )  // more than one second
		{
			float seconds = CastToFloat( time.GetValue() ) / 1000000.0f;
			m_wstrBuffer << seconds << L" s";
		}
		else
		{
			float millisecs = CastToFloat( time.GetValue() ) / 1000.0f;
			m_wstrBuffer << millisecs << L" ms";
		}
		m_wstring = m_wstrBuffer.str();
		m_pStatusBar->DisplayInPart( m_iPartInStatusBar, m_wstring );
	}

private:
	wstring                          m_wstring               { };
	wostringstream                   m_wstrBuffer            { };
	StatusBar                      * m_pStatusBar            { nullptr };
	NNetModelReaderInterface const * m_pModelReaderInterface { nullptr };
	int                              m_iPartInStatusBar      { -1 };
};

/////// functions of class TimeDisplay ///////

using namespace std::chrono;

void TimeDisplay::Start
(
	StatusBar                      * pStatusBar,
	NNetModelReaderInterface const * pModelInterface,
	int                              iPartInStatusBar
)
{
	m_pRefreshRate = new RefreshRate
	( 
		pStatusBar,
		pModelInterface,
		iPartInStatusBar
	);
	m_pRefreshRate->SetRefreshRate( 300ms );
	pStatusBar->AddCustomControl( 8 );        // nr of characters 
}

void TimeDisplay::Stop( )
{
	delete m_pRefreshRate;
	m_pRefreshRate = nullptr;
}

void TimeDisplay::Notify( bool const bImmediately )
{
	if ( m_pRefreshRate )
		m_pRefreshRate->Notify( bImmediately );
}
