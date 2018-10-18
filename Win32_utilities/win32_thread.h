// win32_thread.h : 
//

#pragma once

#include "string"
#include "assert.h"
#include "win32_event.h"

using namespace std;

namespace Util
{
	class Thread
	{
	public:
		void StartThread( BOOL const, wstring const & strName );

		void SetThreadAffinityMask( DWORD_PTR mask )
		{
			::SetThreadAffinityMask( m_handle, mask );
		}

		void Continue( )
		{
			m_eventThreadStarter.Continue();   // trigger waiting thread to continue
		}

		void PostThreadMsg( UINT uiMsg, WPARAM wParam, LPARAM lParam )
		{
			assert( m_threadId != 0 );
		    BOOL const bRes = ::PostThreadMessage( m_threadId, uiMsg, wParam, lParam );
			DWORD err = GetLastError( );
			assert( bRes );
		}

		void Terminate( );

		virtual void ThreadStartupFunc( ) = 0;
		virtual void ThreadMsgDispatcher( MSG const msg )
		{
			(void)TranslateMessage( &msg );
			(void)DispatchMessage( &msg );
		}

	private:
		Event   m_eventThreadStarter;
		HANDLE  m_handle;
		UINT    m_threadId;
		BOOL    m_bLoop;
		wstring m_strThreadName;

		friend static unsigned int __stdcall ThreadProc( void * );
	};
};
