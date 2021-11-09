// SignalGenerator.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"

class Param;

class SignalGenerator
{
public:

	static void Initialize(Param & param) 
	{ 
		m_pParameters = & param; 
    }

	void Tick();
	void TriggerStimulus();

	fHertz const GetActFrequency ()                 const;
	fHertz const GetFrequency    (fMicroSecs const) const;

private:
	float const CUT_OFF_FACTOR { 10.0f };

	inline static Param * m_pParameters { nullptr };

	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };
};
