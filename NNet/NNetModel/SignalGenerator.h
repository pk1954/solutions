// SignalGenerator.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "observable.h"

class Param;

class SignalGenerator : public Observable
{
public:

	SignalGenerator();
	~SignalGenerator() override = default;

    static void Initialize(Param & param) { m_pParameters = & param; }

	void Tick();
	void TriggerStimulus();
	void StopTrigger() { m_bTriggerActive = false; }

	fHertz GetActFrequency()                 const;
	fHertz GetFrequency   (fMicroSecs const) const;
	mV     GetActPeakCurr ()                 const;
	mV     GetPeakCurr    (fMicroSecs const) const;

	void LoadParameterValues();

	fHertz     FreqBase() const { return m_freqBase;    }
	fHertz     FreqMax () const { return m_freqMaxStim; }
	fMicroSecs TimeMax () const { return m_usMax;       }
	mV         CurrMax () const { return m_mVmaxPeak;   }

	void SetParam(ParamType::Value const, float const);
	void SetParams(SignalGenerator const &);

	void SetFreqBase   (fHertz const);
	void SetFreqMax    (fHertz const);
	void SetTimeMax    (fMicroSecs const);
	void SetPeakVoltage(mV const);

	bool       IsTriggerActive() const { return m_bTriggerActive; }
	fMicroSecs TimeTilTrigger () const { return m_usSinceLastStimulus; }
	fMicroSecs CutoffTime     () const { return m_usMax * CUT_OFF_FACTOR; }
	bool       InStimulusRange(fMicroSecs const t) const { return t < CutoffTime(); }

	Param const & GetParams() const { return * m_pParameters; }

private:
	float stimulusFunc(float fParam) const { return exp(1.f - fParam) * fParam;	}

	inline static float const CUT_OFF_FACTOR { 10.0f };

	inline static Param * m_pParameters { nullptr };

	bool       m_bTriggerActive      { false };
	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };

	fHertz     m_freqBase    { };  // base frequency
	fHertz     m_freqMaxStim { };  // max stimulus frequency in addition to base freq
	fMicroSecs m_usMax       { };
	mV         m_mVmaxPeak   { };
};
