// Animation.ixx
//
// Win32_utilities

module;

#include <concepts>
#include <iostream>
#include "bit"
#include "Windows.h"

export module Animation;

import Win32_Util_Resource;
import WinManager;
import WinCommand;
import SmoothMoveFp;
import ThreadPoolTimer;

using std::bit_cast;
using std::move;
using std::same_as;

DWORD const ANIMATION_RECURRING { 0x1L };

//template <typename T>
//concept IsLinearCombinable = requires(T a, T b, float f) {
//    { a += b } -> same_as<T&>;
//    { a -= b } -> same_as<T&>;
//    { a *= f } -> same_as<T&>;
//};

export template <typename ANIM_TYPE>
class Animation
{
public:

    //static_assert(IsLinearCombinable<ANIM_TYPE>, "ANIM_TYPE must be linear combinable");

    Animation
    (
        WinCommand * pCmd, 
        DWORD const dwFlags = 0
    )
      : m_pCmd(pCmd),
        m_dwFlags(dwFlags)
    {}

    void Start(ANIM_TYPE const origin, ANIM_TYPE const target)  // runs in UI thread
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
                bit_cast<Animation<ANIM_TYPE>*>(pContext)->next();
            },
            this
        );
    }

    ANIM_TYPE GetActual()
    {
        ANIM_TYPE result;
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
    ANIM_TYPE m_actual   {};
    ANIM_TYPE m_start    {};
    ANIM_TYPE m_target   {};
    ANIM_TYPE m_distance {};

    SmoothMoveFp m_smoothMove;
    WinCommand * m_pCmd           { nullptr };
    DWORD  const m_dwFlags        { 0 };
    SRWLOCK      m_srwlData       { SRWLOCK_INIT };
    unsigned int m_uiMsPeriod     { 50 };
    unsigned int m_uiNrOfSteps    { 20 };
    bool         m_bTargetReached { false };

    ThreadPoolTimer m_timer;

    void setActual(ANIM_TYPE const newVal)
    {
        AcquireSRWLockExclusive(& m_srwlData);
        m_actual = newVal;
        ReleaseSRWLockExclusive(& m_srwlData);
    }

    void callUI() // runs in animation thread
    {
        WinManager::PostMessage2MainWin(WM_APP_UI_CALL, 0, bit_cast<LPARAM>(m_pCmd)); // calls DoCall from UI thread
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
                    callUI();
                }
                else
                {
                    callUI();
                    m_timer.StopTimer();
                }
            }
            else
            { 
                callUI();
            }
        }
    }
};
