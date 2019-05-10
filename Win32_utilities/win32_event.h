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
		  : m_event
		    ( 
				CreateEvent
				( 
					nullptr, // no security attributes
					TRUE,    // manual reset event 
					FALSE,   // intial state nonsignaled
					nullptr  // no name for event
				) 
			)
		{ }

		~Event( )
		{
			(void)CloseHandle( m_event );
			m_event = nullptr;
		}

		virtual void Wait( )
		{
			(void)ResetEvent( m_event );
			(void)WaitForSingleObject( m_event, INFINITE );
		}

		virtual void Continue( )
		{
			(void)SetEvent( m_event );
		}

	private:
		HANDLE m_event;
	};
};
