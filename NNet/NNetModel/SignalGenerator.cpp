// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "UPSigGenList.h"
#include "SignalGenerator.h"

using std::to_wstring;

SignalGenerator::SignalGenerator(wstring const & name)
  : m_name(name)
{}

void SignalGenerator::Register(ObserverInterface * const pObs)
{
	m_statData.RegisterObserver(*pObs);
	RegisterObserver(*pObs);
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
