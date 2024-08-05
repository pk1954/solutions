// SignalGenerator.ixx
//
// NNetModel

export module NNetModel:SignalGenerator;

import std;
import IoUtil;
import IoConstants;
import ObserverInterface;
import Direct2D;
import Types;
import BasePeak;
import Signals;
import :ParamType;
import :NNetParameters;
import :NNetSignalSource;

using std::wstring;
using std::wostream;
using std::unique_ptr;

class NNetModelIO;

export class SignalGenerator : public NNetSignalSource
{
public:
	explicit SignalGenerator(wstring const&);

	~SignalGenerator() final = default;

	NNetSignalSource::Type SignalSourceType() const final { return NNetSignalSource::Type::generator; }

	mV   GetSignalValue() const final { return m_mVactual; };
	void Dump()           const final {};

	void DrawSigGen(D2D_driver&, fPixelRect, bool const) const;

	static void DrawNewSigGenButton(D2D_driver&, fPixelRect const&);

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

	void           SetNewName(wstring const& name) { m_name = name; }
	wstring const& GetName()                const { return m_name; }
	void           WriteName(wostream& out) const { out << Quoted(m_name) << SPACE; }

	void ClearDynamicData() { m_dynData.Reset(); }
	void PrepareSigGen(NNetParameters const&);

	bool IsStimulusActive() const { return m_dynData.IsStimulusActive(); }

	void StartStimulus() { m_dynData.StartStimulus(); }

	fMicroSecs GetStimulusTime() const { return m_dynData.GetStimulusTime(); }

	void Register(ObserverInterface&);
	void Unregister(ObserverInterface const&);

	inline static fPixel const SIGGEN_WIDTH  { 100._fPixel };
	inline static fPixel const SIGGEN_HEIGHT { 50._fPixel };

	inline static fPixel const CORNERS { 5._fPixel };

private:

	mV                m_mVactual;
	SigGenStaticData  m_statData;
	SigGenDynamicData m_dynData;
	wstring           m_name;

	inline static TextFormatHandle m_hTextFormat     { nullptr }; //lazy definition
	inline static TextFormatHandle m_hTextFormatPlus { nullptr }; //lazy definition
};
