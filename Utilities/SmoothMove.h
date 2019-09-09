// SmoothMove.h
//

#pragma once

#include "PixelTypes.h"

class SmoothMove
{
public:
    SmoothMove( );

    PixelPoint Step( PixelPoint, PixelPoint );

private:
    PIXEL m_pixVelocity; // set in every step
};