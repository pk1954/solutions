// TimeDisplay.cpp
//
// NNetSimu
//
// Displays time since start of simulation in status bar field

#include "stdafx.h"
#include <iostream>
#include <sstream> 
#include <string> 
#include "win32_baseRefreshRate.h"
#include "win32_status.h"
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
		m_iPartInStatusBar     (iPartInStatusBar),
		m_SRWLock              ( )
	{ 
		InitializeSRWLock( & m_SRWLock );
	}

	virtual void Trigger( )
	{
		AcquireSRWLockExclusive( & m_SRWLock );
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
		ReleaseSRWLockExclusive( & m_SRWLock ); 
	}

private:
	SRWLOCK                          m_SRWLock;
	wstring                          m_wstring;
	wostringstream                   m_wstrBuffer;
	StatusBar                      * m_pStatusBar;
	NNetModelReaderInterface const * m_pModelReaderInterface;
	int                              m_iPartInStatusBar;
};

/////// functions of class TimeDisplay ///////

using namespace std::chrono;

TimeDisplay::TimeDisplay
(
	StatusBar                      * pStatusBar,
	NNetModelReaderInterface const * pModelInterface,
	int                              iPartInStatusBar
)
  :	m_pRefreshRate( nullptr )
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

TimeDisplay::~TimeDisplay( )
{
	delete m_pRefreshRate;
	m_pRefreshRate = nullptr;
}

void TimeDisplay::Notify( bool const bImmediately )
{
	m_pRefreshRate->Notify( bImmediately );
}
