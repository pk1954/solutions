// TimeDisplay.h
//
// NNetSIme

#pragma once

#include "util.h"
#include "ObserverInterface.h"
#include "win32_status.h"
#include "NNetReadBuffer.h"
#include "NNetModel.h"

using std::to_wstring;

class TimeDisplay : public ObserverInterface
{
public:
	TimeDisplay
	(
		StatusBar      * pStatusBar,
		NNetReadBuffer * pReadBuffer,
		int              iPartInStatusBar
	)
	  :	m_pStatusBar      (pStatusBar),
		m_pReadBuffer     (pReadBuffer),
		m_iPartInStatusBar(iPartInStatusBar)
	{
		static PIXEL const PIX_WIDTH = PIXEL( 9 ) * 8;   //TODO: avoid magic numbers
		m_pReadBuffer->RegisterObserver( this );         // notify me, if model has changed
		m_pStatusBar->AddCustomControl( PIX_WIDTH ); 
		Notify( false );
	}

	virtual void Notify( bool const bImmediately )
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

