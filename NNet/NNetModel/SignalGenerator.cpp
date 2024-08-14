// SignalGenerator.cpp 
//
// NNetModel

module NNetModel:SignalGenerator;

import std;
import Debug;
import ObserverInterface;
import Types;
import Signals;
import :UPSigGenList;
import :NNetParameters;
import :NNetColors;
import :ParamType;

using std::wstring;
using std::to_wstring;

static ColorF const SIGGEN_NORMAL { 1.0f, 1.0f, 0.9f, 1.0f };
static ColorF const SIGGEN_ACTIVE { 1.0f, 1.0f, 0.6f, 1.0f };

SignalGenerator::SignalGenerator(wstring const & name)
  : m_name(name)
{}

void SignalGenerator::Register(ObserverInterface & obs)
{
	m_statData.RegisterObserver(obs);
}

void SignalGenerator::Unregister(ObserverInterface const & obs)
{
	m_statData.UnregisterObserver(obs);
}

void SignalGenerator::SetStaticData(SigGenStaticData const & data) 
{ 
	m_statData = data; 
}

SigGenStaticData const & SignalGenerator::GetStaticData() const
{ 
	return m_statData; 
}

fHertz SignalGenerator::GetStimulusFrequency(fMicroSecs const stimulusTime) const 
{ 
	return m_statData.GetStimulusFrequency(stimulusTime); 
}

mV SignalGenerator::GetStimulusAmplitude(fMicroSecs const stimulusTime) const 
{	
	return m_statData.GetStimulusAmplitude(stimulusTime); 
}

void SignalGenerator::SetParam(ParamType::Value const par, float const f)
{
	switch (par)
	{
		using enum ParamType::Value;
		case inputPeakFreq: m_statData.SetFreqPeak(fHertz(f));     break;
		case inputBaseVolt: m_statData.SetAmplBase(mV(f));         break;
		case inputPeakVolt: m_statData.SetAmplPeak(mV(f));         break;
		case inputPeakTime: m_statData.SetPeakTime(fMicroSecs(f)); break;
		default: Assert(false);
	}
}

void SignalGenerator::PrepareSigGen(NNetParameters const& param)
{
	m_mVactual = m_dynData.SetTime(m_statData, param.PulseWidth());
}

void SignalGenerator::DrawSigGen
(
	D2D_driver& graphics,
	fPixelRect  fPixRect,
	bool const  bSelected
) const
{
	if (!m_hTextFormat)
		m_hTextFormat = graphics.NewTextFormat(12.0f);
	D2D1::ColorF  const col { bSelected ? SIGGEN_ACTIVE : SIGGEN_NORMAL };
	graphics.FillRoundedRectangle(fPixRect, col, CORNERS);
	graphics.DrawRoundedRectangle(fPixRect, D2D1::ColorF::Black, CORNERS, 2._fPixel);
	graphics.DisplayText(fPixRect, GetName(), m_hTextFormat);

	fPixRect.Move2Vert(15.0_fPixel);
	fHertz const frequency { GetStimulusFrequency() };
	graphics.DisplayText(fPixRect, Format2wstring<fHertz>(frequency, 1), m_hTextFormat);

	fPixRect.Move2Vert(15.0_fPixel);
	mV const voltage { GetStimulusAmplitude() };
	graphics.DisplayText(fPixRect, Format2wstring<mV>(voltage, 1), m_hTextFormat);
}

void SignalGenerator::DrawNewSigGenButton
(
	D2D_driver      & graphics,
	fPixelRect const& fPixRect
) 
{
	graphics.FillRoundedRectangle(fPixRect, SIGGEN_NORMAL, CORNERS);
	graphics.DrawRoundedRectangle(fPixRect, D2D1::ColorF::Black, CORNERS, 2._fPixel);
	graphics.DisplayText(fPixRect, L"+", m_hTextFormat);
}
