// win32_viewCollection.h
//

#pragma once

#include <vector>
#include "ObserverInterface.h"
#include "win32_rootWindow.h"

class ViewCollection : public ObserverInterface
{
public:
    ViewCollection( )
	  : m_event( CreateEvent( nullptr, FALSE, FALSE, nullptr ) )
	{ }

	~ViewCollection( )
	{
		(void)CloseHandle( m_event );
		m_event = nullptr;
	}

	virtual void Notify( bool const bWait = false )
	{
		for ( auto &v : m_aView )
		{
			v->Notify();
		}
		if ( bWait )
		{
			(void)ResetEvent( m_event );
			(void)WaitForSingleObject( m_event, INFINITE );
		}
	}

	ObserverInterface * AttachObserver( RootWindow * pRootWin )
	{
		assert( pRootWin != nullptr );
		ObserverInterface * po = pRootWin->GetObserver( );
		m_aView.push_back( po );
		return po;
	}

    void Continue( ) const
    {
        (void)SetEvent( m_event );
    }

private:
    HANDLE m_event;
    std::vector< ObserverInterface * > m_aView;
};
