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
		void StartThread( wstring const &, BOOL const );

		void SetThreadAffinityMask( DWORD_PTR mask )
		{
			::SetThreadAffinityMask( m_handle, mask );
		}

		void PostThreadMsg( UINT uiMsg, WPARAM const wParam = 0, LPARAM const lParam = 0 )
		{
			if ( m_bAsync )
			{
				assert( m_threadId != 0 );
				BOOL const bRes = ::PostThreadMessage( m_threadId, uiMsg, wParam, lParam );
				DWORD err = GetLastError( );
				assert( bRes );
			}
			else
			{
				ThreadMsgDispatcher( MSG{ nullptr, uiMsg, wParam, lParam } );
			}
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
		BOOL    m_bAsync;

		friend static unsigned int __stdcall ThreadProc( void * );
	};
};
