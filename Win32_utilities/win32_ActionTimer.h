// win32_actionTimer.h 
//
// win32_utilities

#pragma once

#include "windows.h"
#include "win32_hiResTimer.h"
#include "ViewCollection.h"

class ActionTimer
{
public:
	ActionTimer()
		: m_hrtimerSingleAction( ),
		  m_dwSingleActionTime(0  ),
		  m_hrtimerOverall( ),
		  m_dwCounter( 0 )
	{}

	void TimerStart( )
	{
		m_hrtimerSingleAction.Start( );
	};

	void TimerStop( )
	{
		m_hrtimerSingleAction.Stop( );
		m_dwSingleActionTime = m_hrtimerSingleAction.Get( );
		++m_dwCounter;
		m_observers.NotifyAll( false );
	};

	DWORD GetSingleActionTime( )
	{
		return m_dwSingleActionTime;
	}

	DWORD CalcFrequency( DWORD dwCount, DWORD dwMicroSecs )
	{
		return dwMicroSecs ? ( ( dwCount * 1000 * 1000 ) / dwMicroSecs ) : 0;
	}

	DWORD GetMeasuredPerformance( )
	{
		m_hrtimerOverall.Stop( );
		DWORD dwResult = CalcFrequency( m_dwCounter, m_hrtimerOverall.Get( ) ) * 1000;
		m_dwCounter = 0;
		m_hrtimerOverall.Start( );
		return dwResult;
	}

	void RegisterObserver( ObserverInterface * const pObserver )
	{
		m_observers.Register( pObserver );
	}

	void Stop( )
	{
		m_observers.Clear();
	}

private:
	HiResTimer     m_hrtimerSingleAction;
	HiResTimer     m_hrtimerOverall;
	DWORD          m_dwSingleActionTime;   // in microseconds
	DWORD          m_dwCounter;            // nr of executions
	ViewCollection m_observers;
};
