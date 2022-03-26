// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

SignalGenerator::SignalGenerator(Param & param) 
  : m_pParameters(&param),
	m_data
	( 
		SigGenData
		(
			param.InputFreq(), 
			param.InputVolt(), 
			param.InputPeakTime()
		)
	)
{
}

void SignalGenerator::SetData(SigGenData const & data) 
{ 
	m_data = data; 
	NotifyAll(false);
}

SigGenData SignalGenerator::GetData() const
{ 
	return m_data; 
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
		float  const fFactor   { uSecs / m_data.usPeak };
		fHertz const freqDelta { m_data.freq.Peak() - m_data.freq.Base() };
		fHertz const freqStim  { freqDelta * stimulusFunc(fFactor) };
		return m_data.freq.Base() + freqStim;
	}
	else 
		return m_data.freq.Base();
}

fHertz SignalGenerator::GetActFrequency() const 
{ 
	return GetFrequency(m_usSinceLastStimulus);
}

mV SignalGenerator::GetVoltage(fMicroSecs const uSecs) const
{
	if (InStimulusRange(uSecs))
	{
		float const fFactor   { uSecs / m_data.usPeak };
		mV    const voltDelta { m_data.volt.Peak() - m_data.volt.Base() };
		mV    const voltStim  { voltDelta * stimulusFunc(fFactor) };
		return m_data.volt.Base() + voltStim;
	}
	else
		return m_data.volt.Base();
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

void SignalGenerator::SetParam(ParamType::Value const par, float const f)
{
	switch (par)
	{
		using enum ParamType::Value;
		case baseFrequency: m_data.freq.SetBase(fHertz(f)); break;
		case inputPeakFreq: m_data.freq.SetPeak(fHertz(f)); break;
		case inputBaseVolt: m_data.volt.SetBase(mV(f));     break;
		case inputPeakVolt: m_data.volt.SetPeak(mV(f));     break;
		case inputPeakTime: m_data.usPeak = f;              break;
		default: assert(false);
	}
	NotifyAll(false);
}

void SignalGenerator::SetParams
(
	BASE_PEAK<fHertz> freq,
	BASE_PEAK<mV>     volt,
	fMicroSecs        usPeak
)
{
	m_data.freq   = freq;
	m_data.usPeak = usPeak;
	m_data.volt   = volt;
	NotifyAll(false);
}

void SignalGenerator::SetBaseFreq(fHertz const f) 
{ 
	m_data.freq.SetBase(max(0._fHertz, f));
	NotifyAll(false);
}

void SignalGenerator::SetPeakFreq(fHertz const f)
{
	m_data.freq.SetPeak(max(0._fHertz, f));
	NotifyAll(false);
}

void SignalGenerator::SetTimePeak(fMicroSecs const t)
{
	m_data.usPeak = max(1._MicroSecs, t);
	NotifyAll(false);
}

void SignalGenerator::SetBaseVolt(mV const v)
{
	m_data.volt.SetBase(max(0._mV, v));
	NotifyAll(false);
}

void SignalGenerator::SetPeakVolt(mV const v)
{
	m_data.volt.SetPeak(max(0._mV, v));
	NotifyAll(false);
}
