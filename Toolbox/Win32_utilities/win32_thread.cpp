// Win32_Thread.cpp
//
// Toolbox\Win32_utilities

module;

#include <Windows.h>

module Thread;

import std;
import Debug;

using std::wstring;

Thread::~Thread()
{
	int x = 42;
}

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
	m_handle = RunAsAsyncThread(::StdThreadProc, static_cast<void *>(this), &m_threadId);
	Assert(m_handle != nullptr);
	m_eventThreadStarter.Wait();
}

void Thread::PostThreadMsg(UINT uiMsg, WPARAM const wParam, LPARAM const lParam)
{
	Assert(m_threadId != 0);
	bool const bRes = ::PostThreadMessage(m_threadId, uiMsg, wParam, lParam);
	DWORD err = GetLastError();
	if (!bRes)
	{
		int x = 42;
	}
	Assert(bRes);
}

void Thread::Terminate()   // to be called from different thread
{
	TerminateNoWait();
	WaitForSingleObject(m_handle, INFINITE);      // wait until thread has stopped
	CloseHandle(m_handle);
	m_handle = nullptr;
}

static ::Thread* InitializeThread(void* data)
{
	::Thread * const pThread = reinterpret_cast<::Thread*>(data);
	MSG msg;
	SetThreadDescription(GetCurrentThread(), pThread->m_strThreadName.c_str());
	PeekMessage(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);  // cause creation of message queue
	pThread->m_eventThreadStarter.Continue();   // trigger waiting thread to continue
	pThread->ThreadStartupFunc();
	return pThread;
}

static unsigned int __stdcall StdThreadProc(void * data) 
{
    ::Thread * const pThread = InitializeThread(data);

	while (true)
	{
		MSG msg;
		if (pThread->IsInGameMode())
		{
			BOOL incomingMessages { PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) };
			if (msg.message == WM_QUIT)
				goto KILL_THREAD;
			if (incomingMessages)
				pThread->ThreadMsgDispatcher(msg);
			else
				pThread->DoGameStuff();
		}
		else
		{
			int iRes = GetMessage(&msg, nullptr, 0, 0);  // loop ends at WM_QUIT 
			if (iRes == 0)
				goto KILL_THREAD;
			Assert(iRes > 0);
			pThread->ThreadMsgDispatcher(msg);
		}
	} 

KILL_THREAD:
	return 0;
}
