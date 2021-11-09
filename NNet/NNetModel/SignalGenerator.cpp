// SignalGenerator.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetParameters.h"
#include "SignalGenerator.h"

void SignalGenerator::TriggerStimulus()
{
	m_usSinceLastStimulus = 0._MicroSecs;
}

fHertz const SignalGenerator::GetFrequency(fMicroSecs const uSecs) const
{
	fMicroSecs const usMax { m_pParameters->StimulusMaxTime() };
	if (uSecs < usMax * CUT_OFF_FACTOR)
	{
		fHertz const freqMax  { m_pParameters->StimulusMaxFreq() };
		float  const fFactor  { uSecs / usMax };
		fHertz const freqStim { freqMax * exp(1 - fFactor) * fFactor };
		return m_pParameters->BaseFrequency() + freqStim;
	}
	else
		return m_pParameters->BaseFrequency();
}

fHertz const SignalGenerator::GetActFrequency () const 
{ 
	return GetFrequency(m_usSinceLastStimulus);
}

void SignalGenerator::Tick()
{
	m_usSinceLastStimulus += m_pParameters->TimeResolution();
}
