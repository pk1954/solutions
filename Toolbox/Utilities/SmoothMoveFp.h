// SmoothMoveFp.h
//
// Utilities

#pragma once

#include "MoreTypes.h"
#include "PixelTypes.h"

class SmoothMoveFp
{
public:
    SmoothMoveFp( )
    {
        Reset();
    }

    void Reset( )
    {
        m_fPos = START_POINT;
        m_fVelocity = 0.0f;
    }

    float Step( )
    {
        if ( m_fPos < BREAK_POINT )
            m_fVelocity += ACCELERATION;
        else 
            m_fVelocity -= ACCELERATION;
        return m_fPos + m_fVelocity;
    }

    inline static float const START_POINT { 0.0f };
    inline static float const END_POINT   { 1.0f };

    float m_fPos;       // runs from START_POINT to END_POINT
    float m_fVelocity;

private:
    inline static float const DISTANCE     { END_POINT - START_POINT };
    inline static float const BREAK_POINT  { START_POINT + DISTANCE / 2.0f };
    inline static float const NR_OF_STEPS  { 20.0f };
    inline static float const ACCELERATION { ( 4.0f * DISTANCE )/ (NR_OF_STEPS * NR_OF_STEPS) };
};