// win32_smoothMoveCoordsFp.h
//
// Win32_utilities

#pragma once

#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixelCoordsFp.h"
#include "win32_util.h"
#include "win32_animation.h"

class SmoothCoordsFp
{
public:

    UINT_PTR SetUp
    ( 
        HWND          const hwnd,
        PixelCoordsFp const coordTarget,
        PixelCoordsFp     & coord
    )
    {
        m_pCoord     = & coord;
        m_coordStart = coord;
        m_coordDelta = coordTarget - m_coordStart;
        return m_animation.SetUp(hwnd);
    }

    bool Next( ) // returns true if target reached
    {
        bool bTargetReached { m_animation.Next() };
        * m_pCoord = m_coordStart + m_coordDelta * m_animation.GetPos();
        return bTargetReached;
    }

    int const GetNrOfSteps() const
    {
        return m_animation.GetNrOfSteps();
    }

private:
    Animation       m_animation;
    PixelCoordsFp * m_pCoord;
    PixelCoordsFp   m_coordStart;
    PixelCoordsFp   m_coordDelta;
};