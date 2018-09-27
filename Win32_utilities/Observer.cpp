// observerInterface.cpp
//

#include "stdafx.h"
#include "Observer.h"

Observer::Observer( HWND hWnd, INT iMilliSecs ) :
        m_hWnd( hWnd ),
        m_hTimer( nullptr ),
        m_iDisplayRate( iMilliSecs ),  // minimum delay between Invalidate calls 
        m_bTimerActive( FALSE ),
        m_bDirty( TRUE )
{
    SetDirtyFlag( );
}

Observer::~Observer( )
{
    m_hWnd = nullptr;
    m_hTimer = nullptr;
}

void Observer::SetDisplayRate( INT iRate )
{
    m_iDisplayRate = iRate;
    invalidate( );
}

void Observer::SetDirtyFlag( )
{
    if ( m_iDisplayRate == 0 )
        invalidate( );
    else
    {
        m_bDirty = TRUE;
        if ( !m_bTimerActive )
        {
            m_bTimerActive = TRUE;
            invalidate( );
            startTimer( );
        }
    }
}

void CALLBACK Observer::TimerProc( void * const lpParameter, BOOL const TimerOrWaitFired )
{
    Observer * const pOI = static_cast<Observer * >( lpParameter );
    if ( pOI->m_bDirty )
    {
        pOI->invalidate( );
    }
    else
    {
        if ( pOI->m_hTimer != nullptr )
        {
			HANDLE handle = pOI->m_hTimer;
            pOI->m_hTimer = nullptr;
            (void)DeleteTimerQueueTimer( nullptr, handle, 0 );
        }
        pOI->m_bTimerActive = FALSE;
    }
}

void Observer::invalidate( )
{
	assert( m_hWnd != nullptr );
    InvalidateRect( m_hWnd, nullptr, FALSE );
    m_bDirty = FALSE;
}

void Observer::startTimer( )
{
    void * const pVoid = static_cast<void *>( this );
    (void)CreateTimerQueueTimer( &m_hTimer, nullptr, (WAITORTIMERCALLBACK)TimerProc, pVoid, (DWORD)m_iDisplayRate, (DWORD)m_iDisplayRate, 0 );
}
