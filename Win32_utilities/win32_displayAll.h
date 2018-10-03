// win32_displayAll.h
//

#pragma once

#include "DisplayFunctor.h"
#include "win32_viewCollection.h"

// DisplayAll is a callback functor
// It triggers display of all observers and waits for user input.

class DisplayAll : public DisplayFunctor
{
public:
	DisplayAll( )
	  : m_event( CreateEvent( nullptr, FALSE, FALSE, nullptr ) )
	{ }

	~DisplayAll( )
	{
		(void)CloseHandle( m_event );
		m_event = nullptr;
	}

	void Trigger( bool const bWait ) const
	{
		m_ViewCollection.NotifyObservers( );
		if ( bWait )
		{
			(void)ResetEvent( m_event );
			(void)WaitForSingleObject( m_event, INFINITE );
		}
	}

	void AttachObserver( RootWindow * pRootWin )
	{
		m_ViewCollection.AttachObserver( pRootWin );
	}

    void Continue( ) const
    {
        (void)SetEvent( m_event );
    }

private:
    HANDLE         m_event;
    ViewCollection m_ViewCollection;
};

