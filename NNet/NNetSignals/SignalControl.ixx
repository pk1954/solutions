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

using std::round;
using std::array;

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

	LPARAM AddContextMenuEntries(HMENU const) final;

	float ScaleFactorTimeCoord() const;
	float ScaleFactorFreqCoord() const;
	float ScaleFactorVoltCoord() const;

	void SetHorzScale(Scale<fMicroSecs>*);

	bool HasScales() const final { return true; }
	bool HasGrid  () const final { return m_fGridDimFactor > 0.0f; }

private:

	enum class tPos { NONE, TIME, BASE_FREQ, PEAK_FREQ, TIME_FREQ, BASE_VOLT, PEAK_VOLT, TIME_VOLT, BASA_FREQ, BASA_VOLT };

	array<fPixelPoint, 10> m_handles;

	fPixelPoint getPos(tPos const mode) const {	return m_handles[static_cast<int>(mode)]; }

	fPixel const STD_DIAMOND  { 5.0_fPixel };
	fPixel const HIGH_DIAMOND { 8.0_fPixel };

	Scale<fMicroSecs> * m_pHorzScale { nullptr };
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

	fHertz getFreq(fPixel const) const;
	mV     getVolt(fPixel const) const;

	fHertz getFreq(fPixelPoint const& p) const { return getFreq(p.GetY()); }
	mV     getVolt(fPixelPoint const& p) const { return getVolt(p.GetY()); }

	SignalGenerator  const * sigGenSelected  () const { return m_pNMWI->GetSigGenSelected(); }
	SigGenId                 sigGenIdSelected() const { return m_pNMWI->GetSigGenIdSelected(); }
	SigGenStaticData const * sigGenStaticData() const { return &sigGenSelected()->GetStaticData(); }

	fPixel xPeak      () const { return xTime(sigGenStaticData()->GetPeakTime()); }
	fPixel aPeakAmplit() const { return yVolt(sigGenStaticData()->GetAmplitude().Peak()); }
	fPixel yBaseAmplit() const { return yVolt(sigGenStaticData()->GetAmplitude().Base()); }
	fPixel yPeakFreq  () const { return yFreq(sigGenStaticData()->GetFrequency().Peak()); }
	fPixel yBaseFreq  () const { return yFreq(sigGenStaticData()->GetFrequency().Base()); }

	void calcHandles();
	void paintRunControls(fMicroSecs const) const;
	void paintEditControls() const;
	void drawLine(tColor, tPos const, tPos const) const;
	void drawDiam(tColor, tPos const) const;
	void highlightMovedObject() const;
	void testPos(tPos const, fPixelPoint const, fPixel&);
	void setPos(fPixelPoint const&);
};