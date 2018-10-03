// win32_rootWindow.cpp
//

#include "stdafx.h"
#include "Observer.h"
#include "win32_rootWindow.h"

BOOL RootWinIsReady( RootWindow const * pRootWin )
{
    return ( ( pRootWin != nullptr ) && ( pRootWin->GetWindowHandle( ) != nullptr ) );
}

RootWindow::RootWindow( ) : 
	m_hwnd( nullptr ),
	m_hwndApp( nullptr ),
	m_iDisplayRate( 0 ),
    m_hTimer( nullptr ),
    m_bTimerActive( FALSE ),
    m_bDirty( TRUE )
{ }

RootWindow::~RootWindow( ) 
{ 
	m_hwnd   = nullptr; 
    m_hTimer = nullptr;
}

void RootWindow::SetWindowHandle( HWND const hwnd ) 
{ 
	m_hwnd    = hwnd;  
	m_hwndApp = GetAncestor( m_hwnd, GA_ROOTOWNER );
};

void RootWindow::Notify( bool const bWait )
{
	DWORD dwTime = static_cast<DWORD>( m_iDisplayRate );

    if ( dwTime == 0 )
        invalidate( );
    else
    {
        m_bDirty = TRUE;
        if ( !m_bTimerActive )
        {
            m_bTimerActive = TRUE;
            invalidate( );
            startTimer( dwTime );
        }
    }
}

void CALLBACK RootWindow::TimerProc( void * const lpParameter, BOOL const TimerOrWaitFired )
{
    RootWindow * const pRootWin = static_cast<RootWindow * >( lpParameter );
    if ( pRootWin->m_bDirty )
    {
        pRootWin->invalidate( );
    }
    else
    {
        if ( pRootWin->m_hTimer != nullptr )
        {
			HANDLE handle = pRootWin->m_hTimer;
            pRootWin->m_hTimer = nullptr;
            (void)DeleteTimerQueueTimer( nullptr, handle, 0 );
        }
        pRootWin->m_bTimerActive = FALSE;
    }
}

void RootWindow::invalidate( )
{
	Invalidate( FALSE );
    m_bDirty = FALSE;
}

void RootWindow::startTimer( DWORD const dwTime )
{
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
