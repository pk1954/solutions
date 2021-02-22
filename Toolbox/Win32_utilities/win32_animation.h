// win32_animation.h
//
// Win32_utilities

#pragma once

#include <functional>
#include "windows.h"
#include "MoreTypes.h"
#include "win32_rootWindow.h"
#include "SmoothMoveFp.h"

DWORD const ANIMATION_RECURRING    { 0x1L };
DWORD const ANIMATION_SEND_COMMAND { 0x2L };

template <typename ANIM_PAR>
class Animation
{
public:

    using APP_PROC = function<void(bool const)>;

    Animation( APP_PROC const & appProc, DWORD const dwFlags = 0 )
        : m_appProc(appProc),
        m_dwFlags(dwFlags)
    {
        m_pTpTimer = CreateThreadpoolTimer( timerProc, this, nullptr );
    }

    Animation( int const idMsg, HWND const hwnd, DWORD const dwFlags = 0 )
      : m_idMsg(idMsg),
        m_hwnd(hwnd),
        m_dwFlags(dwFlags|ANIMATION_SEND_COMMAND),
        m_appProc(nullptr)
    {
        m_pTpTimer = CreateThreadpoolTimer( timerProc, this, nullptr );
    }

    void Start( ANIM_PAR const origin, ANIM_PAR const target )
    {
        FILETIME fileTime { DEFAULT_MILLISECS, 0 };
        m_start    = origin;
        m_target   = target;
        m_distance = target - origin;
        m_actual   = m_start;
        m_smoothMove.Start( DEFAULT_NR_OF_STEPS );
        SetThreadpoolTimer( m_pTpTimer, &fileTime, DEFAULT_MILLISECS, 50L );
    }

    ANIM_PAR const GetActual()
    {
        ANIM_PAR result;
        AcquireSRWLockExclusive( & m_srwl );
        result = m_actual;
        ReleaseSRWLockExclusive( & m_srwl );
        return result;
    }

private:
    static unsigned int const DEFAULT_NR_OF_STEPS { 20 };
    static unsigned int const DEFAULT_MILLISECS   { 50 };

    void next()
    {
        bool bTargetReached = m_smoothMove.Next();
        AcquireSRWLockExclusive( & m_srwl );
        m_actual = m_start + m_distance * m_smoothMove.GetPos();
        ReleaseSRWLockExclusive( & m_srwl );
        if ( bTargetReached )
        {
            if ( m_dwFlags & ANIMATION_RECURRING )
                Start( m_start, m_target);
            else
                SetThreadpoolTimer( m_pTpTimer, nullptr, 0, 0 );
        }
        if (m_appProc)
            (m_appProc)(bTargetReached);
        else if (m_dwFlags & ANIMATION_SEND_COMMAND)
            ::SendNotifyMessage(m_hwnd, WM_COMMAND, m_idMsg, bTargetReached );     
    }

    static void CALLBACK timerProc(PTP_CALLBACK_INSTANCE i, PVOID pContext, PTP_TIMER p)
    {
        reinterpret_cast<Animation<ANIM_PAR> *>(pContext)->next();
    }

    SmoothMoveFp<float> m_smoothMove;

    TP_TIMER     * m_pTpTimer { nullptr };
    APP_PROC const m_appProc;

    SRWLOCK  m_srwl { SRWLOCK_INIT };

    ANIM_PAR m_actual   {};
    ANIM_PAR m_start    {};
    ANIM_PAR m_target   {};
    ANIM_PAR m_distance {};

    DWORD m_dwFlags { 0 };
    int   m_idMsg   { 0 };
    HWND  m_hwnd    { nullptr };
};
