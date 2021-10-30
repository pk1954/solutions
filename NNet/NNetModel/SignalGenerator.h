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

	fHertz const StimulusFunc    (fMicroSecs const) const;
	void         SetStimulusMax  (fMicroSecs const, fHertz const);
	fHertz const GetBaseFrequency() const { return m_fBaseFrequency; }
	fHertz const SetBaseFrequency(fHertz const);
	fHertz const GetFrequency    ();

private:

	inline static Param const * m_pParameters { nullptr };

	bool       m_bTriggered          { false };
	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };
	fHertz     m_fCutoffFrequency    {  0.1_fHertz };
	fHertz     m_fBaseFrequency      { 10.0_fHertz };
	fHertz     m_fParamA             { 50.0_fHertz }; // Parameter for stimulus function
	float      m_fParamB             {  2.0f };       // Parameter for stimulus function
};
