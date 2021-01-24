// win32_animation.h
//
// Win32_utilities

#pragma once

#include "windows.h"
#include "MoreTypes.h"
#include "SmoothMoveFp.h"

class Animation
{
public:

    UINT_PTR SetUp( HWND const hwnd )
    {
        m_hwnd = hwnd;
        m_smoothMove.SetUp();
        SetTimer( m_hwnd, m_idTimer, 50, nullptr );
        return m_idTimer;
    }

    bool Next( ) // returns true if target reached
    {
        bool bTargetReached { m_smoothMove.Next() };
        if ( bTargetReached )
            KillTimer( m_hwnd, m_idTimer );
        return bTargetReached;
    }

    float const GetPos() const
    {
        return m_smoothMove.GetPos();
    }

    int const GetNrOfSteps() const
    {
        return m_smoothMove.GetNrOfSteps();
    }

private:
    HWND                m_hwnd;
    UINT_PTR            m_idTimer { 4711 };
    SmoothMoveFp<float> m_smoothMove;
};