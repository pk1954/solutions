// Win32_Animation.ixx
//
// Win32_utilities

module;

#include <iostream>
#include "bit"
#include "Windows.h"

export module Animation;

import Commands;
import SmoothMoveFp;

using std::bit_cast;
using std::move;
using std::wcout;

DWORD const ANIMATION_RECURRING { 0x1L };

export template <typename ANIM_PAR>
class Animation
{
public:

    Animation
    (
        Command   * pCmd, 
        DWORD const dwFlags = 0
    )
      : m_pCmd(pCmd),
        m_dwFlags(dwFlags)
    {}

    void Start(ANIM_PAR const origin, ANIM_PAR const target)  // runs in UI thread
    {
        m_start          = origin;
        m_target         = target;
        m_distance       = target - origin;
        m_bTargetReached = false;
        setActual(m_start);
        m_smoothMove.Start(m_uiNrOfSteps);
        if (m_pTpTimer)
        {
            int x = 42;
        }
       // wcout << L"Start: origin = " << origin << L" target = " << target << std::endl;
        startTimer();
    }

    ANIM_PAR GetActual()
    {
        ANIM_PAR result;
        AcquireSRWLockExclusive(& m_srwlData);
        result = m_actual;
        ReleaseSRWLockExclusive(& m_srwlData);
        return move(result);
    }

    void SetNrOfSteps(unsigned int const uiNrOfSteps)
    {
        m_uiNrOfSteps = uiNrOfSteps;
    }

    void SetMsPeriod(unsigned int const uiMsPeriod)
    {
        m_uiMsPeriod = uiMsPeriod;
    }

private:
    ANIM_PAR m_actual   {};
    ANIM_PAR m_start    {};
    ANIM_PAR m_target   {};
    ANIM_PAR m_distance {};

    SmoothMoveFp   m_smoothMove;
    Command      * m_pCmd           { nullptr };
    DWORD    const m_dwFlags        { 0 };
    SRWLOCK        m_srwlData       { SRWLOCK_INIT };
    TP_TIMER     * m_pTpTimer       { nullptr };
    unsigned int   m_uiMsPeriod     { 50 };
    unsigned int   m_uiNrOfSteps    { 20 };
    HWND           m_hwnd           { nullptr };
    bool           m_bTargetReached { false };

    void setActual(ANIM_PAR const newVal)
    {
        AcquireSRWLockExclusive(& m_srwlData);
        m_actual = newVal;
        ReleaseSRWLockExclusive(& m_srwlData);
    }

    void next() // runs in animation thread
    {
        if (! m_bTargetReached)
        {
            m_bTargetReached = m_smoothMove.Next();
            setActual(m_start + m_distance * m_smoothMove.GetPos());
            if (m_bTargetReached)
            {
                if (m_dwFlags & ANIMATION_RECURRING)
                {
                    setActual(m_start);
                    m_pCmd->CallUI(true);
                }
                else
                {
                    m_pCmd->CallUI(true);
                    stopTimer();
                }
            }
            else
            { 
                m_pCmd->CallUI(false);
            }
        }
    }

    void startTimer()  // runs in UI thread
    {
        std::wcout << L"startTimer" << std::endl;
        FILETIME fileTime { m_uiMsPeriod, 0 };
        m_pTpTimer = CreateThreadpoolTimer(timerProc, this, nullptr);
        SetThreadpoolTimer(m_pTpTimer, &fileTime, m_uiMsPeriod, 50L);
    }

    void stopTimer()  // runs in animation thread
    { 
        std::wcout << L"stopTimer" << std::endl;
        if (m_pTpTimer)
        {
            SetThreadpoolTimer(m_pTpTimer, NULL, 0, 0);
            WaitForThreadpoolTimerCallbacks(m_pTpTimer, TRUE);
            CloseThreadpoolTimer(m_pTpTimer);
        }
        m_pTpTimer = nullptr;
    } 

    static void CALLBACK timerProc
    (
        PTP_CALLBACK_INSTANCE, 
        PVOID pContext, 
        PTP_TIMER
    )
    {
        bit_cast<Animation<ANIM_PAR> *>(pContext)->next();
    }
};
