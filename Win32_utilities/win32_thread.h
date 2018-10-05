// win32_thread.h : 
//

#pragma once

#include "assert.h"
#include <Windows.h>
#include <process.h>
#include <string>
#include "win32_event.h"

using namespace std;

namespace Util
{
	class Thread
	{
	public:
		Thread(	unsigned int ( __stdcall *start_address )( void * ), string, void * parameter );

		void PostMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
		{
		    BOOL const bRes = PostThreadMessage( m_threadId, uiMsg, wParam, lParam );
			DWORD err = GetLastError( );
			assert( bRes );
		}

		void SetThreadAffinityMask( DWORD_PTR );
		void Wait4Termination( );

	private:
		Event  m_eventThreadStarter;
		HANDLE m_handle;
		UINT   m_threadId;
		string m_strName;
		void * m_parameter;

		unsigned int ( __stdcall * m_start_address )( void * );

		friend static unsigned int __stdcall ThreadProc( void * );
	};
};
