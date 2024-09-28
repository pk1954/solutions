// ThreadPoolTimer.ixx
//
// Win32_utilities

export module ThreadPoolTimer;

import WinBasics;

export class ThreadPoolTimer
{
public:
    ThreadPoolTimer()
    : m_pTpTimer(nullptr)
    {}

    void StartTimer  // runs in UI thread
    (
        unsigned int uiMsPeriod,
        auto const & timerProc,
        PVOID        pv
    )
    {
        FILETIME fileTime { uiMsPeriod, 0 };
        m_pTpTimer = CreateThreadpoolTimer(timerProc, pv, nullptr);
        SetThreadpoolTimer(m_pTpTimer, &fileTime, uiMsPeriod, uiMsPeriod);
    }

    void StopTimer()  // runs in animation thread
    {
        if (m_pTpTimer)
        {
            SetThreadpoolTimer(m_pTpTimer, nullptr, 0, 0);
            WaitForThreadpoolTimerCallbacks(m_pTpTimer, true);
            CloseThreadpoolTimer(m_pTpTimer);
            m_pTpTimer = nullptr;
        }
    }

    bool IsRunning()
    {
        return m_pTpTimer && IsThreadpoolTimerSet(m_pTpTimer);
    }

private:

    PTP_TIMER m_pTpTimer;
};