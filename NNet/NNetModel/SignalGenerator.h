// SignalGenerator.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "Observable.h"

class Param;

class SignalGenerator : public Observable
{
public:

    static void Initialize(Param & param) 
	{ 
		m_pParameters = & param; 
	}

	void Tick();
	void TriggerStimulus();
	void StopTrigger() { m_bTriggerActive = false; }

	fHertz const GetActFrequency ()                 const;
	fHertz const GetFrequency    (fMicroSecs const) const;

	void LoadParameterValues();
//	void SetParameterValues();

	fHertz     FreqBase() { return m_freqBase; };
	fHertz     FreqMax () { return m_freqMaxStim + m_freqBase; };
	fMicroSecs TimeMax () { return m_usMax;    };

	void SetFreqBase(fHertz     const f) { m_freqBase    = f; }
	void SetFreqMax (fHertz     const f) { m_freqMaxStim = f - m_freqBase; }
	void SetTimeMax (fMicroSecs const t) { m_usMax       = t; }

	bool       const IsTriggerActive() const { return m_bTriggerActive; }
	fMicroSecs const TimeTilTrigger () const { return m_usSinceLastStimulus; }
	bool       const InStimulusRange(fMicroSecs const t) const { return t < m_usMax * CUT_OFF_FACTOR; }

private:

	float const CUT_OFF_FACTOR { 10.0f };

	inline static Param * m_pParameters { nullptr };

	bool       m_bTriggerActive      { false };
	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };

	fHertz     m_freqBase    { };  // base frequency
	fHertz     m_freqMaxStim { };  // max stimulus frequency in addition to base freq
	fMicroSecs m_usMax       { };
};
