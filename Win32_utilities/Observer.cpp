// Observer.cpp
//

#include "stdafx.h"
#include "Observer.h"

Observer::Observer( RootWindow * pRootWindow ) :
        m_pRootWindow( pRootWindow ),
        m_hTimer( nullptr ),
        m_bTimerActive( FALSE ),
        m_bDirty( TRUE )
{
	Trigger( false ); 
}

Observer::~Observer( )
{
    m_pRootWindow = nullptr;
    m_hTimer      = nullptr;
}

void Observer::Trigger( bool const bWait )
{
	DWORD dwTime = static_cast<DWORD>( m_pRootWindow->GetDisplayRate() );

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

void CALLBACK Observer::TimerProc( void * const lpParameter, BOOL const TimerOrWaitFired )
{
    Observer * const pObserver = static_cast<Observer * >( lpParameter );
    if ( pObserver->m_bDirty )
    {
        pObserver->invalidate( );
    }
    else
    {
        if ( pObserver->m_hTimer != nullptr )
        {
			HANDLE handle = pObserver->m_hTimer;
            pObserver->m_hTimer = nullptr;
            (void)DeleteTimerQueueTimer( nullptr, handle, 0 );
        }
        pObserver->m_bTimerActive = FALSE;
    }
}

void Observer::invalidate( )
{
	if ( m_pRootWindow != nullptr )
		m_pRootWindow->Invalidate( FALSE );
    m_bDirty = FALSE;
}

void Observer::startTimer( DWORD const dwTime )
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
