// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

SignalGenerator::SignalGenerator()
{
	SetPulseFrequency(fHertz(m_pParameters->GetParameterValue(ParamType::Value::stdPulseRate)));
}

bool SignalGenerator::operator==(SignalGenerator const & rhs) const
{
	return (m_pulseFrequency == rhs.m_pulseFrequency) &&
		(m_pulseDuration  == rhs.m_pulseDuration);
}

fHertz const SignalGenerator::SetPulseFrequency(fHertz const freq)
{
	fHertz const fOldValue { m_pulseFrequency };
	m_pulseFrequency = freq;
	m_pulseDuration  = PulseDuration(m_pulseFrequency);
	return fOldValue;
}

mV const SignalGenerator::GetPotential(fMicroSecs const usSinceLastPulse)
{
	return mV(m_pParameters->GetParameterValue(ParamType::Value::peakVoltage)) 
	    	* m_pulseFrequency.GetValue() * usSinceLastPulse.GetValue() / 1e6f;
}

fHertz const SignalGenerator::StimulusFunc(fMicroSecs const time) const
{
	float const x { time.GetValue() * 0.001f };  // convert to milliseconds
	return m_pulseFrequency + m_fMaxFrequency * x * pow(m_fBaseFactor, (1.0f - x));
}

void SignalGenerator::SetMaximum(fMicroSecs const uSecs, fHertz const freq)
{
	float const r { 1.0f/uSecs.GetValue() };
	m_fMaxFrequency = freq / exp(r - 1.0f);
	m_fBaseFactor   = exp(r);
}
