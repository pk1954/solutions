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
		if (m_usSinceLastStimulus > m_usCutoffTime)
			m_bStimulusActive = false;
		else
			m_fActFrequency += StimulusFunc(m_usSinceLastStimulus);
	}
}

fHertz const SignalGenerator::StimulusFunc(fMicroSecs const stimulusTime) const
{
	if (stimulusTime > m_usCutoffTime)
		return 0.0_fHertz;
	float  const x { stimulusTime.GetValue() * 1e-3f };  // convert to milliseconds
	return m_fParamA * x * pow(m_fParamB, (1.0f - x));
}

float const SignalGenerator::StimulusIntegral(fMicroSecs const stimulusTime) const
{
	fMicroSecs const time { (stimulusTime < m_usCutoffTime)	? stimulusTime : m_usCutoffTime };
	float      const x    { time.GetValue() * 1e-3f };  // convert to milliseconds
	float      const lnB  { log(m_fParamB) };
	float      const C    { m_fParamA.GetValue() * m_fParamB/(lnB*lnB) };
	float      const fRes { C - m_fParamA.GetValue() * (lnB * x + 1) * pow(m_fParamB, (1.0f - x))/(lnB*lnB) };
	return fRes * 1e-3f;
}

fHertz const SignalGenerator::GetFrequency(fMicroSecs const uSecs) const
{
	return m_fBaseFrequency + StimulusFunc(uSecs);
}

void SignalGenerator::SetStimulusMax(fMicroSecs uSecs, fHertz freq)
{
	freq -= m_fBaseFrequency;
	if (uSecs < 0._MicroSecs)
		uSecs = 0._MicroSecs;
	if (freq < 0._fHertz)
		freq = 0._fHertz;
	float const r { 1e3f/uSecs.GetValue() };
	m_fParamA = freq * r * exp(1.0f - r);
	m_fParamB = exp(r);
	m_usCutoffTime = GetPeakTime() * m_fCutoffFactor;
}

fMicroSecs const SignalGenerator::GetPeakTime() const
{
	return fMicroSecs(1e3f/log(m_fParamB));
}
