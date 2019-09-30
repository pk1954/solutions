// win32_refreshRate.cpp
//
// NNetSimu

#include "stdafx.h"
#include "TimeDisplay.h"

class TimeDisplay::TimeDisplayRefreshRate : public RefreshRate
{
public:
	TimeDisplayRefreshRate	
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
		NNetModel    const * pCore = m_pReadBuffer->LockReadBuffer( );
		microseconds const   time  = pCore->GetSimulationTime( );
		m_pReadBuffer->ReleaseReadBuffer( );
		if ( time > std::chrono::seconds( 1 ) )
		{
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>( time );
			m_wstrBuffer = DecFraction2( CastToUnsignedLong( ms.count() ) ) + L" s";
		}
		else
		{
			m_wstrBuffer = DecFraction2( CastToUnsignedLong( time.count() ) ) + L" ms";
		}
		m_pStatusBar->DisplayInPart( m_iPartInStatusBar, m_wstrBuffer );
	}

private:
	wstring          m_wstrBuffer;
	StatusBar      * m_pStatusBar;
	NNetReadBuffer * m_pReadBuffer;
	int              m_iPartInStatusBar;
};

TimeDisplay::TimeDisplay
(
	StatusBar      * pStatusBar,
	NNetReadBuffer * pReadBuffer,
	int              iPartInStatusBar
)
	:	m_pRefreshRate( nullptr )
{
	static PIXEL const PIX_WIDTH = PIXEL( 9 ) * 8;   //TODO: avoid magic numbers
	m_pRefreshRate = new TimeDisplayRefreshRate
	( 
		pStatusBar,
		pReadBuffer,
		iPartInStatusBar
	);
	m_pRefreshRate->SetRefreshRate( 300ms );
	pReadBuffer->RegisterObserver( this );         // notify me, if model has changed
	pStatusBar->AddCustomControl( PIX_WIDTH ); 
	Notify( false );
}

void TimeDisplay::Notify( bool const bImmediately )
{
	m_pRefreshRate->Notify( bImmediately );
}
