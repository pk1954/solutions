// win32_animation.h
//
// Win32_utilities

#pragma once

#include "windows.h"
#include "SmoothMoveFp.h"

template <typename T>
class Animation
{
public:

    UINT_PTR SetUp
    ( 
        HWND const hwnd,
        T    const target,
        T        & actual
    )
    {
        m_pActual = & actual;
        m_start   = actual;
        m_delta   = target - m_start;
        m_hwnd    = hwnd;
        m_smoothMove.SetUp();
        SetTimer( m_hwnd, m_idTimer, 50, nullptr );
        return m_idTimer;
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
    HWND                m_hwnd;
    UINT_PTR            m_idTimer { 4711 };
    SmoothMoveFp<float> m_smoothMove;
    T                 * m_pActual;
    T                   m_start;
    T                   m_delta;
};