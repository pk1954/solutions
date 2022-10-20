// NNetParameters.cpp
//
// NNetModel

module;

#include <cassert>

module NNetModel:NNetParameters;

import Types;
import :ParamType;

bool NNetParameters::operator==(NNetParameters const & rhs) const
{
	return
	(m_sigGenData      == rhs.m_sigGenData     ) &&
	(m_spikeWidth      == rhs.m_spikeWidth     ) &&
	(m_pulseSpeed      == rhs.m_pulseSpeed     ) &&
	(m_neuronThreshold == rhs.m_neuronThreshold) && 
	(m_neuronPeakVolt  == rhs.m_neuronPeakVolt ) &&
	(m_refractPeriod   == rhs.m_refractPeriod  ) &&
	(m_usResolution    == rhs.m_usResolution   );
}

float NNetParameters::GetParameterValue(ParamType::Value const param) const
{
	switch (param)
	{
		using enum ParamType::Value;
		case inputPeakTime:    return m_sigGenData.GetPeakTime ().GetValue();
		case inputPeakFreq:    return m_sigGenData.GetFrequency().Peak().GetValue();
		case inputPeakVolt:    return m_sigGenData.GetAmplitude().Peak().GetValue();
		case inputBaseFreq:    return m_sigGenData.GetFrequency().Base().GetValue();
		case inputBaseVolt:    return m_sigGenData.GetAmplitude().Base().GetValue();
		case pulseSpeed:	   return m_pulseSpeed      .GetValue();
		case spikeWidth:	   return m_spikeWidth      .GetValue();
		case neuronThreshold:  return m_neuronThreshold .GetValue();
		case synapseThreshold: return m_synapseThreshold.GetValue();
		case synapseBlockTime: return m_synapseBlockTime.GetValue();
		case neuronPeakVolt:   return m_neuronPeakVolt  .GetValue();
		case refractPeriod:    return m_refractPeriod   .GetValue();
		case timeResolution:   return m_usResolution    .GetValue();
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
		case inputPeakTime:    m_sigGenData.SetPeakTime(static_cast<fMicroSecs> (fNewValue)); break;
		case inputPeakFreq:    m_sigGenData.SetFreqPeak(static_cast<fHertz>     (fNewValue)); break;
		case inputPeakVolt:    m_sigGenData.SetAmplPeak(static_cast<mV>         (fNewValue)); break;
		case inputBaseFreq:    m_sigGenData.SetFreqBase(static_cast<fHertz>     (fNewValue)); break;
		case inputBaseVolt:    m_sigGenData.SetAmplBase(static_cast<mV>         (fNewValue)); break;
		case pulseSpeed:	   m_pulseSpeed       =     static_cast<meterPerSec>(fNewValue);  break;
		case spikeWidth:	   m_spikeWidth       =     static_cast<fMicroSecs> (fNewValue);  break;
		case neuronThreshold:  m_neuronThreshold  =     static_cast<mV>         (fNewValue);  break;
		case synapseThreshold: m_synapseThreshold =     static_cast<mV>         (fNewValue);  break;
		case synapseBlockTime: m_synapseBlockTime =     static_cast<fMicroSecs> (fNewValue);  break;
		case neuronPeakVolt:   m_neuronPeakVolt   =     static_cast<mV>         (fNewValue);  break;
		case refractPeriod:    m_refractPeriod    =     static_cast<fMicroSecs> (fNewValue);  break;
		case timeResolution:   m_usResolution     =     static_cast<fMicroSecs> (fNewValue);  break;
		default: assert(false);
	}
	NotifyAll(false);
}
