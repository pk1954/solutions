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

	//bool operator==(SignalGenerator const &) const;

	void SetStimulus();

	fHertz const GetBaseFrequency() const { return m_fBaseFrequency; }
	fHertz const SetBaseFrequency(fHertz const);
	mV     const GetPotIncrease  ();
	fHertz const StimulusFunc    (fMicroSecs const) const;
	void         SetStimulusMax  (fMicroSecs const, fHertz const);

private:
	inline static Param const * m_pParameters { nullptr };

	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };
	fHertz     m_fBaseFrequency      { 10.0_fHertz };
	fHertz     m_fParamA             { 50.0_fHertz }; // Parameter for stimulus function
	float      m_fParamB             {  2.0f };       // Parameter for stimulus function
};
