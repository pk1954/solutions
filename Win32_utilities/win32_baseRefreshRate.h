// win32_baseRefreshRate.h : 
//
// Win32_utilities

#pragma once

#include <chrono>
#include "win32_util.h"

using std::chrono::milliseconds;
using namespace std::chrono;

class BaseRefreshRate
{
public:

	BaseRefreshRate( );
	~BaseRefreshRate( );

	virtual void Trigger( ) = 0;

	void SetRefreshRate( milliseconds const );
	milliseconds GetRefreshRate( );

	void Notify( bool const );

	void RefreshRateDialog( HWND const );

private:
	HANDLE       m_hTimer;
	milliseconds m_msRefreshRate;
	BOOL         m_bTimerActive;
	BOOL         m_bDirty;

	void startTimer( milliseconds const );
	void deleteTimer( );
	void trigger( )
	{
		Trigger( );
		m_bDirty = FALSE;
	}

	static void CALLBACK TimerProc( void * const, BOOL const );
};
