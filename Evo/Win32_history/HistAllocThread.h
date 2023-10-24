// HistAllocThread.h
//
// Win32_history

module;

#include "HistorySystem.h"
#include "win32_thread.h"

class HistorySystem;

class HistAllocThread : public Util::Thread
{
public:

	HistAllocThread()
	 :	m_pHistorySys(nullptr)
	{ }

	void Start
	(
		HistorySystem * const pHistSys,
		BOOL            const bAsync      
	) 
	{
		m_pHistorySys = pHistSys;

		if (bAsync)
		{
			StartThread(L"HistAlloc", true);  
		}
		else
		{
			while (m_pHistorySys->AddHistorySlot()) {}
		}
	}

	~HistAllocThread() {};

	void  ThreadStartupFunc()
	{
		ThreadMsgDispatcher(MSG{0,0,0,0});
	}

	void ThreadMsgDispatcher(MSG msg)
	{
		if (m_pHistorySys->AddHistorySlot())                    // if allocation of slot 
		{                                                         // was successfull,
			PostThreadMsg(msg.message, msg.wParam, msg.lParam); // do it again
		}
	}

private:
	HistorySystem * m_pHistorySys;
};
