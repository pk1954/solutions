// SigGenDynamicData.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "Spike.h"

class SignalGenerator;
class Param;

class SigGenDynamicData
{
public:

	void Reset();
	void StartStimulus();

	fMicroSecs GetStimulusTime() const;

	mV CalcVoltage
	(
		SignalGenerator const &,
		Param           const &
	);

	mV CalcVoltage
	(
		SignalGenerator const &,
		Param           const &,
		fMicroSecs      const
	);

	bool IsStimulusActive() const { return m_stimulusActive; }

private:
	void reset();

	bool       m_stimulusActive   { false };
	fMicroSecs m_usStimuLastSpike { 0.0_MicroSecs }; // in stimulus time
	fMicroSecs m_usSimuStartStimu { 0.0_MicroSecs }; // in simu time
	Spike      m_spike            { };
};