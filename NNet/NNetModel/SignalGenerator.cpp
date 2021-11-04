// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

SignalGenerator::SignalGenerator()
{
	SetBaseFrequency(fHertz(m_pParameters->GetParameterValue(ParamType::Value::stdPulseRate)));
	SetStimulusMax(500000._MicroSecs, 50.0_fHertz);
	m_fActFrequency = m_fBaseFrequency;
}

void SignalGenerator::TriggerStimulus()
{
	m_bStimulusActive = true;
	m_usSinceLastStimulus = 0._MicroSecs;
}

fHertz const SignalGenerator::SetBaseFrequency(fHertz const freq)
{
	fHertz const fOldValue { m_fBaseFrequency };
	m_fBaseFrequency = freq;
	return fOldValue;
}

void SignalGenerator::Tick()
{
	m_fActFrequency = m_fBaseFrequency;
	if (m_bStimulusActive)
	{
		m_usSinceLastStimulus += m_pParameters->TimeResolution();
		fHertz const freqStimulus { StimulusFunc(m_usSinceLastStimulus) };
		if (freqStimulus > m_fCutoffFrequency)
			m_fActFrequency += freqStimulus;
		else
			m_bStimulusActive = false;
	}
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
