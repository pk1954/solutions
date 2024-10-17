// ThreadPoolTimer.ixx
//
// Win32_utilities

export module ThreadPoolTimer;

import Debug;
import WinBasics;

export class ThreadPoolTimer
{
public:
    ThreadPoolTimer
    (
        auto const & timerProc,
        PVOID        pv
    )
    {
        m_pTpTimer = CreateThreadpoolTimer(timerProc, pv, nullptr);
    }

    ~ThreadPoolTimer() 
    {
        if (m_pTpTimer)
        {
            SetThreadpoolTimer(m_pTpTimer, nullptr, 0, 0);
            WaitForThreadpoolTimerCallbacks(m_pTpTimer, true);
            CloseThreadpoolTimer(m_pTpTimer);
            m_pTpTimer = nullptr;
        }
    }

    void StartTimer  // runs in UI thread
    (
        unsigned int uiMsPeriod
    )
    {
        FILETIME fileTime { uiMsPeriod, 0 };
        SetThreadpoolTimer(m_pTpTimer, &fileTime, uiMsPeriod, uiMsPeriod);
    }

    void StopTimer()  // runs in animation thread
    {
        Assert(m_pTpTimer);
        SetThreadpoolTimer(m_pTpTimer, nullptr, 0, 0);
    }

    bool IsRunning()
    {
        return m_pTpTimer && IsThreadpoolTimerSet(m_pTpTimer);
    }

private:

    PTP_TIMER m_pTpTimer;
};