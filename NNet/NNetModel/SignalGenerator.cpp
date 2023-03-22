// SignalGenerator.cpp 
//
// NNetModel

module;

#include <string>
#include <cassert>

module NNetModel:SignalGenerator;

import ObserverInterface;
import Types;
import :SigGenStaticData;
import :UPSigGenList;
import :NNetParameters;
import :NNetColors;
import :ParamType;

using std::wstring;
using std::to_wstring;

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
		default: assert(false);
	}
}

void SignalGenerator::Prepare(NNetParameters const & param)
{
	m_mVactual = m_dynData.SetTime(m_statData, param);
}

void SignalGenerator::DrawSigGen
(
	D2D_driver& graphics,
	fPixelRect  fPixRect,
	bool const  bSelected
) const
{
	if (!m_pTextFormat)
		m_pTextFormat = graphics.NewTextFormat(12.0f);
	D2D1::ColorF  const col { bSelected ? NNetColors::SIGGEN_ACTIVE : NNetColors::SIGGEN_NORMAL };
	graphics.FillRoundedRectangle(fPixRect, col, CORNERS);
	graphics.DrawRoundedRectangle(fPixRect, D2D1::ColorF::Black, CORNERS, 2._fPixel);
	graphics.DisplayText(fPixRect, GetName(), m_pTextFormat);

	fPixRect.MoveVert(15.0_fPixel);
	fHertz const frequency { GetStimulusFrequency() };
	graphics.DisplayText(fPixRect, Format2wstring<fHertz>(frequency, 1), m_pTextFormat);

	fPixRect.MoveVert(15.0_fPixel);
	mV const voltage { GetStimulusAmplitude() };
	graphics.DisplayText(fPixRect, Format2wstring<mV>(voltage, 1), m_pTextFormat);
}

void SignalGenerator::DrawNewSigGenButton
(
	D2D_driver      & graphics,
	fPixelRect const& fPixRect
) 
{
	graphics.FillRoundedRectangle(fPixRect, NNetColors::SIGGEN_NORMAL, CORNERS);
	graphics.DrawRoundedRectangle(fPixRect, D2D1::ColorF::Black, CORNERS, 2._fPixel);
	graphics.DisplayText(fPixRect, L"+", m_pTextFormat);
}
