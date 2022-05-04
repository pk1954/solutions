// Spike.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

class Spike
{
public:
    void Reset()
    {
        m_usSpikeTime = 0.0_MicroSecs;
    }

    void Tick(fMicroSecs const usInc)
    {
        m_usSpikeTime += usInc;
    }

    fMicroSecs SpikeTime() const
    { 
        return m_usSpikeTime; 
    };

    mV GetVoltage
    (
        fMicroSecs const spikeTime,  // relative to start of spike
        mV         const amplitude,
        fMicroSecs const usSpikeWidth
    ) const
    {
        if (spikeTime <= usSpikeWidth)
        {
            float const factorU { spikeTime / usSpikeWidth };
            mV    const mVpot   { amplitude * 4.0f * factorU * (1.0f - factorU) };
            return mVpot;
        }
        else 
            return 0.0_mV;
    }

private:
    fMicroSecs m_usSpikeTime { 0._MicroSecs };
};