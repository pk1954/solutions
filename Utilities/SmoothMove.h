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
    PIXEL m_pixVelocity; // set in every step
};