// SignalGenerator.cpp 
//
// NNetModel

#include <cassert>
#include "NNetParameters.h"
#include "UPSigGenList.h"
#include "SignalGenerator.h"
#include "VoltageType.h"

using std::to_wstring;

SignalGenerator::SignalGenerator(wstring const & name)
  : m_name(name)
{}

void SignalGenerator::Register(ObserverInterface & obs)
{
	m_statData.RegisterObserver(obs);
}

void SignalGenerator::Unregister(ObserverInterface const & obs)
{
	m_statData.UnregisterObserver(obs);
}

void SignalGenerator::SetStaticData(SigGenStaticData const & data) 
{ 
	m_statData = data; 
}

SigGenStaticData const & SignalGenerator::GetStaticData() const
{ 
	return m_statData; 
}

fHertz SignalGenerator::GetStimulusFrequency(fMicroSecs const stimulusTime) const 
{ 
	return m_statData.GetStimulusFrequency(stimulusTime); 
}

mV SignalGenerator::GetStimulusAmplitude(fMicroSecs const stimulusTime) const 
{	
	return m_statData.GetStimulusAmplitude(stimulusTime); 
}

void SignalGenerator::SetParam(ParamType::Value const par, float const f)
{
	switch (par)
	{
		using enum ParamType::Value;
		case baseFrequency: m_statData.SetFreqBase(fHertz(f));     break;
		case inputPeakFreq: m_statData.SetFreqPeak(fHertz(f));     break;
		case inputBaseVolt: m_statData.SetAmplBase(mV(f));         break;
		case inputPeakVolt: m_statData.SetAmplPeak(mV(f));         break;
		case inputPeakTime: m_statData.SetPeakTime(fMicroSecs(f)); break;
		default: assert(false);
	}
}

void SignalGenerator::Prepare(Param const & param)
{
	m_mVactual = m_dynData.SetTime(m_statData, param);
}