// win32_displayAll.cpp
//

#include "stdafx.h"
#include "win32_winManager.h"
#include "win32_displayAll.h"

// DisplayAll is a callback functor
// It triggers all observers and waits for user input.

DisplayAll::DisplayAll( )
  : m_event( CreateEvent( nullptr, FALSE, FALSE, nullptr ) ),
    m_pWinManager( nullptr )
{ }

DisplayAll::~DisplayAll( )
{
    (void)CloseHandle( m_event );
    m_event = nullptr;
}

bool DisplayAll::operator() ( bool const bWait ) const
{
    assert( m_pWinManager != nullptr );
    m_pWinManager->NotifyObservers( );
    if ( bWait )
    {
        (void)ResetEvent( m_event );
        (void)WaitForSingleObject( m_event, INFINITE );
    }
    return true;
}
