// observerInterface.h : 
//

#pragma once

class ObserverInterface
{
public:
    ObserverInterface( HWND, INT );

    virtual ~ObserverInterface( );

    void SetDisplayRate( INT );
    void SetDirtyFlag( );

    HWND const GetWindowHandle( ) { return m_hWnd; }

private:

    static void CALLBACK TimerProc( void * const, BOOL const );

    void invalidate( );
    void startTimer( );

    HWND   m_hWnd;
    HANDLE m_hTimer;
    INT    m_iDisplayRate; // in milliseconds
    BOOL   m_bTimerActive;
    BOOL   m_bDirty;
};
