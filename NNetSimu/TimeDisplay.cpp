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
#include "NNetModel.h"
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
		StatusBar      * pStatusBar,
		NNetReadBuffer * pReadBuffer,
		int              iPartInStatusBar
	)
	:	m_pStatusBar      (pStatusBar),
		m_pReadBuffer     (pReadBuffer),
		m_iPartInStatusBar(iPartInStatusBar)
	{}

	virtual void Trigger( )
	{
		NNetModel const * pModel = m_pReadBuffer->GetModel( );
		MicroSecs const   time   = pModel->GetSimulationTime( );
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
	wstring          m_wstring;
	wostringstream   m_wstrBuffer;
	StatusBar      * m_pStatusBar;
	int              m_iPartInStatusBar;
	NNetReadBuffer * m_pReadBuffer;
};

/////// functions of class TimeDisplay ///////

using namespace std::chrono;

TimeDisplay::TimeDisplay
(
	StatusBar      * pStatusBar,
	NNetReadBuffer * pReadBuffer,
	int              iPartInStatusBar
)
  :	m_pRefreshRate( nullptr )
{
	static PIXEL const PIX_WIDTH = PIXEL( 9 ) * 8;   //TODO: avoid magic numbers
	m_pRefreshRate = new RefreshRate
	( 
		pStatusBar,
		pReadBuffer,
		iPartInStatusBar
	);
	m_pRefreshRate->SetRefreshRate( 300ms );
	pReadBuffer->RegisterObserver( this );         // notify me, if model has changed
	pStatusBar->AddCustomControl( PIX_WIDTH ); 
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
