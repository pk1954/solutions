// SignalGenerator.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

class Param;

class SignalGenerator
{
public:

	static void Initialize(Param const & param) { m_pParameters = & param; }

	SignalGenerator();

	bool operator==(SignalGenerator const &) const;

	fMicroSecs const GetPulseDuration () const { return m_pulseDuration;  }
	fHertz     const GetPulseFrequency() const { return m_pulseFrequency; }

	fHertz const SetPulseFrequency(fHertz const);
	mV     const GetPotential     (fMicroSecs const);
	fHertz const StimulusFunc     (fMicroSecs const) const;
	void         SetMaximum       (fMicroSecs const, fHertz const);

private:
	inline static Param const * m_pParameters { nullptr };

	fHertz     m_pulseFrequency; // pulse frequency and pulse duration depend on each other
	fMicroSecs m_pulseDuration;  // in principle one variable would be enough, but to avoid 
								 // floating point rounding effects, both are stored

	fHertz m_fMaxFrequency { 50.0_fHertz }; // Parameter for stimulus function
	float  m_fBaseFactor   {  2.0f };       // Parameter for stimulus function
};
