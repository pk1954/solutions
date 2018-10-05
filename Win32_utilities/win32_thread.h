// win32_thread.h : 
//

#pragma once

#include "assert.h"
#include <Windows.h>
#include <process.h>
#include "win32_event.h"

namespace Util
{
	class Thread
	{
	public:
		Thread
		(
			unsigned int ( __stdcall *start_address )( void * ),
			void * parameter
		) 
		{
			m_handle = (HANDLE)_beginthreadex( 0, 0, start_address, parameter, 0, & m_threadId );
		    assert( m_handle != nullptr );
		}

		void PostMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
		{
		    BOOL const bRes = PostThreadMessage( m_threadId, uiMsg, wParam, lParam );
			DWORD err = GetLastError( );
			assert( bRes );
		}

		void SetThreadAffinityMask( DWORD_PTR mask )
		{
		    (void)::SetThreadAffinityMask( m_handle, mask );
		}

		void Wait4Termination( )
		{
			WaitForSingleObject( m_handle, INFINITE );   // wait until thread has stopped
			CloseHandle( m_handle );
		}

	private:
		Event  m_eventThreadStarter;
		HANDLE m_handle;
		UINT   m_threadId;
	};

};
