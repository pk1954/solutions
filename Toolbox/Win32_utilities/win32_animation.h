// win32_animation.h
//
// Win32_utilities

#pragma once

#include "windows.h"
#include "MoreTypes.h"
#include "win32_rootWindow.h"
#include "SmoothMoveFp.h"

template <typename T>
T * GetWinPtr( HWND hwnd )
{
    return reinterpret_cast<T *>(GetUserDataPtr( hwnd ));
}

template <typename T>
class Animation
{
public:

    void Start
    ( 
        HWND      const hwnd,
        T             & actual,
        T         const target,
        TIMERPROC const timerProc
    )
    {
        auto pWin { GetWinPtr<RootWindow>( hwnd ) };
        m_hwnd           = hwnd;
        m_pActual        = & actual;
        m_start          = actual;
        m_target         = target;
        m_idTimer        = pWin->GetTimerId();
        m_pTimerProc     = timerProc;
        m_bTargetReached = false;
        m_smoothMove.Start( DEFAULT_NR_OF_STEPS );
        setTimer();
    }

    void Restart( )
    {
        setTimer();
    }

    bool const Next( )
    {
        auto pRootWin { GetWinPtr<RootWindow>( m_hwnd ) };
        m_bTargetReached = m_smoothMove.Next();
        * m_pActual = m_start + (m_target - m_start) * m_smoothMove.GetPos();
        if ( m_bTargetReached )
            KillTimer( m_hwnd, m_idTimer );
        pRootWin->Notify( false );
        return m_bTargetReached;
    }

    bool  const TargetReached() const { return m_bTargetReached; }
    float const GetPos()        const { return m_smoothMove.GetPos(); }
    T     const GetTarget()     const { return m_target; }

private:
    static unsigned int const DEFAULT_NR_OF_STEPS { 20 };
    static unsigned int const DEFAULT_MILLISECS   { 50 };

    void setTimer() 
    { 
        SetTimer( m_hwnd, m_idTimer, DEFAULT_MILLISECS, m_pTimerProc ); 
    }

    SmoothMoveFp<float> m_smoothMove;

    TIMERPROC m_pTimerProc     { nullptr };
    bool      m_bTargetReached { false };
    UINT_PTR  m_idTimer        { 0 };
    HWND      m_hwnd           { nullptr };
    T       * m_pActual        { nullptr };
    T         m_start;
    T         m_target;
};
