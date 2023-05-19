// Spike.ixx
//
// Signals

module;

#include <iostream>

export module Signals:Spike;

import Types;

export class Spike
{
public:
    static mV GetVoltage
    (
        mV         const amplitude,
        fMicroSecs const usPulseWidth,
        fMicroSecs const usSpikeTime
    )
    {
        if (usSpikeTime <= usPulseWidth)
        {
            float const factorU { usSpikeTime / usPulseWidth };
            mV    const mVpot   { amplitude * 4.0f * factorU * (1.0f - factorU) };
            return mVpot;
        }
        else
            return 0.0_mV;
    }
};