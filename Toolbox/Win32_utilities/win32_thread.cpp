// win32_thread.cpp
//
// Toolbox\Win32_utilities

#include "stdafx.h"
#include <process.h>
#include "win32_thread.h"

HANDLE Util::RunAsAsyncThread
(
	unsigned int __stdcall func( void * ),
	void * arg,
	UINT * pThreadId
)
{
	UINT threadId;
	HANDLE res = (HANDLE)_beginthreadex( 0, 0, func, arg, 0, & threadId );
	if ( pThreadId )
		* pThreadId = threadId;
	return res;
}

void Util::Thread::BeginThread
(  
	wstring const & strName // for debugging only
)
{
	m_strThreadName = strName;
	m_handle = RunAsAsyncThread( Util::ThreadProc, static_cast<void *>(this), & m_threadId );
	assert( m_handle != nullptr );
}

void Util::Thread::StartThread
(  
	wstring const & strName, // for debugging only
	bool    const   bAsync
)
{
	m_bAsync = bAsync;
	if ( m_bAsync )
	{
		BeginThread( strName );
		m_eventThreadStarter.Wait();
	}
}

void Util::Thread::Terminate( )   // to be called from different thread
{
	if ( m_bAsync )
	{
		PostThreadMessage( m_threadId, WM_QUIT, 0, 0 ); // PostQuitMessage( 0 );  doesn't work
		WaitForSingleObject( m_handle, INFINITE );      // wait until thread has stopped
		CloseHandle( m_handle );
	}
}

static unsigned int __stdcall Util::ThreadProc( void * data ) 
{
    Util::Thread * const pThread = reinterpret_cast<Util::Thread *>( data );
    MSG msg;
	INT iRes;

	SetThreadDescription( GetCurrentThread(), pThread->m_strThreadName.c_str() );
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
