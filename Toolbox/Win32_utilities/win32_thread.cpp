// Win32_Thread.cpp
//
// Toolbox\Win32_utilities

module;

#include <string>
#include <cassert>
#include <process.h>
#include "Windows.h"

module Thread;

using std::wstring;

HANDLE RunAsAsyncThread
(
	unsigned int __stdcall func(void *),
	void * arg,
	UINT * pThreadId
)
{
	UINT threadId;
	HANDLE res = (HANDLE)_beginthreadex(0, 0, func, arg, 0, & threadId);
	if (pThreadId)
		* pThreadId = threadId;
	return res;
}

void Thread::SetThreadAffinityMask(DWORD_PTR const mask)
{
	::SetThreadAffinityMask(m_handle, mask);
}

void Thread::StartThread
( 
	wstring const & strName // for debugging only
)
{
	m_strThreadName = strName;
	m_handle = RunAsAsyncThread(::ThreadProc, static_cast<void *>(this), & m_threadId);
	assert(m_handle != nullptr);
	m_eventThreadStarter.Wait();
}

void Thread::PostThreadMsg(UINT uiMsg, WPARAM const wParam, LPARAM const lParam)
{
	assert(m_threadId != 0);
	bool const bRes = ::PostThreadMessage(m_threadId, uiMsg, wParam, lParam);
	//				DWORD err = GetLastError();
	assert(bRes);
}

void Thread::Terminate()   // to be called from different thread
{
	TerminateNoWait();
	WaitForSingleObject(m_handle, INFINITE);      // wait until thread has stopped
	CloseHandle(m_handle);
	m_handle = nullptr;
}

static unsigned int __stdcall ThreadProc(void * data) 
{
    ::Thread * const pThread = reinterpret_cast<::Thread *>(data);
    MSG msg;
	INT iRes;

	SetThreadDescription(GetCurrentThread(), pThread->m_strThreadName.c_str());
	(void)PeekMessage(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);  // cause creation of message queue

	pThread->m_eventThreadStarter.Continue();   // trigger waiting thread to continue

	pThread->ThreadStartupFunc();

	while (iRes = GetMessage(&msg, nullptr, 0, 0))  // loop ends at WM_QUIT 
	{
		assert(iRes > 0);
		pThread->ThreadMsgDispatcher(msg);
	} 

	pThread->ThreadShutDownFunc();

	return iRes;
}
