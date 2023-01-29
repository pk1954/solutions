// SigGenDynamicData.ixx
//
// NNetModel

export module NNetModel:SigGenDynamicData;

import Types;
import :SigGenStaticData;
import :NNetParameters;

export class SigGenDynamicData
{
public:

	void Reset();
	void StartStimulus();
	void StopStimulus();

	fMicroSecs GetStimulusTime() const;

	mV SetTime
	(
		SigGenStaticData const &,
		NNetParameters   const &
	);

	mV SetTime
	(
		SigGenStaticData const &,
		NNetParameters   const &,
		fMicroSecs       const
	);

	bool IsStimulusActive() const { return m_stimulusActive; }

private:
	bool       m_stimulusActive   { false };
	fMicroSecs m_usSimuStartStimu { 0.0_MicroSecs }; // in simu time
	fMicroSecs m_usSimuStartSpike { 0.0_MicroSecs }; // in simu time
};