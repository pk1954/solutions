// SmoothMove.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <algorithm>
#include "SmoothMove.h"

SmoothMove::SmoothMove( ) :
    m_pixVelocity( 0 )
{}

PixelPoint SmoothMove::Step( PixelPoint pixActual, PixelPoint pixTarget )  // returns new pixOffset, which is closer to pixTarget
{
    PixelPoint pixDelta         = pixTarget - pixActual;
    PIXEL      pixDistance      = MaxAbsDelta( pixDelta );
    PIXEL      pixBreakDistCont = PIXEL( ( m_pixVelocity.GetValue() + 1 ) * m_pixVelocity.GetValue() / 2);   // break distance if we continue with actual velocity
    PIXEL      pixBreakDistAcc  = pixBreakDistCont + m_pixVelocity + PIXEL(1_PIXEL);               // break distance if we accelerate
    PIXEL      pixDeltaV        = ( pixDistance >= pixBreakDistAcc ) 
                                ? PIXEL(1_PIXEL)                                 // distance is big enough to accelerate 
                                : ( pixDistance >= pixBreakDistCont ) 
                                  ? PIXEL(0_PIXEL)                               // continue with current velocity
                                  : -PIXEL(1_PIXEL);                             // we have to reduce velocity
    PixelPoint pixStep;
    m_pixVelocity += pixDeltaV;
    assert( m_pixVelocity >= PIXEL(0_PIXEL) );
    
    if ( pixDelta.GetX().abs_value() > pixDelta.GetY().abs_value() )  // x is major direction
    {
        assert( pixDelta.GetX() != PIXEL(0_PIXEL) );
        pixStep.SetX( ( pixDelta.GetX() > PIXEL(0_PIXEL) ) ? m_pixVelocity : -m_pixVelocity );
        pixActual.IncX( pixStep.GetX() );
        if ( pixDelta.GetY() != PIXEL(0_PIXEL) )
        {
            PIXEL pixVelocityMinor = PIXEL(abs( ( pixStep.GetXlong() * pixDelta.GetYlong() + pixDelta.GetXlong() / 2 ) / pixDelta.GetXlong() ));
            pixStep.SetY( ( pixDelta.GetY() > PIXEL(0_PIXEL) ) ? pixVelocityMinor : -pixVelocityMinor );
            pixActual.IncY( pixStep.GetY() );
        }
    }
    else  // y is major direction
    {
        assert( pixDelta.GetY() != PIXEL(0_PIXEL) );
        pixStep.SetY( ( pixDelta.GetY() > PIXEL(0_PIXEL) ) ? m_pixVelocity : -m_pixVelocity );
        pixActual.IncY( pixStep.GetY() );
        if ( pixDelta.GetX() != PIXEL(0_PIXEL) )
        {
            PIXEL pixVelocityMinor = PIXEL(abs( ( pixStep.GetYlong() * pixDelta.GetXlong() + pixDelta.GetYlong() / 2 ) / pixDelta.GetYlong() ));
            pixStep.SetX( ( pixDelta.GetX() > PIXEL(0_PIXEL) ) ? pixVelocityMinor : -pixVelocityMinor );
            pixActual.IncX( pixStep.GetX() );
        }
    }

#ifdef _DEBUG
    PixelPoint pixDeltaNew  = pixTarget - pixActual;
    PIXEL      pixDistanceNew = MaxAbsDelta( pixDeltaNew );
    assert( pixDistanceNew <= pixDistance );
#endif

   return pixActual;
}

