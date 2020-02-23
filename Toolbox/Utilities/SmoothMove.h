// SmoothMove.h
//

#pragma once

#include "PixelTypes.h"

class SmoothMove
{
public:
    SmoothMove( );

    PixelPoint Step( PixelPoint, PixelPoint );
    MicroMeter Step( MicroMeter, MicroMeter );
    
private:
    PIXEL      m_pixVelocity; // set in every step
    MicroMeter m_umVelocity;  //
};