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

    float m_fPos;
    float m_fVelocity;

private:
    float const START_POINT  { 0.0f };
    float const END_POINT    { 1.0f };
    float const DISTANCE     { END_POINT - START_POINT };
    float const BREAK_POINT  { START_POINT + DISTANCE / 2.0f };
    float const NR_OF_STEPS  { 20.0f };
    float const ACCELERATION { ( 4.0f * DISTANCE )/ (NR_OF_STEPS * NR_OF_STEPS) };
};