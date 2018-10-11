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
		void StartThread( BOOL const );

		void SetThreadAffinityMask( DWORD_PTR );

		void PostThreadMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
		{
		    BOOL const bRes = ::PostThreadMessage( m_threadId, uiMsg, wParam, lParam );
			DWORD err = GetLastError( );
			assert( bRes );
		}

		void Terminate( );

	protected:
		virtual void ThreadStartupFunc( ) {}
		virtual void ThreadMsgDispatcher( MSG const ) {};

	private:
		Event  m_eventThreadStarter;
		HANDLE m_handle;
		UINT   m_threadId;
		BOOL   m_bLoop;

		friend static unsigned int __stdcall ThreadProc( void * );
	};
};
