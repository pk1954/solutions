// SignalGenerator.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "observable.h"
#include "ParameterType.h"

class Param;

struct SigGenData
{
	BASE_PEAK<fHertz> freq;
	BASE_PEAK<mV>     volt;
	fMicroSecs        usPeak;
};

class SignalGenerator : public Observable
{
public:

	explicit SignalGenerator(Param &);
	~SignalGenerator() final = default;

	void Tick();
	void TriggerStimulus();
	void StopTrigger() { m_bTriggerActive = false; }

	fHertz GetActFrequency()                 const;
	fHertz GetFrequency   (fMicroSecs const) const;
	mV     GetActVoltage  ()                 const;
	mV     GetVoltage     (fMicroSecs const) const;

	fMicroSecs        const & TimePeak () const { return m_data.usPeak; }
	BASE_PEAK<fHertz> const & Frequency() const { return m_data.freq;   }
	BASE_PEAK<mV>     const & Voltage  () const { return m_data.volt;   }

	void SetParam(ParamType::Value const, float const);
	void SetParams(BASE_PEAK<fHertz>, BASE_PEAK<mV>, fMicroSecs);

	void SetData(SigGenData const &);
	SigGenData GetData() const;

	void SetBaseFreq(fHertz const);
	void SetPeakFreq(fHertz const);
	void SetBaseVolt(mV const);
	void SetPeakVolt(mV const);
	void SetTimePeak(fMicroSecs const); 

	bool       IsTriggerActive() const { return m_bTriggerActive; }
	fMicroSecs TimeTilTrigger () const { return m_usSinceLastStimulus; }
	fMicroSecs CutoffTime     () const { return m_data.usPeak * CUT_OFF_FACTOR; }
	bool       InStimulusRange(fMicroSecs const t) const { return t < CutoffTime(); }

	Param const & GetParams() const { return * m_pParameters; }

private:
	float stimulusFunc(float fParam) const { return exp(1.f - fParam) * fParam;	}

	inline static float const CUT_OFF_FACTOR { 10.0f };

	Param    * m_pParameters         { nullptr };
	bool       m_bTriggerActive      { false };
	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };
	SigGenData m_data;
};
