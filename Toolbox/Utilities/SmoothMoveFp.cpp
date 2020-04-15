// SmoothMoveFp.cpp
//
// Utilities

#include "stdafx.h"
#include "assert.h"
#include <algorithm>
#include "SmoothMoveFp.h"
    
//SmoothMoveFp::SmoothMoveFp( )
//{
//    Reset();
//}
//
//void SmoothMoveFp::Reset( )
//{
//    m_fPos = START_POINT;
//    m_fVelocity = 0.0f;
//}
//
//float SmoothMoveFp::Step( )
//{
//    if ( m_fPos < BREAK_POINT )
//        m_fVelocity += ACCELERATION;
//    else 
//        m_fVelocity -= ACCELERATION;
//    return m_fPos + m_fVelocity;
//}
