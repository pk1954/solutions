// NNetParameters.cpp
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"

bool Param::operator==(Param const & rhs) const
{
	return
	(m_inputPeakTime  == rhs.m_inputPeakTime ) &&
	(m_inputFreq      == rhs.m_inputFreq     ) &&
	(m_inputVolt      == rhs.m_inputVolt     ) &&
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
		case inputPeakTime:  return m_inputPeakTime .GetValue();
		case inputPeakFreq:  return m_inputFreq.Peak().GetValue();
		case inputPeakVolt:  return m_inputVolt.Peak().GetValue();
		case inputBaseFreq:  return m_inputFreq.Base().GetValue();
		case inputBaseVolt:  return m_inputVolt.Base().GetValue();
		case pulseSpeed:	 return m_pulseSpeed    .GetValue();
		case pulseWidth:                   // Legacy
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

void Param::SetParameterValue
(
	ParamType::Value const param, 
	float            const fNewValue 
)
{
	switch (param)
	{
		using enum ParamType::Value;
		case inputPeakTime:  m_inputPeakTime  = static_cast<fMicroSecs >(fNewValue); break;	    
		case inputPeakFreq:  m_inputFreq.SetPeak(static_cast<fHertz    >(fNewValue)); break;
		case inputPeakVolt:  m_inputVolt.SetPeak(static_cast<mV        >(fNewValue)); break;
		case inputBaseFreq:  m_inputFreq.SetBase(static_cast<fHertz    >(fNewValue)); break;
		case inputBaseVolt:  m_inputVolt.SetBase(static_cast<mV        >(fNewValue)); break;
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
