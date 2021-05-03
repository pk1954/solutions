// win32_animation.h
//
// Win32_utilities

#pragma once

#include <functional>
#include "windows.h"
#include "MoreTypes.h"
#include "win32_rootWindow.h"
#include "SmoothMoveFp.h"

using AnimationScript = vector<DWORD>;

DWORD const ANIMATION_RECURRING { 0x1L };

template <typename ANIM_PAR>
class Animation
{
public:

    using APP_PROC = function<void(bool const)>;

    Animation( APP_PROC const & appProc, DWORD const dwFlags = 0 )
      : m_appProc(appProc),
        m_dwFlags(dwFlags)
    {}

    void Start(ANIM_PAR const origin, ANIM_PAR const target)  // runs in UI thread
    {
//        AcquireSRWLockExclusive( & m_srwlData );
        m_start          = origin;
        m_target         = target;
        m_distance       = target - origin;
        m_bTargetReached = false;
        m_actual         = m_start;
        m_smoothMove.Start( m_uiNrOfSteps );
        if ( m_pTpTimer )
        {
            int x = 42;
        }
//        ReleaseSRWLockExclusive( & m_srwlData );
        startTimer();
    }

    ANIM_PAR const GetActual()
    {
        ANIM_PAR result;
        AcquireSRWLockExclusive( & m_srwlData );
        result = m_actual;
        ReleaseSRWLockExclusive( & m_srwlData );
        return move(result);
    }

    void SetNrOfSteps( unsigned int const uiNrOfSteps )
    {
        m_uiNrOfSteps = uiNrOfSteps;
    }

    void SetMsPeriod( unsigned int const uiMsPeriod )
    {
        m_uiMsPeriod = uiMsPeriod;
    }

private:
    ANIM_PAR m_actual   {};
    ANIM_PAR m_start    {};
    ANIM_PAR m_target   {};
    ANIM_PAR m_distance {};

    SmoothMoveFp<float> m_smoothMove;
    APP_PROC const      m_appProc;
    DWORD    const      m_dwFlags;
    SRWLOCK             m_srwlData       { SRWLOCK_INIT };
    TP_TIMER          * m_pTpTimer       { nullptr };
    unsigned int        m_uiMsPeriod     { 50 };
    unsigned int        m_uiNrOfSteps    { 20 };
    HWND                m_hwnd           { nullptr };
    bool                m_bTargetReached { false };

    void next() // runs in animation thread
    {
//        AcquireSRWLockExclusive( & m_srwlData );
        if ( ! m_bTargetReached )
        {
            m_bTargetReached = m_smoothMove.Next();
            m_actual = m_start + m_distance * m_smoothMove.GetPos();
            if ( m_bTargetReached )
            {
                if ( m_dwFlags & ANIMATION_RECURRING )
                    m_actual = m_start;
                else 
                    stopTimer();
            }
            if (m_appProc)
                (m_appProc)(m_bTargetReached);
        }
//        ReleaseSRWLockExclusive( & m_srwlData );
    }

    void startTimer()  // runs in UI thread
    {
        FILETIME fileTime { m_uiMsPeriod, 0 };
        m_pTpTimer = CreateThreadpoolTimer( timerProc, this, nullptr );
        SetThreadpoolTimer( m_pTpTimer, &fileTime, m_uiMsPeriod, 50L );
    }

    void stopTimer()  // runs in animation thread
    { 
        SetThreadpoolTimer( m_pTpTimer, nullptr, 0, 0 );
//        WaitForThreadpoolTimerCallbacks( m_pTpTimer, true );
        CloseThreadpoolTimer( m_pTpTimer );
        m_pTpTimer = nullptr;
    } 

    static void CALLBACK timerProc(PTP_CALLBACK_INSTANCE i, PVOID pContext, PTP_TIMER p)
    {
        reinterpret_cast<Animation<ANIM_PAR> *>(pContext)->next();
    }
};
