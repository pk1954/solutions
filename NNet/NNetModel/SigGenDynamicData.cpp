// SigGenDynamicData.cpp
//
// NNetModel

module NNetModel:SigGenDynamicData;

import Types;
import SimulationTime;
import :SigGenStaticData;
import :NNetParameters;
import :Spike;

void SigGenDynamicData::Reset()
{
	m_usSimuStartSpike = 0.0_MicroSecs;
	StopStimulus();
}

void SigGenDynamicData::StartStimulus()
{
	m_usSimuStartStimu = SimulationTime::Get();
	m_usSimuStartSpike = 0.0_MicroSecs;
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
	NNetParameters   const & param
)
{
	return SetTime(statData, param, GetStimulusTime());
}

mV SigGenDynamicData::SetTime
(
	SigGenStaticData const & statData,
	NNetParameters   const & param,
	fMicroSecs       const   stimuTime  // time since last stimulus
)
{
	mV         const amplitude   { m_stimulusActive ? statData.GetStimulusAmplitude(stimuTime) : statData.GetAmplitude().Base() };
	fHertz     const frequency   { m_stimulusActive ? statData.GetStimulusFrequency(stimuTime) : statData.GetFrequency().Base() };
	fMicroSecs const usSpikeTime { stimuTime - m_usSimuStartSpike };
	mV         const mVresult    { Spike::GetVoltage(amplitude, param.SpikeWidth(), usSpikeTime) };

	if (m_stimulusActive && !statData.InStimulusRange(stimuTime))
		StopStimulus();

	if (usSpikeTime > PulseDuration(frequency))  // start new spike?
		m_usSimuStartSpike = stimuTime;

	return mVresult;
}
