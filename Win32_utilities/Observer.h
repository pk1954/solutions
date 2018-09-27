// Observer.h : 
//

#pragma once

#include "observerInterface.h"

class Observer : public ObserverInterface
{
public:
    Observer( HWND, INT );

    virtual ~Observer( );

    virtual void SetDisplayRate( INT );
    virtual void SetDirtyFlag( );

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
