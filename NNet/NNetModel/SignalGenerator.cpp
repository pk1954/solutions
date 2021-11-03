// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

SignalGenerator::SignalGenerator()
{
	SetBaseFrequency(fHertz(m_pParameters->GetParameterValue(ParamType::Value::stdPulseRate)));
	SetStimulusMax(1000000._MicroSecs, 500.0_fHertz);
}

void SignalGenerator::TriggerStimulus()
{
	m_bTriggered = true;
	m_usSinceLastStimulus = 0._MicroSecs;
}

fHertz const SignalGenerator::SetBaseFrequency(fHertz const freq)
{
	fHertz const fOldValue { m_fBaseFrequency };
	m_fBaseFrequency = freq;
	return fOldValue;
}

fHertz const SignalGenerator::GetFrequency()
{
	if (!m_bTriggered)
		return m_fBaseFrequency;
	fHertz freq { m_fBaseFrequency };
	m_usSinceLastStimulus += m_pParameters->TimeResolution();
	fHertz const freqStimulus { StimulusFunc(m_usSinceLastStimulus) };
	if (freqStimulus > m_fCutoffFrequency)
		freq += freqStimulus;
	else
		m_bTriggered = false;
	return freq;
}

fHertz const SignalGenerator::StimulusFunc(fMicroSecs const stimulusTime) const
{
	float  const x              { stimulusTime.GetValue() * 1e-3f };  // convert to milliseconds
	fHertz const fStimFrequency { m_fParamA * x * pow(m_fParamB, (1.0f - x)) };
	return fStimFrequency;
}

void SignalGenerator::SetStimulusMax(fMicroSecs const uSecs, fHertz const freq)
{
	float const r { 1e3f/uSecs.GetValue() };
	m_fParamA = freq * r * exp(1.0f - r);
	m_fParamB = exp(r);
}
