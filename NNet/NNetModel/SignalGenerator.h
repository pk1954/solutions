// SignalGenerator.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "observable.h"
#include "observerInterface.h"
#include "ParameterType.h"
#include "SigGenStaticData.h"
#include "SigGenDynamicData.h"

class Param;
class UPSigGenList;
class SignalGenerator;

using std::wstring;
using std::unique_ptr;

class SignalGenerator : public Observable
{
public:

	explicit SignalGenerator(wstring const &);

	fHertz GetStimulusFrequency(fMicroSecs const) const;
	mV     GetStimulusAmplitude(fMicroSecs const) const;

	fMicroSecs        const & TimePeak () const { return m_statData.GetPeakTime (); }
	BASE_PEAK<fHertz> const & Frequency() const { return m_statData.GetFrequency(); }
	BASE_PEAK<mV>     const & Amplitude() const { return m_statData.GetAmplitude(); }

	void SetParam(ParamType::Value const, float const);

	void                     SetStaticData(SigGenStaticData const &);
	SigGenStaticData const & GetStaticData() const;

	void            SetName(wstring const & name) { m_name = name; }
	wstring const & GetName() const               { return m_name; }

	void ClearDynamicData() { m_dynData.Reset(); }
	void Prepare(Param const &);

	mV   GetVoltage      () const { return m_mVactual; }
	bool IsStimulusActive() const { return m_dynData.IsStimulusActive(); }

	void StartStimulus() { m_dynData.StartStimulus(); }

	fMicroSecs GetStimulusTime() const { return m_dynData.GetStimulusTime(); }

	void Register(ObserverInterface * const);

private:

	mV                m_mVactual;  
	SigGenStaticData  m_statData;      
	SigGenDynamicData m_dynData;
	wstring           m_name;
};
