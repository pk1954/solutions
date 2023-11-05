// NNetParameters.ixx
//
// NNetModel

module;

#include <chrono>

export module NNetModel:NNetParameters;

import Observable;
import SaveCast;
import Types;
import Signals;
import Raster;
import :ParamType;

export class NNetParameters : public Observable
{
public:
	NNetParameters
	(
		SignalParameters * const pSigPars,
		Raster           * const pScanRaster
	)
	  : m_pSignalParameters(pSigPars),
		m_pScanRaster(pScanRaster)
	{}

	//bool operator==(NNetParameters const & rhs) const;

	float GetParameterValue(ParamType::Value const) const;
	void  SetParameterValue(ParamType::Value const, float const);

	fHertz      PulseFreqMax  () const { return m_freqMax; }
	mV          NeuronPeakVolt() const { return m_neuronPeakVolt; }
	mV          Threshold     () const { return m_threshold; }
	fMicroSecs  SynapseDelay  () const { return m_synapseDelay; }
	fMicroSecs  PulseWidth    () const { return m_pulseWidth; }
	meterPerSec PulseSpeed    () const { return m_pulseSpeed; }
	fMicroSecs  PulseDistMin  () const { return m_usPulseDistMin; }
	fMicroSecs  PixelScanTime () const { return m_usPixelScanTime; }
	fMicroSecs  TimeResolution() const { return m_pSignalParameters->TimeResolution(); }
	MicroMeter  ScanResolution() const { return m_pScanRaster->Resolution(); }
	RasterPoint ScanSize      () const { return m_pScanRaster->Size(); }
	fMicroSecs  ScanTime      () const { return m_usPixelScanTime; }

private:
	fHertz      m_freqMax         { 50.0_fHertz };
	mV          m_neuronPeakVolt  { 100._mV };
	mV          m_threshold       { 20._mV };
	fMicroSecs  m_synapseDelay    { 500._MicroSecs };
	meterPerSec m_pulseSpeed      { 120.0_meterPerSec };
	fMicroSecs  m_pulseWidth      { 2000._MicroSecs };
	fMicroSecs  m_usPulseDistMin  { PulseDuration(m_freqMax) };
	fMicroSecs  m_usPixelScanTime { 100._MicroSecs };

	Raster           *m_pScanRaster       { nullptr };
	SignalParameters *m_pSignalParameters { nullptr };
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
