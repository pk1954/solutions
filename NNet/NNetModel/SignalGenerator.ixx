// SignalGenerator.ixx
//
// NNetModel

module;

#include <memory>
#include <string>
#include <iostream>

export module NNetModel:SignalGenerator;

import ObserverInterface;
import Types;
import BasePeak;
import :ParamType;
import :SigGenStaticData;
import :SigGenDynamicData;
import :NNetParameters;
import :SignalSource;

using std::wstring;
using std::wostream;
using std::unique_ptr;

export class SignalGenerator : public SignalSource
{
public:
	explicit SignalGenerator(wstring const&);

	~SignalGenerator() final = default;

	static SignalGenerator StdSigGen;

	mV   GetSignalValue()              const final { return m_mVactual; };
	bool Includes(MicroMeterPnt const) const final { return false; };

	void Dump()                               const final {};
	void WriteInfo(wostream&)                 const final {};
	void Draw(DrawContext const&, bool const) const final {};
	void Recalc(UPNobList const&)                   final {};

	fHertz GetStimulusFrequency(fMicroSecs const) const;
	mV     GetStimulusAmplitude(fMicroSecs const) const;

	fHertz GetStimulusFrequency() const { return GetStimulusFrequency(GetStimulusTime()); };
	mV     GetStimulusAmplitude() const { return GetStimulusAmplitude(GetStimulusTime()); };

	fMicroSecs              TimePeak()  const { return m_statData.GetPeakTime(); }
	BasePeak<fHertz> const& Frequency() const { return m_statData.GetFrequency(); }
	BasePeak<mV>     const& Amplitude() const { return m_statData.GetAmplitude(); }

	void SetParam(ParamType::Value const, float const);

	void                    SetStaticData(SigGenStaticData const&);
	SigGenStaticData const& GetStaticData() const;

	void           SetName(wstring const& name) { m_name = name; }
	wstring const& GetName() const { return m_name; }

	void ClearDynamicData() { m_dynData.Reset(); }
	void Prepare(NNetParameters const&);

	bool IsStimulusActive() const { return m_dynData.IsStimulusActive(); }

	void StartStimulus() { m_dynData.StartStimulus(); }

	fMicroSecs GetStimulusTime() const { return m_dynData.GetStimulusTime(); }

	void Register(ObserverInterface&);
	void Unregister(ObserverInterface const&);

private:

	mV                m_mVactual;
	SigGenStaticData  m_statData;
	SigGenDynamicData m_dynData;
	wstring           m_name;
};
