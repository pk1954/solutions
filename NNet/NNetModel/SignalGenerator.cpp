// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

SignalGenerator::SignalGenerator()
{
	LoadParameterValues();
}

void SignalGenerator::TriggerStimulus()
{
	m_usSinceLastStimulus = 0._MicroSecs;
	m_bTriggerActive = true;
	NotifyAll(false);
}

fHertz SignalGenerator::GetFrequency(fMicroSecs const uSecs) const
{
	if (InStimulusRange(uSecs))
	{
		float  const fFactor  { uSecs / m_usMax };
		fHertz const freqStim { m_freqMaxStim * exp(1 - fFactor) * fFactor };
		return m_freqBase + freqStim;
	}
	else
		return m_freqBase;
}

fHertz SignalGenerator::GetActFrequency() const 
{ 
	return GetFrequency(m_usSinceLastStimulus);
}

void SignalGenerator::Tick()
{
	m_usSinceLastStimulus += m_pParameters->TimeResolution();
	if (m_bTriggerActive && ! InStimulusRange(m_usSinceLastStimulus))
		StopTrigger();
	NotifyAll(false);
}

void SignalGenerator::LoadParameterValues()
{
	m_freqBase    = m_pParameters->BaseFrequency();
	m_freqMaxStim = m_pParameters->StimulusMaxFreq();
	m_usMax       = m_pParameters->StimulusMaxTime();
	NotifyAll(false);
}

void SignalGenerator::SetParam(ParamType::Value const par, float const f)
{
	switch (par)
	{
		using enum ParamType::Value;
		case stimulusMaxFreq: m_freqMaxStim = f; break;
		case stimulusMaxTime: m_usMax       = f; break;
		case baseFrequency:   m_freqBase    = f; break;
		case peakVoltage:     m_mVpeak      = f; break;
		default: assert(false);
	}
	NotifyAll(false);
}

void SignalGenerator::SetParams(SignalGenerator const& src)
{
	m_freqBase    = src.m_freqBase;
	m_freqMaxStim = src.m_freqMaxStim;
	m_usMax       = src.m_usMax;
	m_mVpeak      = src.m_mVpeak;
	NotifyAll(false);
}

void SignalGenerator::SetFreqBase(fHertz const f) 
{ 
	m_freqBase = max(0._fHertz, f);
	NotifyAll(false);
}

void SignalGenerator::SetFreqMax(fHertz const f)
{
	m_freqMaxStim = max(0._fHertz, f);
	NotifyAll(false);
}

void SignalGenerator::SetTimeMax(fMicroSecs const t)
{
	m_usMax = max(1._MicroSecs, t);
	NotifyAll(false);
}

void SignalGenerator::SetPeakVoltage(mV const v)
{
	m_mVpeak = max(0._mV, v);
	NotifyAll(false);
}
