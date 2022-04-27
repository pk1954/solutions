// ActionPotential.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

inline mV ActionPotential
(
    fMicroSecs const spikeTime,  // relative to start of spike
    mV         const amplitude,
    fMicroSecs const umSpikeWidth
)
{
    if (spikeTime <= umSpikeWidth)
    {
        float const factorU { spikeTime / umSpikeWidth };
        mV    const mVpot   { amplitude * 4.0f * factorU * (1.0f - factorU) };
        return mVpot;
    }
    else 
        return 0.0_mV;
}
