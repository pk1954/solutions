// win32_thread.h : 
//

#pragma once

#include "string"
#include "assert.h"
#include "win32_event.h"

using std::wstring;

namespace Util
{
	class Thread
	{
	public:
		void StartThread( wstring const & strName );

		void SetThreadAffinityMask( DWORD_PTR mask )
		{
			::SetThreadAffinityMask( m_handle, mask );
		}

		void PostThreadMsg( MSG msg )
		{
			assert( m_threadId != 0 );
		    BOOL const bRes = ::PostThreadMessage( m_threadId, msg.message, msg.wParam, msg.lParam );
			DWORD err = GetLastError( );
			if ( !bRes )
			{
				if ( err == ERROR_INVALID_THREAD_ID )
				{
					int y = 76;
				}
				int x = 42;
			}
			assert( bRes );
		}

		void PostThreadMsg( unsigned int uiMsg )
		{
			PostThreadMsg( MSG{ nullptr, uiMsg, 0, 0 } );
		}

		void Terminate( ); // Waits until thread has stopped

		virtual void ThreadStartupFunc( ) {};
		virtual void ThreadMsgDispatcher( MSG const ) = 0;
		virtual void ThreadShutDownFunc( ) {};

	private:
		Event   m_eventThreadStarter;
		HANDLE  m_handle;
		UINT    m_threadId;
		wstring m_strThreadName;

		friend static unsigned int __stdcall ThreadProc( void * );
	};
};
