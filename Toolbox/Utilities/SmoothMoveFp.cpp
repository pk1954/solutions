// SmoothMoveFp.cpp
//
// Utilities

#include "stdafx.h"
#include "assert.h"
#include <algorithm>
#include "SmoothMoveFp.h"

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

//MicroMeter SmoothMoveFp::Step( MicroMeter umActual, MicroMeter umDesired )
//{
//    MicroMeter const umDelta { umDesired - umActual };
//
//    if ( m_umAccStep.IsNull( ) )
//    {
//        //float const fFactor { 2.0f / (NR_OF_STEPS * (NR_OF_STEPS + 1)) };
//        float const fFactor { 1000.0f };
//        m_umAccStep = MicroMeter( umDelta.GetAbsValue() / fFactor ); // Always positive!
//    }
//
//    MicroMeter const umAbsDelta    { umDelta.GetAbsValue() };
//    MicroMeter const umAbsVelocity { m_umVelocity.GetAbsValue() };
//    float      const fSteps2Break  { umAbsVelocity / m_umAccStep };
//    MicroMeter const umBreakDist   { umAbsVelocity * fSteps2Break * 0.5f };
//    MicroMeter       umAbsVelocityNew;
//    if ( umAbsDelta > umBreakDist ) // distance is big enough to accelerate 
//    {
//        umAbsVelocityNew = umAbsVelocity + m_umAccStep;
//    }
//    else  // we have to reduce velocity
//    {
//        umAbsVelocityNew = umAbsVelocity - m_umAccStep;
//        if ( umAbsDelta < umAbsVelocityNew  )
//        {
//            Reset();
//            return umDesired;
//        }
//    }
//
//    m_umVelocity = ( umDelta > 0.0_MicroMeter ) ? umAbsVelocityNew : - umAbsVelocityNew;
//    return umActual + m_umVelocity;
//}
