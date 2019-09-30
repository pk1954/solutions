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

	RefreshRate( );
	~RefreshRate( );

	virtual void Trigger( ) = 0;

	void SetRefreshRate( milliseconds const );
	milliseconds GetRefreshRate( );

	void Notify( bool const );

private:
	HANDLE       m_hTimer;
	milliseconds m_msRefreshRate;
	BOOL         m_bTimerActive;
	BOOL         m_bDirty;

	void startTimer( milliseconds const );
	void deleteTimer( );
	void invalidate( );

	static void CALLBACK TimerProc( void * const, BOOL const );
};
