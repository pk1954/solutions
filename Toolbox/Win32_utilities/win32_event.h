// win32_event.h
//

#pragma once

#include "Windows.h"
#include "EventInterface.h"

namespace Util
{
	class Event : public EventInterface
	{
	public:
		Event( )
		  : m_eventHandle
		    ( 
				CreateEvent
				( 
					nullptr, // no security attributes
					true,    // manual reset event 
					false,   // initial state nonsignaled
					nullptr  // no name for event
				) 
			)
		{ }

		~Event( )
		{
			(void)CloseHandle( m_eventHandle );
			m_eventHandle = nullptr;
		}

		virtual void Wait( )
		{
			(void)ResetEvent( m_eventHandle );
			(void)WaitForSingleObject( m_eventHandle, INFINITE );
		}

		virtual void Continue( )
		{
			(void)SetEvent( m_eventHandle );
		}

	private:
		HANDLE m_eventHandle;
	};
};
