// Observer.h : 
//

#pragma once

#include "observerInterface.h"
#include "win32_rootWindow.h"

class Observer : public ObserverInterface
{
public:
    Observer( RootWindow * );

    virtual ~Observer( );

    virtual void Notify( bool const );

    HWND const GetWindowHandle( ) 
	{ 
		return m_pRootWindow->GetWindowHandle(); 
	}

private:

    static void CALLBACK TimerProc( void * const, BOOL const );

    void invalidate( );
    void startTimer( DWORD const );

    RootWindow * m_pRootWindow;
    HANDLE       m_hTimer;
    BOOL         m_bTimerActive;
    BOOL         m_bDirty;
};
