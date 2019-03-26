// win32_rootWindow.cpp
//

#include "stdafx.h"
#include "win32_rootWindow.h"

BOOL RootWinIsReady( RootWindow const * pRootWin )
{
    return ( ( pRootWin != nullptr ) && ( pRootWin->GetWindowHandle( ) != nullptr ) );
}

RootWindow::RootWindow( ) : 
	m_hwnd( nullptr ),
	m_hwndApp( nullptr ),
	m_msRefreshRate( 0ms ),
    m_hTimer( nullptr ),
    m_bTimerActive( FALSE ),
    m_bDirty( TRUE )
{ }

RootWindow::~RootWindow( ) 
{ 
	deleteTimer( );
	m_hwnd   = nullptr; 
    m_hTimer = nullptr;
}

void RootWindow::SetWindowHandle( HWND const hwnd ) 
{ 
	m_hwnd    = hwnd;  
	m_hwndApp = GetAncestor( m_hwnd, GA_ROOTOWNER );
};

void RootWindow::Notify( bool const bImmediately )
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

void CALLBACK RootWindow::TimerProc( void * const lpParam, BOOL const TimerOrWaitFired )
{
    RootWindow * const pRootWin = reinterpret_cast<RootWindow *>( lpParam );
    if ( pRootWin->m_bDirty )
    {
        pRootWin->invalidate( );
    }
    else
    {
		pRootWin->deleteTimer( );
		pRootWin->m_bTimerActive = FALSE;
    }
}

void RootWindow::startTimer( milliseconds const msTimer )
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
