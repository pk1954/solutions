// win32_displayAll.h
//

#pragma once

#include "DisplayFunctor.h"
#include "win32_winManager.h"

// DisplayAll is a callback functor
// It triggers display of all observers and waits for user input.

class DisplayAll : public DisplayFunctor
{
public:
    explicit DisplayAll( );
    virtual ~DisplayAll( );

    virtual bool operator() ( bool const ) const;

    void SetWinManager( WinManager * pWinMan )
    {
        m_pWinManager = pWinMan;
    }

    void Continue( ) const
    {
        (void)SetEvent( m_event );
    }

private:
    HANDLE       m_event;
    WinManager * m_pWinManager;
};

