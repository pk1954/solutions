// NNetParameters.cpp
//
// NNetModel

module;

#include <cassert>

module NNetModel:NNetParameters;

import Types;
import :ParamType;

bool NNetParameters::operator==(NNetParameters const& rhs) const
{
	return
		(m_sigGenData       == rhs.m_sigGenData) &&
		(m_freqMax          == rhs.m_freqMax) &&
		(m_neuronPeakVolt   == rhs.m_neuronPeakVolt) &&
		(m_neuronThreshold  == rhs.m_neuronThreshold) &&
		(m_synapseDelay     == rhs.m_synapseDelay) &&
		(m_pulseSpeed       == rhs.m_pulseSpeed) &&
		(m_pulseWidth       == rhs.m_pulseWidth) &&
		(m_usResolution     == rhs.m_usResolution);
}

float NNetParameters::GetParameterValue(ParamType::Value const param) const
{
	switch (param)
	{
		using enum ParamType::Value;
	case inputPeakTime:   return m_sigGenData.GetPeakTime().GetValue();
	case inputPeakVolt:   return m_sigGenData.GetAmplitude().Peak().GetValue();
	case inputPeakFreq:   return m_sigGenData.GetFrequency().Peak().GetValue();
	case inputBaseFreq:   return m_sigGenData.GetFrequency().Base().GetValue();
	case inputBaseVolt:   return m_sigGenData.GetAmplitude().Base().GetValue();
	case pulseFreqMax:    return m_freqMax.GetValue();
	case neuronPeakVolt:  return m_neuronPeakVolt.GetValue();
	case neuronThreshold: return m_neuronThreshold.GetValue();
	case synapseDelay:    return m_synapseDelay.GetValue();
	case pulseSpeed:	  return m_pulseSpeed.GetValue();
	case pulseWidth:	  return m_pulseWidth.GetValue();
	case timeResolution:  return m_usResolution.GetValue();
	default: assert(false);
	}
	return 0.f;
}

void NNetParameters::SetSigGenStaticData(SigGenStaticData const& data)
{
	m_sigGenData = data;
	NotifyAll(false);
}

void NNetParameters::SetParameterValue
(
	ParamType::Value const param,
	float            const fNewValue
)
{
	switch (param)
	{
		using enum ParamType::Value;
	case pulseFreqMax:     m_freqMax = (static_cast<fHertz>(fNewValue));
                           m_usPulseDistMin = PulseDuration(m_freqMax);
                           break;
	case inputPeakTime:    m_sigGenData.SetPeakTime(static_cast<fMicroSecs> (fNewValue)); break;
	case inputPeakFreq:    m_sigGenData.SetFreqPeak(static_cast<fHertz>     (fNewValue)); break;
	case inputBaseFreq:    m_sigGenData.SetFreqBase(static_cast<fHertz>     (fNewValue)); break;
	case inputPeakVolt:    m_sigGenData.SetAmplPeak(static_cast<mV>         (fNewValue)); break;
	case inputBaseVolt:    m_sigGenData.SetAmplBase(static_cast<mV>         (fNewValue)); break;
	case pulseSpeed:	   m_pulseSpeed       =     static_cast<meterPerSec>(fNewValue);  break;
	case pulseWidth:	   m_pulseWidth       =     static_cast<fMicroSecs> (fNewValue);  break;
	case neuronThreshold:  m_neuronThreshold  =     static_cast<mV>         (fNewValue);  break;
	case synapseThreshold:                                                                break;
	case synapseDelay:     m_synapseDelay     =     static_cast<fMicroSecs> (fNewValue);  break;
	case neuronPeakVolt:   m_neuronPeakVolt   =     static_cast<mV>         (fNewValue);  break;
	case timeResolution:   m_usResolution     =     static_cast<fMicroSecs> (fNewValue);  break;
	default: assert(false);
	}
	NotifyAll(false);
}
