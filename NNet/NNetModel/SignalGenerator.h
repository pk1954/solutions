// SignalGenerator.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "observable.h"
#include "observerInterface.h"
#include "SimulationTime.h"
#include "ParameterType.h"
#include "ActionPotential.h"
#include "SigGenData.h"

class Param;
class UPSigGenList;
class SignalGenerator;

using std::wstring;
using std::unique_ptr;

class SignalGenerator : public Observable
{
public:

	explicit SignalGenerator(UPSigGenList &);
	SignalGenerator(UPSigGenList &, wstring const &);

	fHertz GetFrequency(fMicroSecs const) const;
	mV     GetAmplitude(fMicroSecs const) const;

	fMicroSecs        const & TimePeak () const { return m_data.GetPeakTime(); }
	BASE_PEAK<fHertz> const & Frequency() const { return m_data.GetFreq();     }
	BASE_PEAK<mV>     const & Amplitude() const { return m_data.GetAmplit();   }

	void SetParam(ParamType::Value const, float const);

	void               SetData(SigGenData const &);
	SigGenData const & GetData() const;

	void            SetName(wstring const & name) { m_name = name; }
	wstring const & GetName() const               { return m_name; }

	UPSigGenList const & GetSigGenList() const {return m_list; }

	void Register(ObserverInterface * const);

	mV CalcVoltage(fMicroSecs const, fMicroSecs const);

	bool IsStimulusActive() const;

	void StartStimulus();

	fMicroSecs GetStimulusTime() const; // time til last stimulus start  

private:

	UPSigGenList & m_list;
	SigGenData     m_data;      
	wstring        m_name;
	fMicroSecs     m_usStartLastSpike { 0.0_MicroSecs }; // in stimulus time
	fMicroSecs     m_usStartStimuTime { 0.0_MicroSecs }; // in SimuTime
};
