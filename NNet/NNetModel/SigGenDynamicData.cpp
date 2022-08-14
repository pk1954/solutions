// SigGenDynamicData.cpp
//
// NNetModel

#pragma once

#include "Spike.h"
#include "NNetParameters.h"
#include "SimulationTime.h"
#include "SignalGenerator.h"
#include "SigGenDynamicData.h"
#include "VoltageType.h"

void SigGenDynamicData::Reset()
{
	m_usSimuStartSpike = 0.0_MicroSecs;
	StopStimulus();
}

void SigGenDynamicData::StartSpike()
{
	m_usSimuStartSpike = SimulationTime::Get();
}

void SigGenDynamicData::StartStimulus()
{
	m_usSimuStartStimu = SimulationTime::Get();
	m_stimulusActive = true;
}

void SigGenDynamicData::StopStimulus()
{
	m_usSimuStartStimu = 0.0_MicroSecs;
	m_stimulusActive = false;
}

fMicroSecs SigGenDynamicData::GetStimulusTime() const
{
	return SimulationTime::Get() - m_usSimuStartStimu;
}

mV SigGenDynamicData::SetTime
(
	SigGenStaticData const & statData,
	Param            const & param
)
{
	return SetTime(statData, param, GetStimulusTime());
}

mV SigGenDynamicData::SetTime
(
	SigGenStaticData const & statData,
	Param            const & param,
	fMicroSecs       const   stimuTime  // time since last stimulus
)
{
	mV         const amplitude   { m_stimulusActive ? statData.GetStimulusAmplitude(stimuTime) : statData.GetAmplitude().Base() };
	fHertz     const frequency   { m_stimulusActive ? statData.GetStimulusFrequency(stimuTime) : statData.GetFrequency().Base() };
	fMicroSecs const usSpikeTime { SimulationTime::Get() - m_usSimuStartSpike };
	mV         const mVresult    { Spike::GetVoltage(amplitude, param.SpikeWidth(), usSpikeTime) };

	if (m_stimulusActive && !statData.InStimulusRange(stimuTime))
		StopStimulus();

	if (usSpikeTime > PulseDuration(frequency))  // start new spike?
		StartSpike();

	return mVresult;
}
