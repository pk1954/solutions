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

	SignalGenerator();
	virtual ~SignalGenerator() {}

    static void Initialize(Param & param) { m_pParameters = & param; }

	void Tick();
	void TriggerStimulus();
	void StopTrigger() { m_bTriggerActive = false; }

	fHertz GetActFrequency()                 const;
	fHertz GetFrequency   (fMicroSecs const) const;

	void LoadParameterValues();

	fHertz     FreqBase() const { return m_freqBase; };
	fHertz     FreqMax () const { return m_freqMaxStim; };
	fMicroSecs TimeMax () const { return m_usMax;    };

	void SetFreqBase(fHertz     const f) { m_freqBase    = f; }
	void SetFreqMax (fHertz     const f) { m_freqMaxStim = f; }
	void SetTimeMax (fMicroSecs const t) { m_usMax       = t; }

	void SetParam(ParamType::Value const, float const);

	bool       IsTriggerActive() const { return m_bTriggerActive; }
	fMicroSecs TimeTilTrigger () const { return m_usSinceLastStimulus; }
	fMicroSecs CutoffTime     () const { return m_usMax * CUT_OFF_FACTOR; }
	bool       InStimulusRange(fMicroSecs const t) const { return t < CutoffTime(); }

	Param const & GetParams() const { return * m_pParameters; }

private:

	float const CUT_OFF_FACTOR { 10.0f };

	inline static Param * m_pParameters { nullptr };

	bool       m_bTriggerActive      { false };
	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };

	fHertz     m_freqBase    { };  // base frequency
	fHertz     m_freqMaxStim { };  // max stimulus frequency in addition to base freq
	fMicroSecs m_usMax       { };
};
