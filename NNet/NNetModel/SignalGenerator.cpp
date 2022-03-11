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
		float  const fFactor   { uSecs / m_usPeak };
		fHertz const freqDelta { m_freq.peak - m_freq.base };
		fHertz const freqStim  { freqDelta * stimulusFunc(fFactor) };
		return m_freq.base + freqStim;
	}
	else 
		return m_freq.base;
}

fHertz SignalGenerator::GetActFrequency() const 
{ 
	return GetFrequency(m_usSinceLastStimulus);
}

mV SignalGenerator::GetVoltage(fMicroSecs const uSecs) const
{
	if (InStimulusRange(uSecs))
	{
		float const fFactor   { uSecs / m_usPeak };
		mV    const voltDelta { m_volt.peak - m_volt.base };
		mV    const voltStim  { voltDelta * stimulusFunc(fFactor) };
		return m_volt.base + voltStim;
	}
	else
		return m_volt.base;
}

mV SignalGenerator::GetActVoltage() const
{
	return GetVoltage(m_usSinceLastStimulus);
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
	m_usPeak = m_pParameters->InputPeakTime();
	m_freq   = m_pParameters->InputFreq();
	m_volt   = m_pParameters->InputVolt();
	NotifyAll(false);
}

void SignalGenerator::SetParam(ParamType::Value const par, float const f)
{
	switch (par)
	{
		using enum ParamType::Value;
		case baseFrequency: m_freq.base = f; break;
		case inputPeakFreq: m_freq.peak = f; break;
		case inputBaseVolt: m_volt.base = f; break;
		case inputPeakVolt: m_volt.peak = f; break;
		case inputPeakTime: m_usPeak    = f; break;
		default: assert(false);
	}
	NotifyAll(false);
}

void SignalGenerator::SetParams(SignalGenerator const& src)
{
	m_freq   = src.m_freq;
	m_usPeak = src.m_usPeak;
	m_volt   = src.m_volt;
	NotifyAll(false);
}

void SignalGenerator::SetBaseFreq(fHertz const f) 
{ 
	m_freq.base = max(0._fHertz, f);
	NotifyAll(false);
}

void SignalGenerator::SetPeakFreq(fHertz const f)
{
	m_freq.peak = max(0._fHertz, f);
	NotifyAll(false);
}

void SignalGenerator::SetTimePeak(fMicroSecs const t)
{
	m_usPeak = max(1._MicroSecs, t);
	NotifyAll(false);
}

void SignalGenerator::SetBaseVolt(mV const v)
{
	m_volt.base = max(0._mV, v);
	NotifyAll(false);
}

void SignalGenerator::SetPeakVolt(mV const v)
{
	m_volt.peak = max(0._mV, v);
	NotifyAll(false);
}
