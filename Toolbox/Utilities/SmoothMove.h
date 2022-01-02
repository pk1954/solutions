// SmoothMove.h
//

#pragma once

#include "PixelTypes.h"

class SmoothMove
{
public:
    PixelPoint Step(PixelPoint, PixelPoint);
    
private:
    PIXEL m_pixVelocity { 0 }; // set in every step
};