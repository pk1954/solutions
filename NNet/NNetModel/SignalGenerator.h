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

	void TriggerStimulus();

	fHertz const SetBaseFrequency(fHertz const);
	fHertz const StimulusFunc    (fMicroSecs const) const;
	float  const StimulusIntegral(fMicroSecs const) const;
	void         SetStimulusMax  (fMicroSecs const, fHertz const);
	void         Tick            ();

	fHertz const GetBaseFrequency() const { return m_fBaseFrequency; }
	fHertz const GetActFrequency () const { return m_fActFrequency;  }

	fHertz const GetFrequency(fMicroSecs const) const;

	fMicroSecs const GetPeakTime() const;

private:

	inline static Param const * m_pParameters { nullptr };

	bool       m_bStimulusActive     { false };
	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };
	fMicroSecs m_usCutoffTime        { 0._MicroSecs };
	float      m_fCutoffFactor       { 10.0f };
	fHertz     m_fBaseFrequency      { 10.0_fHertz };
	fHertz     m_fActFrequency       { };
	fHertz     m_fParamA             { 50.0_fHertz }; // Parameter for stimulus function
	float      m_fParamB             {  2.0f };       // Parameter for stimulus function
};
