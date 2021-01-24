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
        HWND           const   hwnd,
        MicroMeterRect const & umRect,      
        float          const   fRatioFactor,
        PixelCoordsFp        & coord
    )
    {
        PixelRect  const & pixRect           { Util::GetClPixelRect(hwnd) };
        MicroMeter const   umPixelSizeTarget { coord.ComputeZoom( umRect, pixRect.GetSize(), fRatioFactor ) };
        m_fpCenter         = Convert2fPixelPoint( Util::GetClRectCenter(hwnd) );
        m_coordStart       = coord;
        m_umPixelSizeStart = m_coordStart.GetPixelSize();
        m_umPntCenterStart = m_coordStart.Transform2MicroMeterPointPos(pixRect.GetCenter());
        m_umPixelSizeDelta = umPixelSizeTarget - m_umPixelSizeStart;
        m_umPntCenterDelta = umRect.GetCenter() - m_umPntCenterStart;
        m_pCoord           = & coord;
        return m_animation.SetUp(hwnd);
    }

    bool Next( ) // returns true if target reached
    {
        bool bTargetReached { m_animation.Next() };
        MicroMeter      const umNewSize      { m_umPixelSizeStart + m_umPixelSizeDelta * m_animation.GetPos() };
        MicroMeterPoint const umPntNewCenter { m_umPntCenterStart + m_umPntCenterDelta * m_animation.GetPos() };
        m_pCoord->Zoom  ( umNewSize );
        m_pCoord->Center( umPntNewCenter, m_fpCenter );
        return bTargetReached;
    }

    int const GetNrOfSteps() const
    {
        return m_animation.GetNrOfSteps();
    }

private:
    Animation m_animation;

    fPixelPoint   m_fpCenter;

    PixelCoordsFp * m_pCoord;

    PixelCoordsFp m_coordStart;

    MicroMeterPoint m_umPntCenterStart { MicroMeterPoint::NULL_VAL() }; 
    MicroMeterPoint m_umPntCenterDelta { MicroMeterPoint::NULL_VAL() }; 
    MicroMeter      m_umPixelSizeStart { MicroMeter::NULL_VAL() };      
    MicroMeter      m_umPixelSizeDelta { MicroMeter::NULL_VAL() };      
};