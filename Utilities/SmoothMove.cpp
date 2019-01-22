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
    PIXEL      pixBreakDistCont = PIXEL( ( m_pixVelocity.get() + 1 ) * m_pixVelocity.get() / 2);   // break distance if we continue with actual velocity
    PIXEL      pixBreakDistAcc  = pixBreakDistCont + m_pixVelocity + PIXEL(1_PIXEL);               // break distance if we accelerate
    PIXEL      pixDeltaV        = ( pixDistance >= pixBreakDistAcc ) 
                                ? PIXEL(1_PIXEL)                                 // distance is big enough to accelerate 
                                : ( pixDistance >= pixBreakDistCont ) 
                                  ? PIXEL(0_PIXEL)                               // continue with current velocity
                                  : -PIXEL(1_PIXEL);                             // we have to reduce velocity
    PixelPoint pixStep;
    m_pixVelocity += pixDeltaV;
    assert( m_pixVelocity >= PIXEL(0_PIXEL) );
    
    if ( abs( pixDelta.x ) > abs( pixDelta.y ) )  // x is major direction
    {
        assert( pixDelta.x != PIXEL(0_PIXEL) );
        pixStep.x    = ( pixDelta.x > PIXEL(0_PIXEL) ) ? m_pixVelocity : -m_pixVelocity;
        pixActual.x += pixStep.x;
        if ( pixDelta.y != PIXEL(0_PIXEL) )
        {
            PIXEL pixVelocityMinor = PIXEL(abs( ( pixStep.x.get() * pixDelta.y.get() + pixDelta.x.get() / 2 ) / pixDelta.x.get() ));
            pixStep.y = ( pixDelta.y > PIXEL(0_PIXEL) ) ? pixVelocityMinor : -pixVelocityMinor;
            pixActual.y += pixStep.y;
        }
    }
    else  // y is major direction
    {
        assert( pixDelta.y != PIXEL(0_PIXEL) );
        pixStep.y    = ( pixDelta.y > PIXEL(0_PIXEL) ) ? m_pixVelocity : -m_pixVelocity;
        pixActual.y += pixStep.y;
        if ( pixDelta.x != PIXEL(0_PIXEL) )
        {
            PIXEL pixVelocityMinor = PIXEL(abs( ( pixStep.y.get() * pixDelta.x.get() + pixDelta.y.get() / 2 ) / pixDelta.y.get() ));
            pixStep.x = ( pixDelta.x > PIXEL(0_PIXEL) ) ? pixVelocityMinor : -pixVelocityMinor;
            pixActual.x += pixStep.x;
        }
    }

#ifdef _DEBUG
    PixelPoint pixDeltaNew  = pixTarget - pixActual;
    PIXEL      pixDistanceNew = MaxAbsDelta( pixDeltaNew );
    assert( pixDistanceNew <= pixDistance );
#endif

   return pixActual;
}

