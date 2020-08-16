// SmoothMoveFp.h
//
// Utilities

#pragma once

#include "MoreTypes.h"
#include "PixelTypes.h"

class SmoothMoveFp
{
public:

    void SetUp
    ( 
        MicroMeter      const umPixelSizeStart, 
        MicroMeterPoint const umPntCenterStart, 
        MicroMeter      const umPixelSizeTarget, 
        MicroMeterPoint const umPntCenterTarget 
    )
    {
        m_umPixelSizeStart = umPixelSizeStart;
        m_umPntCenterStart = umPntCenterStart;
        m_umPixelSizeDelta = umPixelSizeTarget - m_umPixelSizeStart;
        m_umPntCenterDelta = umPntCenterTarget - m_umPntCenterStart;
        m_fPos = START_POINT;
        m_fVelocity = 0.0f;
    }

    bool Next( ) // returns true if target reached
    {
        if ( m_fPos <= BREAK_POINT )
            m_fVelocity += ACCELERATION;
        else 
            m_fVelocity -= ACCELERATION;
        m_fPos += m_fVelocity;
        if ( m_fPos >= END_POINT * 0.99f )
        {
            m_fPos = END_POINT;
            return true;
        }
        return false;
    }

    MicroMeter const GetNewSize( )
    {
        return m_umPixelSizeStart + m_umPixelSizeDelta * m_fPos ;
    }

    MicroMeterPoint const GetNewCenter( )
    {
        return m_umPntCenterStart + m_umPntCenterDelta * m_fPos;
    }

private:
    inline static float const START_POINT { 0.0f };
    inline static float const END_POINT   { 1.0f };

    inline static float const DISTANCE     { END_POINT - START_POINT };
    inline static float const BREAK_POINT  { START_POINT + DISTANCE / 2.0f };
    inline static float const NR_OF_STEPS  { 20.0f };
    inline static float const ACCELERATION { ( 4.0f * DISTANCE ) / (NR_OF_STEPS * NR_OF_STEPS) };

    float           m_fPos      { START_POINT };       // runs from START_POINT to END_POINT
    float           m_fVelocity { 0.0f };

    MicroMeterPoint m_umPntCenterStart { MicroMeterPoint::NULL_VAL() }; 
    MicroMeterPoint m_umPntCenterDelta { MicroMeterPoint::NULL_VAL() }; 
    MicroMeter      m_umPixelSizeStart { MicroMeter::NULL_VAL() };      
    MicroMeter      m_umPixelSizeDelta { MicroMeter::NULL_VAL() };      

};