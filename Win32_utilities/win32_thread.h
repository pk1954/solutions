// win32_thread.h : 
//

#pragma once

#include "assert.h"
#include <Windows.h>
#include "win32_event.h"

using namespace std;

namespace Util
{
	class Thread
	{
	public:
		void StartThread( );

		void SetThreadAffinityMask( DWORD_PTR );

		void PostMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
		{
		    BOOL const bRes = PostThreadMessage( m_threadId, uiMsg, wParam, lParam );
			DWORD err = GetLastError( );
			assert( bRes );
		}

		void Terminate( );

	protected:
		virtual LRESULT DispatchThreadMsg( UINT const, WPARAM const, LPARAM const ) = 0;

		enum ThreadMessage : UINT
		{
			THREAD_MSG_EXIT = WM_USER + 1,
			THREAD_MSG_APP_FIRST    // messages of application start here
		};

	private:
		Event  m_eventThreadStarter;
		HANDLE m_handle;
		UINT   m_threadId;

		friend static unsigned int __stdcall ThreadProc( void * );
	};
};
