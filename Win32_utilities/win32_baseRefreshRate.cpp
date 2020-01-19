// win32_refreshRate.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "win32_stdDialogBox.h"
#include "win32_baseRefreshRate.h"

using namespace std::chrono;
using std::wstring;
using std::to_wstring;

BaseRefreshRate::BaseRefreshRate( )
 :	m_hTimer( nullptr ),
	m_bTimerActive( FALSE ),
	m_bDirty( TRUE ),
	m_msRefreshRate( 0ms )
{}

BaseRefreshRate::~BaseRefreshRate( )
{
	deleteTimer( );
	m_hTimer = nullptr;
}

void BaseRefreshRate::SetRefreshRate( milliseconds const msRate ) 
{ 
	m_msRefreshRate = msRate; 
	deleteTimer( );
}

milliseconds BaseRefreshRate::GetRefreshRate( )
{ 
	return m_msRefreshRate; 
}

void BaseRefreshRate::Notify( bool const bImmediately )
{
	if ( bImmediately || (m_msRefreshRate == 0ms) )
		trigger( );
	else
	{
		m_bDirty = TRUE;
		if ( !m_bTimerActive )
		{
			m_bTimerActive = TRUE;
			trigger( );
			startTimer( m_msRefreshRate );
		}
	}
}

void BaseRefreshRate::RefreshRateDialog( HWND const hwnd )
{
	static double MIN_REFRESH_RATE { 20.0 };

	double dNewValue = StdDialogBox::Show
	( 
		hwnd,
		static_cast<float>( GetRefreshRate( ).count() ),
		L"Refresh Rate",
		L"milliseconds"
	);
	if ( dNewValue < MIN_REFRESH_RATE )
	{
		wstring text { L"Minimum refresh rate is " + to_wstring(MIN_REFRESH_RATE) + L" ms" };
		MessageBox( nullptr, text.c_str(), NULL, MB_OK );
		dNewValue = MIN_REFRESH_RATE;
	}
	SetRefreshRate( static_cast<milliseconds>(static_cast<long long>(dNewValue)) );
}

void BaseRefreshRate::startTimer( milliseconds const msTimer )
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

void BaseRefreshRate::deleteTimer( )
{
	if ( m_hTimer != nullptr )
	{
		HANDLE handle = m_hTimer;
		m_hTimer = nullptr;
		(void)DeleteTimerQueueTimer( nullptr, handle, INVALID_HANDLE_VALUE );
	}
	m_bTimerActive = FALSE;
}

void CALLBACK BaseRefreshRate::TimerProc( void * const lpParam, BOOL const TimerOrWaitFired )
{
	BaseRefreshRate * const pRefreshRate = reinterpret_cast<BaseRefreshRate *>( lpParam );
	if ( pRefreshRate->m_bDirty )
		pRefreshRate->trigger( );
	else
		pRefreshRate->deleteTimer( );
}
