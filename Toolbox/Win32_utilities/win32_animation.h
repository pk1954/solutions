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
        HWND         const hwnd,
        T                & actual,
        T            const target,
        UINT_PTR     const idTimer,
        TIMERPROC    const timerFunc
    )
    {
        m_hwnd       = hwnd;
        m_pActual    = & actual;
        m_start      = actual;
        m_target     = target;
        m_idTimer    = idTimer;
        m_pTimerFunc = timerFunc;
        m_smoothMove.Start( DEFAULT_NR_OF_STEPS );
        setTimer();
    }

    void Restart( )
    {
        setTimer();
    }

    bool Next( bool const bStopOnTarget = true ) // returns true if target reached
    {
        bool bTargetReached { m_smoothMove.Next() };
        * m_pActual = m_start + (m_target - m_start) * m_smoothMove.GetPos();
        if ( bTargetReached ) 
        {
            if ( bStopOnTarget )
                KillTimer( m_hwnd, m_idTimer );
            else 
                m_smoothMove.Start( DEFAULT_NR_OF_STEPS );
        }
        return bTargetReached;
    }

    float const GetPos()    const { return m_smoothMove.GetPos(); }
    T     const GetTarget() const { return m_target; }

private:
    static unsigned int const DEFAULT_NR_OF_STEPS { 20 };
    static unsigned int const DEFAULT_MILLISECS   { 50 };

    void setTimer() { SetTimer( m_hwnd, m_idTimer, DEFAULT_MILLISECS, m_pTimerFunc ); }

    TIMERPROC           m_pTimerFunc { nullptr };
    UINT_PTR            m_idTimer    { 0 };
    HWND                m_hwnd       { nullptr };
    T                 * m_pActual    { nullptr };
    T                   m_start;
    T                   m_target;
    SmoothMoveFp<float> m_smoothMove;
};
