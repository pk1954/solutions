// TimerQueueTimer.ixx
//
// Win32_utilities

export module TimerQueueTimer;

import std;
import Debug;
import WinBasics;

using std::bit_cast;
using std::wstring;
using std::chrono::milliseconds;
using namespace std::chrono;

export class TimerQueueTimer
{
public:

	~TimerQueueTimer()
	{
		deleteTimer();
		m_hTimer = nullptr;
	}

	void SetRefreshRate(milliseconds const msRate) 
	{ 
		m_msRefreshRate = msRate; 
		deleteTimer();
		startTimer(msRate);
	}

	milliseconds GetRefreshRate() const
	{ 
		return m_msRefreshRate; 
	}

	virtual void TimerProc() = 0;

private:

	HANDLE       m_hTimer        { nullptr };
	milliseconds m_msRefreshRate { static_cast<milliseconds>(0) };

	void startTimer(milliseconds const msTimer)
	{
		auto dwTime = static_cast<DWORD>(msTimer.count());
		CreateTimerQueueTimer
		(
			& m_hTimer,                     // output parameter 
			nullptr,                        // use default timer queue
			(WAITORTIMERCALLBACK)timerProc,          // the timer procedure
			static_cast<void *>(this),      // pointer to this object as parameter to TimerProc
			dwTime,                           // timer is signaled the first time after dwTime msecs
			dwTime,                            // timer is signaled periodically every dwTime msecs
			0                                        // no flags
		);
		Assert(m_hTimer != nullptr);
	}

	void deleteTimer()
	{
		if (m_hTimer != nullptr)
		{
			DeleteTimerQueueTimer(nullptr, m_hTimer, InvalidHandle());
			m_hTimer = nullptr;
		}
	}

	static void __stdcall timerProc(void * const lpParam, bool const TimerOrWaitFired)
	{
		auto const pTimerQueueTimer = bit_cast<TimerQueueTimer *>(lpParam);
		pTimerQueueTimer->TimerProc();
	}
};
