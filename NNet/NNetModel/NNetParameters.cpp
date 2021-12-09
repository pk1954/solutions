// NNetParameters.cpp
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"

bool Param::operator==(Param const & rhs) const
{
	return
	(m_stimulusMaxFreq == rhs.m_stimulusMaxFreq ) &&
	(m_stimulusMaxTime == rhs.m_stimulusMaxTime ) &&
	(m_baseFrequency   == rhs.m_baseFrequency   ) &&
	(m_pulseWidth      == rhs.m_pulseWidth      ) &&
	(m_pulseSpeed      == rhs.m_pulseSpeed      ) &&
	(m_threshold       == rhs.m_threshold       ) && 
	(m_peakVoltage     == rhs.m_peakVoltage     ) &&
	(m_refractPeriod   == rhs.m_refractPeriod   ) &&
	(m_usResolution    == rhs.m_usResolution    );
}

float Param::GetParameterValue(ParamType::Value const param) const
{
	switch (param)
	{
		case ParamType::Value::stimulusMaxFreq: return m_stimulusMaxFreq.GetValue();
		case ParamType::Value::stimulusMaxTime: return m_stimulusMaxTime.GetValue();
		case ParamType::Value::baseFrequency:   return m_baseFrequency  .GetValue();
		case ParamType::Value::pulseSpeed:	    return m_pulseSpeed     .GetValue();
		case ParamType::Value::pulseWidth:	    return m_pulseWidth     .GetValue();
		case ParamType::Value::threshold:  	    return m_threshold      .GetValue();
		case ParamType::Value::peakVoltage:	    return m_peakVoltage    .GetValue();
		case ParamType::Value::refractPeriod:   return m_refractPeriod  .GetValue();
		case ParamType::Value::timeResolution:  return m_usResolution   .GetValue();
		case ParamType::Value::signalLoss:      return 0.0f;  // Legacy
		case ParamType::Value::stdPulseRate:    return 0.0f;  // Legacy
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
	    case ParamType::Value::stimulusMaxFreq: m_stimulusMaxFreq = static_cast<fHertz     >(fNewValue); break;
	    case ParamType::Value::stimulusMaxTime: m_stimulusMaxTime = static_cast<fMicroSecs >(fNewValue); break;	    
		case ParamType::Value::baseFrequency:   m_baseFrequency   = static_cast<fHertz     >(fNewValue); break;
		case ParamType::Value::pulseSpeed:	    m_pulseSpeed      = static_cast<meterPerSec>(fNewValue); break;
		case ParamType::Value::pulseWidth:	    m_pulseWidth      = static_cast<fMicroSecs >(fNewValue); break;
		case ParamType::Value::threshold:	    m_threshold       = static_cast<mV         >(fNewValue); break;
		case ParamType::Value::peakVoltage:	    m_peakVoltage     = static_cast<mV         >(fNewValue); break;
		case ParamType::Value::refractPeriod:   m_refractPeriod   = static_cast<fMicroSecs >(fNewValue); break;
		case ParamType::Value::timeResolution:  m_usResolution    = static_cast<fMicroSecs >(fNewValue); break;
		case ParamType::Value::stdPulseRate:    break;  // Legacy
		case ParamType::Value::signalLoss:      break;  // Legacy
		default: assert(false);
	}

	NotifyAll(false);
}
