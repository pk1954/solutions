// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

SignalGenerator::SignalGenerator()
{
	SetPulseFrequency(STD_PULSE_FREQ);
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
		/ m_pulseDuration.GetValue() * usSinceLastPulse.GetValue();
}

fHertz const SignalGenerator::StimulusFunc(fMicroSecs const time) const
{
	float const x { time.GetValue() * 0.001f };
	return fHertz(m_A * x * pow(m_B, (1.0f - x)));
}

void SignalGenerator::SetMaximum(fMicroSecs const uSecs, fHertz const hertz)
{
	float const x { uSecs.GetValue() };
	float const y { hertz.GetValue() };
	float const r { 1.0f/x };
	m_A = y / exp(r - 1.0f);
	m_B = exp(r);
}
