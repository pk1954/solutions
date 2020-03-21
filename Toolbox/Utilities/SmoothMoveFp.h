// SmoothMoveFp.h
//
// Utilities

#pragma once

#include "MoreTypes.h"
#include "PixelTypes.h"

class SmoothMoveFp
{
public:
    SmoothMoveFp( );

    void  Reset( );
    float Step( );

    static float const START_POINT;
    static float const END_POINT;

    float m_fPos;       // runs from START_POINT to END_POINT
    float m_fVelocity;

private:
    static float const DISTANCE;    
    static float const BREAK_POINT; 
    static float const NR_OF_STEPS; 
    static float const ACCELERATION;
};