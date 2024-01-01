// HistAllocThread.ixx
//
// Win32_history

export module HistAllocThread;

import HistorySystem;
import Thread;

export class HistAllocThread : public Thread
{
public:

	HistAllocThread()
	 :	m_pHistorySys(nullptr)
	{ }

	void Start
	(
		HistorySystem * const pHistSys,
		bool            const bAsync      
	) 
	{
		m_pHistorySys = pHistSys;

		if (bAsync)
		{
			StartThread(L"HistAlloc");  
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
