// win32_thread.cpp
//

#include "stdafx.h"
#include <process.h>
#include "win32_thread.h"

void Util::Thread::StartThread( )
{
	m_handle = (HANDLE)_beginthreadex( 0, 0, Util::ThreadProc, this, 0, & m_threadId );
	assert( m_handle != nullptr );
	m_eventThreadStarter.Wait();
}

void Util::Thread::SetThreadAffinityMask( DWORD_PTR mask )
{
	::SetThreadAffinityMask( m_handle, mask );
}

void Util::Thread::Terminate( )
{
	PostMessage( Util::Thread::THREAD_MSG_EXIT, 0, 0 );        // stop message pump of thread
	WaitForSingleObject( m_handle, INFINITE );   // wait until thread has stopped
	CloseHandle( m_handle );
}

static unsigned int __stdcall Util::ThreadProc( void * data ) 
{
    Util::Thread * const pThread = static_cast<Util::Thread *>( data );
    MSG  msg;
    (void)PeekMessage( &msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE );  // cause creation of message queue
    pThread->m_eventThreadStarter.Continue();       			        // trigger waiting thread to continue

    for(;;)
    {
        BOOL const bRet = GetMessage( &msg, nullptr, 0, 0 );
        assert( bRet >= 0 );
		if ( msg.message == pThread->THREAD_MSG_EXIT )
			break;
        (void)pThread->DispatchThreadMsg( msg.message, msg.wParam, msg.lParam );
    } 

	return 0;
}
