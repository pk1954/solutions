// win32_smoothMoveCoordsFp.h
//
// Win32_utilities

#pragma once

#include "MoreTypes.h"
#include "PixelTypes.h"
#include "PixelCoordsFp.h"
#include "win32_animation.h"

class SmoothCoordsFp
{
public:

    UINT_PTR SetUp
    ( 
        HWND           const   hwnd,
        MicroMeterRect const & umRect,      
        float          const   fRatioFactor,
        PixelRect      const & pixRect,
        PixelCoordsFp  const & coord
    )
    {
        MicroMeter      const umPixelSizeTarget { coord.ComputeZoom( umRect, pixRect.GetSize(), fRatioFactor ) };
        m_pCoordStart      = coord;
        m_umPixelSizeStart = m_pCoordStart.GetPixelSize();
        m_umPntCenterStart = m_pCoordStart.Transform2MicroMeterPointPos(pixRect.GetCenter());
        m_umPixelSizeDelta = umPixelSizeTarget - m_umPixelSizeStart;
        m_umPntCenterDelta = umRect.GetCenter() - m_umPntCenterStart;
        return m_animation.SetUp(hwnd);
    }

    bool Next( ) // returns true if target reached
    {
        return m_animation.Next();
    }

    MicroMeter const GetNewSize() const
    {
        return m_umPixelSizeStart + m_umPixelSizeDelta * m_animation.GetPos();
    }

    MicroMeterPoint const GetNewCenter() const
    {
        return m_umPntCenterStart + m_umPntCenterDelta * m_animation.GetPos();
    }

    int const GetNrOfSteps() const
    {
        return m_animation.GetNrOfSteps();
    }

private:
    Animation m_animation;

    PixelCoordsFp m_pCoordStart;

    MicroMeterPoint m_umPntCenterStart { MicroMeterPoint::NULL_VAL() }; 
    MicroMeterPoint m_umPntCenterDelta { MicroMeterPoint::NULL_VAL() }; 
    MicroMeter      m_umPixelSizeStart { MicroMeter::NULL_VAL() };      
    MicroMeter      m_umPixelSizeDelta { MicroMeter::NULL_VAL() };      
};