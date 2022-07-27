// SmoothMove.ixx
//

module;

#pragma once

#include "PixelTypes.h"

export module SmoothMove;

export class SmoothMove
{
public:
    PixelPoint Step(PixelPoint, PixelPoint);

private:
    PIXEL m_pixVelocity { 0 }; // set in every step
};