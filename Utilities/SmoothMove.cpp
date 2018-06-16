// SmoothMove.cpp
//

#include "stdafx.h"
#include "assert.h"
#include <algorithm>
#include "SmoothMove.h"

SmoothMove::SmoothMove( ) :
    m_lVelocity( 0 )
{}

PixelPoint SmoothMove::Step( PixelPoint pixActual, PixelPoint pixTarget )  // returns new pixOffset, which is closer to pixTarget
{
    PixelPoint pixDelta       = pixTarget - pixActual;
    long       lDistance      = std::max( abs( pixDelta.x ), abs( pixDelta.y ) );
    long       lBreakDistCont = ( m_lVelocity + 1 ) * ( m_lVelocity ) / 2;   // break distance if we continue with actual velocity
    long       lBreakDistAcc  = lBreakDistCont + m_lVelocity + 1;            // break distance if we accelerate
    long       lDeltaV        = ( lDistance >= lBreakDistAcc ) 
                                ? 1                                 // distance is big enough to accelerate 
                                : ( lDistance >= lBreakDistCont ) 
                                  ? 0                               // continue with current velocity
                                  : -1;                             // we have to reduce velocity
    PixelPoint pixStep;
    m_lVelocity += lDeltaV;
    assert( m_lVelocity >= 0 );
    
    if ( abs( pixDelta.x ) > abs( pixDelta.y ) )  // x is major direction
    {
        assert( pixDelta.x != 0 );
        pixStep.x    = ( pixDelta.x > 0 ) ? m_lVelocity : -m_lVelocity;
        pixActual.x += pixStep.x;
        if ( pixDelta.y != 0 )
        {
            long lVelocityMinor = abs( ( pixStep.x * pixDelta.y + pixDelta.x / 2 ) / pixDelta.x );
            pixStep.y = ( pixDelta.y > 0 ) ? lVelocityMinor : -lVelocityMinor;
            pixActual.y += pixStep.y;
        }
    }
    else  // y is major direction
    {
        assert( pixDelta.y != 0 );
        pixStep.y    = ( pixDelta.y > 0 ) ? m_lVelocity : -m_lVelocity;
        pixActual.y += pixStep.y;
        if ( pixDelta.x != 0 )
        {
            long lVelocityMinor = abs( ( pixStep.y * pixDelta.x + pixDelta.y / 2 ) / pixDelta.y );
            pixStep.x = ( pixDelta.x > 0 ) ? lVelocityMinor : -lVelocityMinor;
            pixActual.x += pixStep.x;
        }
    }

#ifdef _DEBUG
    PixelPoint pixDeltaNew  = pixTarget - pixActual;
    long       lDistanceNew = std::max( abs( pixDeltaNew.x ), abs( pixDeltaNew.y ) );
    assert( lDistanceNew <= lDistance );
#endif

   return pixActual;
}

