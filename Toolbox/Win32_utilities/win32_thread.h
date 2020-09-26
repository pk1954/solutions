// win32_thread.h : 
//
// Win32_utilities

#pragma once

#include "string"
#include <assert.h>
#include "win32_event.h"

using std::wstring;

namespace Util
{
	HANDLE RunAsAsyncThread( unsigned int __stdcall ( void * ), void *, UINT * = nullptr );

	class Thread
	{
	public:

		~Thread() {};

		void BeginThread( wstring const & );
		void StartThread( wstring const &, bool const );

		void SetThreadAffinityMask( DWORD_PTR mask )
		{
			::SetThreadAffinityMask( m_handle, mask );
		}

		void PostThreadMsg( UINT uiMsg, WPARAM const wParam = 0, LPARAM const lParam = 0 )
		{
			if ( m_bAsync )
			{
				assert( m_threadId != 0 );
				bool const bRes = ::PostThreadMessage( m_threadId, uiMsg, wParam, lParam );
//				DWORD err = GetLastError( );
				assert( bRes );
			}
			else
			{
				ThreadMsgDispatcher( MSG { nullptr, uiMsg, wParam, lParam } );
			}
		}

		void Terminate( ); // Waits until thread has stopped
		void TerminateNoWait( ) { PostThreadMessage( m_threadId, WM_QUIT, 0, 0 ); }// PostQuitMessage( 0 );  doesn't work

		bool IsAsyncThread( ) { return m_bAsync; }

		virtual void ThreadStartupFunc( ) {};
		virtual void ThreadMsgDispatcher( MSG const ) = 0;
		virtual void ThreadShutDownFunc( ) {};

	private:
		HANDLE  m_handle   { nullptr };
		UINT    m_threadId { 0 };
		bool    m_bAsync   { false };
		Event   m_eventThreadStarter { };
		wstring m_strThreadName { };

		friend static unsigned int __stdcall ThreadProc( void * );
	};
};
