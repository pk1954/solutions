// SignalGenerator.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "observable.h"
#include "observerInterface.h"
#include "ParameterType.h"
#include "SigGenData.h"

class Param;

class Stimulus : public Observable
{
public:

	void TriggerStimulus()
	{
		m_usSinceLastStimulus = 0._MicroSecs;
		m_bTriggerActive = true;
		NotifyAll(false);
	}

	void Tick(fMicroSecs const tIncr)
	{
		m_usSinceLastStimulus += tIncr;
		NotifyAll(false);
	}
	void StopTrigger() { m_bTriggerActive = false; }

	bool       IsTriggerActive() const { return m_bTriggerActive; }
	fMicroSecs TimeTilTrigger () const { return m_usSinceLastStimulus; }

private:
	bool       m_bTriggerActive      { false };
	fMicroSecs m_usSinceLastStimulus { 0._MicroSecs };
};

class SignalGenerator
{
public:

	explicit SignalGenerator(Param &);

	void Tick();
	void TriggerStimulus();
	void StopTrigger() { m_stimulus.StopTrigger(); }

	fHertz GetFrequency(fMicroSecs const uSecs) const { return m_data.GetFrequency(uSecs); }
	mV     GetAmplitude(fMicroSecs const uSecs) const {	return m_data.GetAmplitude(uSecs); }
	fHertz GetActFrequency()                    const { return GetFrequency(TimeTilTrigger()); }
	mV     GetAmplitude   ()                    const {	return GetAmplitude(TimeTilTrigger());}

	fMicroSecs        const & TimePeak () const { return m_data.GetPeakTime(); }
	BASE_PEAK<fHertz> const & Frequency() const { return m_data.GetFreq();     }
	BASE_PEAK<mV>     const & Amplitude() const { return m_data.GetAmplit();   }

	void SetParam(ParamType::Value const, float const);

	void SetData(SigGenData const &);
	SigGenData GetData() const;

	void SetFreqBase(fHertz const);
	void SetFreqPeak(fHertz const);
	void SetBaseVolt(mV const);
	void SetPeakVolt(mV const);
	void SetTimePeak(fMicroSecs const);
	void Register  (ObserverInterface &);
	void Unregister(ObserverInterface &);

	bool       IsTriggerActive() const { return m_stimulus.IsTriggerActive(); }
	fMicroSecs TimeTilTrigger () const { return m_stimulus.TimeTilTrigger(); }

	Param       & GetParams ()       { return * m_pParameters; }
	Param const & GetParamsC() const { return * m_pParameters; }

private:

	Param    * m_pParameters { nullptr };
	Stimulus   m_stimulus;
	SigGenData m_data;
};
