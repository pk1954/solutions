// NNetParameters.cpp
//
// NNetModel

module;

#include <cassert>

module NNetParameters;

import Types;

bool Param::operator==(Param const & rhs) const
{
	return
	(m_sigGenData     == rhs.m_sigGenData    ) &&
	(m_spikeWidth     == rhs.m_spikeWidth    ) &&
	(m_pulseSpeed     == rhs.m_pulseSpeed    ) &&
	(m_threshold      == rhs.m_threshold     ) && 
	(m_neuronPeakVolt == rhs.m_neuronPeakVolt) &&
	(m_refractPeriod  == rhs.m_refractPeriod ) &&
	(m_usResolution   == rhs.m_usResolution  ) &&
	(m_usFilterSize   == rhs.m_usFilterSize  );
}

float Param::GetParameterValue(ParamType::Value const param) const
{
	switch (param)
	{
		using enum ParamType::Value;
		case inputPeakTime:  return m_sigGenData.GetPeakTime ().GetValue();
		case inputPeakFreq:  return m_sigGenData.GetFrequency().Peak().GetValue();
		case inputPeakVolt:  return m_sigGenData.GetAmplitude().Peak().GetValue();
		case inputBaseFreq:  return m_sigGenData.GetFrequency().Base().GetValue();
		case inputBaseVolt:  return m_sigGenData.GetAmplitude().Base().GetValue();
		case pulseSpeed:	 return m_pulseSpeed    .GetValue();
		case pulseWidth:     return m_spikeWidth    .GetValue();  // Legacy
		case spikeWidth:	 return m_spikeWidth    .GetValue();
		case threshold:  	 return m_threshold     .GetValue();
		case neuronPeakVolt: return m_neuronPeakVolt.GetValue();
		case refractPeriod:  return m_refractPeriod .GetValue();
		case timeResolution: return m_usResolution  .GetValue();
		case filterSize:     return m_usFilterSize  .GetValue();
		case stdPulseRate:   return 0.0f;  // Legacy
		case signalLoss:     return 0.0f;  // Legacy
		case pulseRate:      return 0.0f;  // Legacy
		case baseFrequency:  return 0.0f;  // Legacy
		default: assert(false);
	}
	return 0.f;
}

void Param::SetSigGenStaticData(SigGenStaticData const& data)
{
	m_sigGenData = data;
	NotifyAll(false);
}

void Param::SetParameterValue
(
	ParamType::Value const param, 
	float            const fNewValue 
)
{
	switch (param)
	{
		using enum ParamType::Value;
		case inputPeakTime:  m_sigGenData.SetPeakTime(static_cast<fMicroSecs >(fNewValue)); break;
		case inputPeakFreq:  m_sigGenData.SetFreqPeak(static_cast<fHertz     >(fNewValue)); break;
		case inputPeakVolt:  m_sigGenData.SetAmplPeak(static_cast<mV         >(fNewValue)); break;
		case inputBaseFreq:  m_sigGenData.SetFreqBase(static_cast<fHertz     >(fNewValue)); break;
		case inputBaseVolt:  m_sigGenData.SetAmplBase(static_cast<mV         >(fNewValue)); break;
		case pulseSpeed:	 m_pulseSpeed     = static_cast<meterPerSec>(fNewValue); break;
		case pulseWidth:              // Legacy
		case spikeWidth:	 m_spikeWidth     = static_cast<fMicroSecs >(fNewValue); break;
		case threshold:	     m_threshold      = static_cast<mV         >(fNewValue); break;
		case neuronPeakVolt: m_neuronPeakVolt = static_cast<mV         >(fNewValue); break;
		case refractPeriod:  m_refractPeriod  = static_cast<fMicroSecs >(fNewValue); break;
		case timeResolution: m_usResolution   = static_cast<fMicroSecs >(fNewValue); break;
		case filterSize:     m_usFilterSize   = static_cast<fMicroSecs >(fNewValue); break;
		case stdPulseRate:   break;  // Legacy
		case signalLoss:     break;  // Legacy
		case pulseRate:      break;  // Legacy
		case baseFrequency:  break;  // Legacy
		default: assert(false);
	}
	NotifyAll(false);
}
