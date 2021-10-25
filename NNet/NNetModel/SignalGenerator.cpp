// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

SignalGenerator::SignalGenerator()
{
	SetBaseFrequency(fHertz(m_pParameters->GetParameterValue(ParamType::Value::stdPulseRate)));
	SetStimulusMax(2000._MicroSecs, 50.0_fHertz);
}

//bool SignalGenerator::operator==(SignalGenerator const & rhs) const
//{
//	return m_fBaseFrequency == rhs.m_fBaseFrequency;
//}

void SignalGenerator::SetStimulus()
{
	m_usSinceLastStimulus = 0._MicroSecs;
}

fHertz const SignalGenerator::SetBaseFrequency(fHertz const freq)
{
	fHertz const fOldValue { m_fBaseFrequency };
	m_fBaseFrequency = freq;
	return fOldValue;
}

mV const SignalGenerator::GetPotIncrease()
{
	m_usSinceLastStimulus += m_pParameters->GetTimeResolution();

	fHertz     const freq     { StimulusFunc(m_usSinceLastStimulus) };
	fMicroSecs const us2Fill  { PulseDuration(freq) };
	float      const fillRate { us2Fill / m_pParameters->GetTimeResolution() };
	mV         const potInc   { fillRate * m_pParameters->GetParameterValue(ParamType::Value::peakVoltage) };
	return potInc;
}

fHertz const SignalGenerator::StimulusFunc(fMicroSecs const stimulusTime) const
{
	float  const x              { stimulusTime.GetValue() * 1e-3f };  // convert to milliseconds
	fHertz const fStimFrequency { m_fParamA * x * pow(m_fParamB, (1.0f - x)) };
	return m_fBaseFrequency + fStimFrequency;
}

void SignalGenerator::SetStimulusMax(fMicroSecs const uSecs, fHertz const freq)
{
	float const r { 1e3f/uSecs.GetValue() };
	m_fParamA = freq * r * exp(1.0f - r);
	m_fParamB = exp(r);
}
