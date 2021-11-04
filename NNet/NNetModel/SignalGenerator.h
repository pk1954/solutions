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
	void         SetStimulusMax  (fMicroSecs const, fHertz const);
	void         Tick            ();

	fHertz const GetBaseFrequency() const { return m_fBaseFrequency; }
	fHertz const GetActFrequency () const { return m_fActFrequency;  }

private:

	inline static Param const * m_pParameters { nullptr };

	bool       m_bStimulusActive     { false };
	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };
	fHertz     m_fCutoffFrequency    {  0.1_fHertz };
	fHertz     m_fBaseFrequency      { 10.0_fHertz };
	fHertz     m_fActFrequency       { };
	fHertz     m_fParamA             { 50.0_fHertz }; // Parameter for stimulus function
	float      m_fParamB             {  2.0f };       // Parameter for stimulus function
};
