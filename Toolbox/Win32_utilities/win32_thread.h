// win32_thread.h : 
//
// Win32_utilities

#pragma once

#include "string"
#include <assert.h>
#include "win32_event.h"

using std::wstring;

namespace Util
{
	HANDLE RunAsAsyncThread(unsigned int __stdcall (void *), void *, UINT * = nullptr);

	static unsigned int __stdcall ThreadProc(void *);

	class Thread
	{
	public:

		~Thread() {};

		void BeginThread(wstring const &);
		void StartThread(wstring const &, bool const);

		void SetThreadAffinityMask(DWORD_PTR const);

		void PostThreadMsg(UINT, WPARAM const = 0, LPARAM const = 0);

		void Terminate(); // Waits until thread has stopped
		void TerminateNoWait() { PostThreadMessage(m_threadId, WM_QUIT, 0, 0); }// PostQuitMessage(0);  doesn't work

		bool IsAsyncThread() { return m_bAsync; }

		virtual void ThreadStartupFunc() {};
		virtual void ThreadMsgDispatcher(MSG const) = 0;
		virtual void ThreadShutDownFunc() {};

	private:
		HANDLE  m_handle             { nullptr };
		UINT    m_threadId           { 0 };
		bool    m_bAsync             { false };
		Event   m_eventThreadStarter { };
		wstring m_strThreadName      { };

		friend static unsigned int __stdcall Util::ThreadProc(void *);
	};
};
