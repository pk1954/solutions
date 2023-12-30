// win32_thread.ixx
//
// Win32_utilities

module;

#include <string>
#include <cassert>
#include "Windows.h"

export module Thread;

import Win32_Event;

using std::wstring;

export HANDLE RunAsAsyncThread(unsigned int __stdcall (void *), void *, UINT * = nullptr);

static unsigned int __stdcall ThreadProc(void *);

export class Thread
{
public:

	virtual ~Thread() = default;

	void StartThread(wstring const &);

	void SetThreadAffinityMask(DWORD_PTR const);

	void PostThreadMsg(UINT, WPARAM const = 0, LPARAM const = 0);

	void TerminateNoWait() const 
	{ 
		PostThreadMessage(m_threadId, WM_QUIT, 0, 0); // PostQuitMessage(0);  doesn't work
	}

	virtual void Terminate(); // Waits until thread has stopped
	virtual void ThreadStartupFunc() {};
	virtual void ThreadMsgDispatcher(MSG const &) = 0;
	virtual void ThreadShutDownFunc() {};

private:
	HANDLE      m_handle             { nullptr };
	UINT        m_threadId           { 0 };
	Win32_Event m_eventThreadStarter { };
	wstring     m_strThreadName      { };

	friend static unsigned int __stdcall ::ThreadProc(void *);
};
