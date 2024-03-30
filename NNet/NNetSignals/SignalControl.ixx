// SignalControl.ixx
//
// NNetSignals

module;

#include <cmath>
#include <array>
#include <Windows.h>

export module NNetSignals:SignalControl;

import Types;
import Scale;
import Color;
import PixFpDimension;
import Direct2D;
import NNetModel;
import NNetCommands;
import :NNetTimeGraph;
import :SimuRunning;

using std::array;
using std::round;

export class SignalControl : public NNetTimeGraph
{
public:
	SignalControl
	(
		HWND const,
		Observable&,
		Observable&,
		SimuRunning const&,
		float &
	);

	~SignalControl() final;

	enum class tColor { FREQ, VOLT, TIME, HIGH };
	void SetColor(tColor const t, Color const c) { m_colTable[static_cast<int>(t)] = c; }

	LPARAM AddContextMenuEntries(HMENU const) final;

	void SetVertScaleFreq(Scale<fHertz>*);
	void SetVertScaleVolt(Scale<mV>*);
	void SetHorzScale    (Scale<fMicroSecs>*);

	float ScaleFactorTimeCoord() const;
	float ScaleFactorFreqCoord() const;
	float ScaleFactorVoltCoord() const;

	bool HasScales() const final { return true; }
	bool HasGrid  () const final { return m_fGridDimFactor > 0.0f; }

private:

	enum class tPos { NONE, TIME, BASE_FREQ, PEAK_FREQ, TIME_FREQ, BASE_VOLT, PEAK_VOLT, TIME_VOLT, BASA_FREQ, BASA_VOLT };

	array<fPixelPoint, 10> m_handles;

	fPixelPoint getPos(tPos const mode) const
	{
		return m_handles[static_cast<int>(mode)];
	}

	array<Color, 4> m_colTable
	{
		D2D1::ColorF::Black, // FREQ
		D2D1::ColorF::Black, // VOLT
		D2D1::ColorF::Black, // TIME
		D2D1::ColorF::Red    // HIGH
	};

	Color getColor(tColor const type) const	{ return m_colTable[static_cast<int>(type)]; }

	fPixel const STD_DIAMOND  { 5.0_fPixel };
	fPixel const HIGH_DIAMOND { 8.0_fPixel };

	Scale<fHertz>     * m_pVertScaleFreq { nullptr };
	Scale<mV>         * m_pVertScaleVolt { nullptr };
	Scale<fMicroSecs> * m_pHorzScale     { nullptr };

	SimuRunning const & m_simuRunning;
	Observable        & m_runObservable;
	Observable        & m_dynamicModelObservable;
	float             & m_fGridDimFactor;
	tPos                m_moveMode       { tPos::NONE };

	bool snap2Grid() const { return m_fGridDimFactor > 0.0f; }

	void PaintGraphics() final;

	bool OnCommand    (WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	void OnMouseMove  (WPARAM const, LPARAM const) final;
	bool OnLButtonDown(WPARAM const, LPARAM const) final;
	bool OnLButtonUp  (WPARAM const, LPARAM const) final;
	void OnMouseLeave () final;

	fMicroSecs getTime(fPixelPoint const &) const;

	fPixel getY(fPixel const fPix) const { return yBottom() - fPix; }

	PixFpDimension<fHertz>      & vertCoordFreq()        { return m_pVertScaleFreq->GetDimension(); }
	PixFpDimension<mV>          & vertCoordVolt()        { return m_pVertScaleVolt->GetDimension(); }

	PixFpDimension<fHertz> const& vertCoordFreqC() const { return m_pVertScaleFreq->GetDimension(); }
	PixFpDimension<mV>     const& vertCoordVoltC() const { return m_pVertScaleVolt->GetDimension(); }

	fHertz getFreq(fPixel const) const;
	mV     getVolt(fPixel const) const;

	fHertz getFreq(fPixelPoint const& p) const { return getFreq(p.GetY()); }
	mV     getVolt(fPixelPoint const& p) const { return getVolt(p.GetY()); }

	fPixel yFreq(fHertz const freq) const { return getY(vertCoordFreqC().Transform2fPixelPos(freq)); }
	fPixel yVolt(mV     const volt) const { return getY(vertCoordVoltC().Transform2fPixelPos(volt)); }

	fPixel xPeak      () const { return xTime(GetSigGenStaticData()->GetPeakTime()); }
	fPixel aPeakAmplit() const { return yVolt(GetSigGenStaticData()->GetAmplitude().Peak()); }
	fPixel yBaseAmplit() const { return yVolt(GetSigGenStaticData()->GetAmplitude().Base()); }
	fPixel yPeakFreq  () const { return yFreq(GetSigGenStaticData()->GetFrequency().Peak()); }
	fPixel yBaseFreq  () const { return yFreq(GetSigGenStaticData()->GetFrequency().Base()); }

	fPixelPoint pixPntFreq(fMicroSecs const t, fHertz const f) const { return fPixelPoint(xTime(t), yFreq(f)); }
	fPixelPoint pixPntVolt(fMicroSecs const t, mV     const v) const { return fPixelPoint(xTime(t), yVolt(v)); }
	fPixelPoint pixPntStimulusFreq(fMicroSecs const t) const { return pixPntFreq(t, GetSigGenSelected()->GetStimulusFrequency(t)); }
	fPixelPoint pixPntStimulusVolt(fMicroSecs const t) const { return pixPntVolt(t, GetSigGenSelected()->GetStimulusAmplitude(t)); }

	void calcHandles();
	void paintRunControls(fMicroSecs const) const;
	void paintEditControls() const;
	void drawLine(tColor, tPos const, tPos const) const;
	void drawDiam(tColor, tPos const) const;
	void highlightMovedObject() const;
	void testPos(tPos const, fPixelPoint const, fPixel&);
	void setPos(fPixelPoint const&);
};