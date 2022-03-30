// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

SignalGenerator::SignalGenerator(Param & param) 
  : m_pParameters(&param),
	m_data(param.GetSigGenData())
{}

void SignalGenerator::Register(ObserverInterface & obs)
{
	m_data.RegisterObserver(obs);
	m_stimulus.RegisterObserver(obs);
}

void SignalGenerator::Unregister(ObserverInterface & obs)
{
	m_data.UnregisterObserver(obs);
	m_stimulus.UnregisterObserver(obs);
}

void SignalGenerator::SetData(SigGenData const & data) 
{ 
	m_data = data; 
	m_data.NotifyAll(false);
}

SigGenData SignalGenerator::GetData() const
{ 
	return m_data; 
}

void SignalGenerator::TriggerStimulus()
{
	m_stimulus.TriggerStimulus();
}

void SignalGenerator::Tick()
{
	m_stimulus.Tick(m_pParameters->TimeResolution());
	if (m_stimulus.IsTriggerActive() && ! m_data.InStimulusRange(m_stimulus.TimeTilTrigger()))
		StopTrigger();
}

void SignalGenerator::SetParam(ParamType::Value const par, float const f)
{
	switch (par)
	{
		using enum ParamType::Value;
		case baseFrequency: m_data.SetFreqBase(fHertz(f));     break;
		case inputPeakFreq: m_data.SetFreqPeak(fHertz(f));     break;
		case inputBaseVolt: m_data.SetAmplBase(mV(f));         break;
		case inputPeakVolt: m_data.SetAmplPeak(mV(f));         break;
		case inputPeakTime: m_data.SetPeakTime(fMicroSecs(f)); break;
		default: assert(false);
	}
}

void SignalGenerator::SetFreqBase(fHertz const f) 
{ 
	m_data.SetFreqBase(max(0._fHertz, f));
}

void SignalGenerator::SetFreqPeak(fHertz const f)
{
	m_data.SetFreqPeak(max(0._fHertz, f));
}

void SignalGenerator::SetTimePeak(fMicroSecs const t)
{
	m_data.SetPeakTime(max(1._MicroSecs, t));
}

void SignalGenerator::SetBaseVolt(mV const v)
{
	m_data.SetAmplBase(max(0._mV, v));
}

void SignalGenerator::SetPeakVolt(mV const v)
{
	m_data.SetAmplPeak(max(0._mV, v));
}
