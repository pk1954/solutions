// SmoothMove.h
//

#pragma once

#include "pixelPoint.h"

class SmoothMove
{
public:
    SmoothMove( );

    PixelPoint Step( PixelPoint, PixelPoint );

private:
    long m_lVelocity; // set in every step
};