// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "UPSigGenList.h"
#include "SignalGenerator.h"

using std::to_wstring;

SignalGenerator::SignalGenerator(UPSigGenList & list)
  : m_list(list),
	m_name(list.GenerateUniqueName())
{}

SignalGenerator::SignalGenerator(UPSigGenList & list, wstring const & name)
  : m_list(list),
	m_name(name)
{}

void SignalGenerator::Register(ObserverInterface * const pObs)
{
	m_data.RegisterObserver(*pObs);
	RegisterObserver(*pObs);
}

void SignalGenerator::SetData(SigGenData const & data) 
{ 
	m_data = data; 
}

SigGenData const & SignalGenerator::GetData() const
{ 
	return m_data; 
}

fHertz SignalGenerator::GetFrequency(fMicroSecs const stimulusTime) const 
{ 
	return m_data.GetFrequency(stimulusTime); 
}

mV SignalGenerator::GetAmplitude(fMicroSecs const stimulusTime) const 
{	
	return m_data.GetAmplitude(stimulusTime); 
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

bool SignalGenerator::IsStimulusActive() const
{ 
	return m_data.InStimulusRange(GetStimulusTime());
}

void SignalGenerator::StartStimulus()
{
	m_usStartLastSpike = 0.0_MicroSecs;
	m_usStartStimuTime = SimulationTime::Get();
//	NotifyAll(false);
}

fMicroSecs SignalGenerator::GetStimulusTime() const // time til last stimulus start  
{ 
	return SimulationTime::Get() - m_usStartStimuTime; 
}

mV SignalGenerator::CalcVoltage
(
	fMicroSecs const stimulusTime,
	fMicroSecs const spikeWidth
)
{
	fMicroSecs const spikeTimeMax { SpikeTimeMax(GetFrequency(stimulusTime)) };
	fMicroSecs const spikeTime    { stimulusTime - m_usStartLastSpike };
	mV         const amplitude    { GetAmplitude(stimulusTime) };
	if (spikeTime > spikeTimeMax)
		m_usStartLastSpike = stimulusTime;
	return ActionPotential(spikeTime, amplitude, spikeWidth);
}
