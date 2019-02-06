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

    m_pixVelocity += pixDeltaV;
    assert( m_pixVelocity >= PIXEL(0_PIXEL) );
    
    if ( pixDelta.GetX().abs_value() > pixDelta.GetY().abs_value() )  // x is major direction
    {
        PIXEL pixStepX;
        PIXEL pixStepY;

        assert( pixDelta.GetX().IsNotZero() );
        pixStepX = pixDelta.GetX().IsPositive() ? m_pixVelocity : -m_pixVelocity;
        pixActual += PixelPoint( pixStepX, PIXEL(0_PIXEL) );
        if ( pixDelta.GetY().IsNotZero() )
        {
            PIXEL pixVelocityMinor = PIXEL(abs( ( pixStepX.GetValue() * GetYvalue(pixDelta) + GetXvalue(pixDelta) / 2 ) / GetXvalue(pixDelta) ));
            pixStepY = pixDelta.GetY().IsPositive() ? pixVelocityMinor : -pixVelocityMinor;
            pixActual += PixelPoint( PIXEL(0_PIXEL), pixStepY );
        }
    }
    else  // y is major direction
    {
        PIXEL pixStepX;
        PIXEL pixStepY;

        assert( pixDelta.GetY().IsNotZero() );
        pixStepY = pixDelta.GetY().IsPositive() ? m_pixVelocity : -m_pixVelocity;
        pixActual += PixelPoint( PIXEL(0_PIXEL), pixStepY );
        if ( pixDelta.GetX().IsNotZero() )
        {
            PIXEL pixVelocityMinor = PIXEL(abs( ( pixStepY.GetValue() * GetXvalue(pixDelta) + GetYvalue(pixDelta) / 2 ) / GetYvalue(pixDelta) ));
            pixStepX = pixDelta.GetX().IsPositive() ? pixVelocityMinor : -pixVelocityMinor;
            pixActual += PixelPoint( pixStepX, PIXEL(0_PIXEL) );
        }
    }

#ifdef _DEBUG
    PixelPoint pixDeltaNew  = pixTarget - pixActual;
    PIXEL      pixDistanceNew = MaxAbsDelta( pixDeltaNew );
    assert( pixDistanceNew <= pixDistance );
#endif

   return pixActual;
}

