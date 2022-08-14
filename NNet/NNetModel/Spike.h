// Spike.h
//
// NNetModel

#pragma once

#include <ostream>
#include "BasicTypes.h"
#include "VoltageType.h"

import MoreTypes;

class Spike
{
public:
    static mV GetVoltage
    (
        mV         const amplitude,
        fMicroSecs const usSpikeWidth,
        fMicroSecs const usSpikeTime
    )
    {
        if (usSpikeTime <= usSpikeWidth)
        {
            float const factorU { usSpikeTime / usSpikeWidth };
            mV    const mVpot   { amplitude * 4.0f * factorU * (1.0f - factorU) };
            return mVpot;
        }
        else 
            return 0.0_mV;
    }
};