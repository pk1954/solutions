// SmoothMove.cpp
//
// Utilities

#include "stdafx.h"
#include <assert.h>
#include <algorithm>
#include "SmoothMove.h"

PixelPoint SmoothMove::Step(PixelPoint pixActual, PixelPoint pixDesired)  // returns new pixOffset, which is closer to pixTarget
{
    PixelPoint pixDelta         = pixDesired - pixActual;
    PIXEL      pixDistance      = MaxAbsDelta(pixDelta);
    PIXEL      pixBreakDistCont = PIXEL((m_pixVelocity.GetValue() + 1) * m_pixVelocity.GetValue() / 2);   // break distance if we continue with actual velocity
    PIXEL      pixBreakDistAcc  = pixBreakDistCont + m_pixVelocity + 1_PIXEL;               // break distance if we accelerate
    PIXEL      pixDeltaV        = (pixDistance >= pixBreakDistAcc) 
                                ? 1_PIXEL                                 // distance is big enough to accelerate 
                                : (pixDistance >= pixBreakDistCont) 
                                  ? 0_PIXEL                               // continue with current velocity
                                  : -1_PIXEL;                             // we have to reduce velocity

    m_pixVelocity += pixDeltaV;
    assert(m_pixVelocity >= 0_PIXEL);
    
    if (::abs(pixDelta.GetXvalue()) > ::abs(pixDelta.GetYvalue()))  // x is major direction
    {
        PIXEL pixStepX;
        PIXEL pixStepY;

        assert(pixDelta.GetX().IsNotZero());
        pixStepX = pixDelta.GetX().IsPositive() ? m_pixVelocity : -m_pixVelocity;
        pixActual += PixelPoint(PIXEL(pixStepX), 0_PIXEL);
        if (pixDelta.GetY().IsNotZero())
        {
            PIXEL pixVelocityMinor = PIXEL(abs((pixStepX.GetValue() * pixDelta.GetYvalue() + pixDelta.GetXvalue() / 2) / pixDelta.GetXvalue()));
            pixStepY = pixDelta.GetY().IsPositive() ? pixVelocityMinor : -pixVelocityMinor;
            pixActual += PixelPoint(0_PIXEL, PIXEL(pixStepY) );
        }
    }
    else  // y is major direction
    {
        PIXEL pixStepX;
        PIXEL pixStepY;

        assert(pixDelta.GetY().IsNotZero());
        pixStepY = pixDelta.GetY().IsPositive() ? m_pixVelocity : -m_pixVelocity;
        pixActual += PixelPoint(0_PIXEL, PIXEL(pixStepY));
        if (pixDelta.GetX().IsNotZero())
        {
            PIXEL pixVelocityMinor = PIXEL(abs((pixStepY.GetValue() * pixDelta.GetXvalue() + pixDelta.GetYvalue() / 2) / pixDelta.GetYvalue()));
            pixStepX = pixDelta.GetX().IsPositive() ? pixVelocityMinor : -pixVelocityMinor;
            pixActual += PixelPoint(PIXEL(pixStepX), 0_PIXEL);
        }
    }

#ifdef _DEBUG
    PixelPoint pixDeltaNew  = pixDesired - pixActual;
    PIXEL      pixDistanceNew = MaxAbsDelta(pixDeltaNew);
    assert(pixDistanceNew <= pixDistance);
#endif

   return pixActual;
}

