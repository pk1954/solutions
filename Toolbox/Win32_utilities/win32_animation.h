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

    using APP_PROC = function<void(bool const, bool const)>;

    Animation( APP_PROC const & appProc, DWORD const dwFlags = 0 )
      : m_appProc(appProc),
        m_dwFlags(dwFlags)
    {}

    void Start(ANIM_PAR const origin, ANIM_PAR const target, bool bForwards)
    {
        FILETIME fileTime { m_uiMillisecs, 0 };
        m_start          = origin;
        m_target         = target;
        m_distance       = target - origin;
        m_bTargetReached = false;
        m_bForwards      = bForwards;
        setActual(m_start);
        m_smoothMove.Start( m_uiMsPeriod );
        assert( ! m_pTpTimer );
        m_pTpTimer = CreateThreadpoolTimer( timerProc, this, nullptr );
        SetThreadpoolTimer( m_pTpTimer, &fileTime, m_uiMillisecs, 50L );
    }

    void Stop()
    {
        m_bTargetReached = true;
        if ( m_pTpTimer )
        {
            SetThreadpoolTimer( m_pTpTimer, nullptr, 0, 0 );
            WaitForThreadpoolTimerCallbacks( m_pTpTimer, true );
            CloseThreadpoolTimer( m_pTpTimer );
            m_pTpTimer = nullptr;
        }
    }

    ANIM_PAR const GetActual()
    {
        ANIM_PAR result;
        protect( [&](){ result = m_actual; } );
        return result;
    }

    bool const TargetReached() { return m_bTargetReached; }

    void SetNrOfSteps( unsigned int const uiNrOfSteps )
    {
        m_uiMsPeriod = uiNrOfSteps;
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
    SRWLOCK             m_srwl           { SRWLOCK_INIT };
    TP_TIMER          * m_pTpTimer       { nullptr };
    unsigned int        m_uiMsPeriod     { 20 };
    unsigned int        m_uiMillisecs    { 50 };
    HWND                m_hwnd           { nullptr };
    bool                m_bTargetReached { false };
    bool                m_bForwards;

    void protect( function<void()> func )
    {
        AcquireSRWLockExclusive( & m_srwl );
        (func)();
        ReleaseSRWLockExclusive( & m_srwl );
    }

    void setActual( ANIM_PAR const newVal )
    {
        protect( [&](){ m_actual = newVal; } );
    }

    void next() // runs in animation thread
    {
        if ( ! m_bTargetReached )
        {
            m_bTargetReached = m_smoothMove.Next();
            setActual(m_start + m_distance * m_smoothMove.GetPos());
            if (m_appProc)
                (m_appProc)(m_bTargetReached, m_bForwards);
            if ( m_bTargetReached )
            {
                if ( m_dwFlags & ANIMATION_RECURRING )
                    setActual(m_start);
                else 
                    Stop();
            }
        }
    }

    static void CALLBACK timerProc(PTP_CALLBACK_INSTANCE i, PVOID pContext, PTP_TIMER p)
    {
        reinterpret_cast<Animation<ANIM_PAR> *>(pContext)->next();
    }
};
