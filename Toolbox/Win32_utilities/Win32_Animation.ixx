// Win32_Animation.ixx
//
// Win32_utilities

module;

#include <iostream>
#include "bit"
#include "Windows.h"

export module Animation;

import WinCommand;
import SmoothMoveFp;
import ThreadPoolTimer;

using std::bit_cast;
using std::move;

DWORD const ANIMATION_RECURRING { 0x1L };

export template <typename ANIM_PAR>
class Animation
{
public:

    Animation
    (
        WinCommand * pCmd, 
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
        m_timer.StartTimer
        (
            m_uiMsPeriod, 
            [](PTP_CALLBACK_INSTANCE, PVOID pContext, PTP_TIMER)
            {
                bit_cast<Animation<ANIM_PAR>*>(pContext)->next();
            },
            this
        );
    }

    ANIM_PAR GetActual()
    {
        ANIM_PAR result;
        AcquireSRWLockExclusive(& m_srwlData);
        result = m_actual;
        ReleaseSRWLockExclusive(& m_srwlData);
        return result;
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

    SmoothMoveFp m_smoothMove;
    WinCommand * m_pCmd           { nullptr };
    DWORD  const m_dwFlags        { 0 };
    SRWLOCK      m_srwlData       { SRWLOCK_INIT };
    unsigned int m_uiMsPeriod     { 50 };
    unsigned int m_uiNrOfSteps    { 20 };
    bool         m_bTargetReached { false };

    ThreadPoolTimer m_timer;

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
                    m_timer.StopTimer();
                }
            }
            else
            { 
                m_pCmd->CallUI(false);
            }
        }
    }
};
