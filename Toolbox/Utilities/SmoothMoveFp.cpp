// SmoothMoveFp.cpp
//
// Utilities

#include "stdafx.h"
#include "assert.h"
#include <algorithm>
#include "SmoothMoveFp.h"

float const SmoothMoveFp::START_POINT  { 0.0f };
float const SmoothMoveFp::END_POINT    { 1.0f };
float const SmoothMoveFp::DISTANCE     { END_POINT - START_POINT };
float const SmoothMoveFp::BREAK_POINT  { START_POINT + DISTANCE / 2.0f };
float const SmoothMoveFp::NR_OF_STEPS  { 20.0f };
float const SmoothMoveFp::ACCELERATION { ( 4.0f * DISTANCE )/ (NR_OF_STEPS * NR_OF_STEPS) };
    
SmoothMoveFp::SmoothMoveFp( )
{
    Reset();
}

void SmoothMoveFp::Reset( )
{
    m_fPos = START_POINT;
    m_fVelocity = 0.0f;
}

float SmoothMoveFp::Step( )
{
    if ( m_fPos < BREAK_POINT )
        m_fVelocity += ACCELERATION;
    else 
        m_fVelocity -= ACCELERATION;
    return m_fPos + m_fVelocity;
}
