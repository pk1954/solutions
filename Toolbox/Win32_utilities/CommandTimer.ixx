// CommandTimer.cpp
//
// Toolbox\Win32_utilities

module;

#include <chrono>

export module CommandTimer;

import Thread;
import TimerQueueTimer;

using namespace std::chrono;

export class CommandTimer : public TimerQueueTimer
{
public:
	CommandTimer
	(
		Thread * const pThread,
		int            iCmd,
		milliseconds   ms
	)
		: m_pComputeThread(pThread),
	      m_iCmd(iCmd)
	{
		SetRefreshRate(ms);
	}

	void TimerProc() final
	{
		m_pComputeThread->PostThreadCmd(m_iCmd);
	}

private:
	Thread * m_pComputeThread;
	int      m_iCmd;
};



