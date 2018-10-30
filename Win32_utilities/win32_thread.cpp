// win32_thread.cpp
//

#include "stdafx.h"
#include <process.h>
#include "win32_thread.h"

void Util::Thread::StartThread
(  
	wstring const & strName // for debugging only
)
{
	m_strThreadName = strName;
	m_handle = (HANDLE)_beginthreadex( 0, 0, Util::ThreadProc, this, 0, & m_threadId );
	assert( m_handle != nullptr );
	m_eventThreadStarter.Wait();
}

void Util::Thread::Terminate( )   // to be called from different thread
{
	PostThreadMessage( m_threadId, WM_QUIT, 0, 0 ); // PostQuitMessage( 0 );  but doesn't work
	WaitForSingleObject( m_handle, INFINITE );  // wait until thread has stopped
	CloseHandle( m_handle );
}

static unsigned int __stdcall Util::ThreadProc( void * data ) 
{
    Util::Thread * const pThread = static_cast<Util::Thread *>( data );
    MSG  msg;
	INT  iRes;

    (void)PeekMessage( &msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE );  // cause creation of message queue

	pThread->m_eventThreadStarter.Continue();   // trigger waiting thread to continue

	pThread->ThreadStartupFunc( );

	while ( iRes = GetMessage( &msg, nullptr, 0, 0 ) )  // loop ends at WM_QUIT 
	{
		assert( iRes > 0 );
		pThread->ThreadMsgDispatcher( msg );
	} 

	pThread->ThreadShutDownFunc( );

	return iRes;
}
