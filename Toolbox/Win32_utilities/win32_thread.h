// win32_thread.h : 
//
// Win32_utilities

#pragma once

#include "string"
#include "assert.h"
#include "win32_event.h"

using std::wstring;

namespace Util
{
	HANDLE RunAsAsyncThread( unsigned int __stdcall ( void * ), void *, UINT * = nullptr );

	class Thread
	{
	public:

		~Thread() {};

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
				ThreadMsgDispatcher( MSG { nullptr, uiMsg, wParam, lParam } );
			}
		}

		void Terminate( ); // Waits until thread has stopped

		BOOL IsAsyncThread( ) { return m_bAsync; }

		virtual void ThreadStartupFunc( ) {};
		virtual void ThreadMsgDispatcher( MSG const ) = 0;
		virtual void ThreadShutDownFunc( ) {};

	private:
		HANDLE  m_handle   { nullptr };
		UINT    m_threadId { 0 };
		BOOL    m_bAsync   { false };
		Event   m_eventThreadStarter { };
		wstring m_strThreadName { };

		friend static unsigned int __stdcall ThreadProc( void * );
	};
};
