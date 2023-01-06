// NNetParameters.ixx
//
// NNetModel

module;

#include <chrono>

export module NNetModel:NNetParameters;

import Observable;
import SaveCast;
import Types;
import :ParamType;
import :SigGenStaticData;

export class NNetParameters : public Observable
{
public:

	bool operator==(NNetParameters const & rhs) const;

	float GetParameterValue(ParamType::Value const) const;
	void  SetParameterValue(ParamType::Value const, float const);

	SigGenStaticData const & GetSigGenStaticData() const { return m_sigGenData; }

	void SetSigGenStaticData(SigGenStaticData const&);

	fHertz      PulseFreqMax    () const { return m_freqMax; }
	mV          NeuronPeakVolt  () const { return m_neuronPeakVolt; }
	mV          NeuronThreshold () const { return m_neuronThreshold; }
	mV          SynapseThreshold() const { return m_synapseThreshold; }
	fMicroSecs  SpikeWidth      () const { return m_spikeWidth; }
	meterPerSec PulseSpeed      () const { return m_pulseSpeed; }
	fMicroSecs  TimeResolution  () const { return m_usResolution; }
	fMicroSecs  PulseDistMin    () const { return m_usPulseDistMin; }

private:
	SigGenStaticData m_sigGenData       { };
	fHertz           m_freqMax          { 50.0_fHertz };
	mV               m_neuronPeakVolt   { 20._mV };
	mV               m_neuronThreshold  { 20._mV };
	mV               m_synapseThreshold { 20._mV };
	meterPerSec      m_pulseSpeed       { 120.0_meterPerSec };
	fMicroSecs       m_spikeWidth       { 2000._MicroSecs };
	fMicroSecs       m_usResolution     { 100._MicroSecs };
	fMicroSecs       m_usPulseDistMin   { PulseDuration(m_freqMax) };
};

// geometry
export inline float         const NEURON_INTERIOR { 0.8f };                // Neurons of all kinds have 80% interior, 20 % border 
export inline float         const PIPE_INTERIOR   { 0.6f };                // Pipes have 60% interior, 40 % border 
export inline MicroMeter    const NEURON_RADIUS   { 50.0_MicroMeter };
export inline MicroMeter    const PIPE_WIDTH      { 20.0_MicroMeter };
export inline MicroMeter    const PIPE_WIDTH_HIGH { 30.0_MicroMeter };
export inline MicroMeter    const KNOT_WIDTH      { PIPE_WIDTH / 2 };
export inline MicroMeterPnt const STD_OFFSET      { MicroMeterPnt(0._MicroMeter, NEURON_RADIUS * 2.0) };
export inline MicroMeter    const MICRO_OFFSET    { 1.0_MicroMeter };

export unsigned int CalcNrOfSteps(MicroMeter const um)
{
	static MicroMeter const umPerStep { NEURON_RADIUS / 5.0f };
	return Cast2UnsignedInt(um / umPerStep);
}

export unsigned int CalcNrOfSteps(Radian const rad)
{
	static Radian const radPerStep{ Degrees2Radian(6.0_Degrees) };
	return Cast2UnsignedInt(rad.GetAbs() / radPerStep);
}
