// win32_animation.h
//
// Win32_utilities

#pragma once

#include "windows.h"
#include "MoreTypes.h"
#include "SmoothMoveFp.h"

template <typename T>
class Animation
{
public:

    void SetUp
    ( 
        HWND         const hwnd,
        T                & actual,
        T            const target,
        UINT_PTR     const idTimer,
        unsigned int const uiNrOfSteps,
        MilliSecs    const ms
    )
    {
        m_hwnd    = hwnd;
        m_pActual = & actual;
        m_start   = actual;
        m_delta   = target - m_start;
        m_idTimer = idTimer;
        m_smoothMove.SetUp( uiNrOfSteps );
        SetTimer( m_hwnd, m_idTimer, ms.GetValue(), nullptr );
    }

    bool Next( ) // returns true if target reached
    {
        bool bTargetReached { m_smoothMove.Next() };
        if ( bTargetReached )
            KillTimer( m_hwnd, m_idTimer );
        * m_pActual = m_start + m_delta * m_smoothMove.GetPos();
        return bTargetReached;
    }

    int const GetNrOfSteps() const
    {
        return m_smoothMove.GetNrOfSteps();
    }

private:
    UINT_PTR            m_idTimer { 0 };
    HWND                m_hwnd    { nullptr };
    T                 * m_pActual { nullptr };
    T                   m_start;
    T                   m_delta;
    SmoothMoveFp<float> m_smoothMove;
};