// observerInterface.cpp
//

#include "stdafx.h"
#include "observerInterface.h"

ObserverInterface::ObserverInterface( HWND hWnd, INT iMilliSecs ) :
        m_hWnd( hWnd ),
        m_hTimer( nullptr ),
        m_iDisplayRate( iMilliSecs ),  // minimum delay between Invalidate calls 
        m_bTimerActive( FALSE ),
        m_bDirty( TRUE )
{
    SetDirtyFlag( );
}

ObserverInterface::~ObserverInterface( )
{
    m_hWnd = nullptr;
    m_hTimer = nullptr;
}

void ObserverInterface::SetDisplayRate( INT iRate )
{
    m_iDisplayRate = iRate;
    invalidate( );
}

void ObserverInterface::SetDirtyFlag( )
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

void CALLBACK ObserverInterface::TimerProc( void * const lpParameter, BOOL const TimerOrWaitFired )
{
    ObserverInterface * const pOI = static_cast<ObserverInterface * >( lpParameter );
    if ( pOI->m_bDirty )
    {
        pOI->invalidate( );
    }
    else
    {
        if ( pOI->m_hTimer != nullptr )
        {
            (void)DeleteTimerQueueTimer( nullptr, pOI->m_hTimer, 0 );
            pOI->m_hTimer = nullptr;
        }
        pOI->m_bTimerActive = FALSE;
    }
}

void ObserverInterface::invalidate( )
{
    InvalidateRect( m_hWnd, nullptr, FALSE );
    m_bDirty = FALSE;
}

void ObserverInterface::startTimer( )
{
    void * const pVoid = static_cast<void *>( this );
    (void)CreateTimerQueueTimer( &m_hTimer, nullptr, (WAITORTIMERCALLBACK)TimerProc, pVoid, (DWORD)m_iDisplayRate, (DWORD)m_iDisplayRate, 0 );
}
