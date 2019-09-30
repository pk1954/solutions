// win32_refreshRate.h : 
//
// Win32_utilities

#pragma once

#include <chrono>
#include "win32_util.h"

using std::chrono::milliseconds;
using namespace std::chrono;

class RefreshRate
{
public:

	RefreshRate( )
	 :	m_hTimer( nullptr ),
		m_bTimerActive( FALSE ),
		m_bDirty( TRUE ),
		m_msRefreshRate( 0ms )
	{}

	~RefreshRate( )
	{
		deleteTimer( );
		m_hTimer = nullptr;
	}

	virtual void Trigger( ) = 0;

	void SetRefreshRate( milliseconds const msRate ) 
	{ 
		m_msRefreshRate = msRate; 
	}

	milliseconds GetRefreshRate( )
	{ 
		return m_msRefreshRate; 
	}

	void Notify( bool const bImmediately )
	{
		if ( bImmediately || (m_msRefreshRate == 0ms) )
			invalidate( );
		else
		{
			m_bDirty = TRUE;
			if ( !m_bTimerActive )
			{
				m_bTimerActive = TRUE;
				invalidate( );
				startTimer( m_msRefreshRate );
			}
		}
	}

private:
	HANDLE       m_hTimer;
	milliseconds m_msRefreshRate;
	BOOL         m_bTimerActive;
	BOOL         m_bDirty;

	void startTimer( milliseconds const msTimer )
	{
		DWORD dwTime = static_cast<DWORD>(msTimer.count());
		(void)CreateTimerQueueTimer
		( 
			& m_hTimer,                     // output parameter 
			nullptr,                        // use default timer queue
			(WAITORTIMERCALLBACK)TimerProc, // the timer procedure
			static_cast<void *>( this ),    // pointer to this object as parameter to TimerProc
			dwTime,                         // timer is signaled the first time after dwTime msecs
			dwTime,                         // timer is signaled periodically every dwTime msecs
			0                               // no flags
		);
	}

	void deleteTimer( )
	{
		if ( m_hTimer != nullptr )
		{
			HANDLE handle = m_hTimer;
			m_hTimer = nullptr;
			(void)DeleteTimerQueueTimer( nullptr, handle, 0 );
		}
	}

	void invalidate( )
	{
		Trigger( );
		m_bDirty = FALSE;
	}

	static void CALLBACK TimerProc( void * const lpParam, BOOL const TimerOrWaitFired )
	{
		RefreshRate * const pRefreshRate = reinterpret_cast<RefreshRate *>( lpParam );
		if ( pRefreshRate->m_bDirty )
		{
			pRefreshRate->invalidate( );
		}
		else
		{
			pRefreshRate->deleteTimer( );
			pRefreshRate->m_bTimerActive = FALSE;
		}
	}
};