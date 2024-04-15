// NNetTimeGraph.ixx
//
// NNetSignals

module;

#include <d2d1helper.h>
#include <cassert>
#include <compare>
#include <math.h>
#include <array>
#include <Windows.h>

export module NNetSignals:NNetTimeGraph;

import Types;
import Color;
import Scale;
import PixFpDimension;
import Direct2D;
import TimeGraph;
import NNetModel;
import Signals;

using std::array;

export class NNetTimeGraph : public TimeGraph
{
public:
	NNetTimeGraph(HWND const, LPCTSTR const);

	virtual void SetModelInterface(NNetModelWriterInterface* const);

	//void SetBackgroundColorRef(COLORREF const) override;
	void SetDefaultBackgroundColor()           override;
	
	void SetVertScaleFreq(Scale<fHertz>*);
	void SetVertScaleVolt(Scale<mV>*);
	void SetHorzScale    (Scale<fMicroSecs>*);

	enum class tColor { FREQ, VOLT, TIME, HIGH };

	void SetColor(tColor const t, Color const c) { m_colTable[static_cast<int>(t)] = c; }

protected:

	NNetModelWriterInterface* m_pNMWI { nullptr };

	Scale<fHertz>     * m_pVertScaleFreq { nullptr };
	Scale<mV>         * m_pVertScaleVolt { nullptr };
	Scale<fMicroSecs> * m_pHorzScale     { nullptr };

	NNetParameters    * GetParams() const { return &m_pNMWI->GetParams(); }

	void PaintFreqCurve(SignalGenerator const*, fMicroSecs const = 0.0_MicroSecs);
	void PaintVoltCurve(SignalGenerator const*, fMicroSecs const = 0.0_MicroSecs);

	Color GetColor(tColor const type) const	{ return m_colTable[static_cast<int>(type)]; }

	fPixel yFreq(fHertz const freq) const { return getY(vertCoordFreqC().Transform2fPixelPos(freq)); }
	fPixel yVolt(mV     const volt) const { return getY(vertCoordVoltC().Transform2fPixelPos(volt)); }

	PixFpDimension<fHertz> const& vertCoordFreqC() const { return m_pVertScaleFreq->GetDimension(); }
	PixFpDimension<mV>     const& vertCoordVoltC() const { return m_pVertScaleVolt->GetDimension(); }

	PixFpDimension<fHertz>      & vertCoordFreq()        { return m_pVertScaleFreq->GetDimension(); }
	PixFpDimension<mV>          & vertCoordVolt()        { return m_pVertScaleVolt->GetDimension(); }

	fPixelPoint pixPntStimulusFreq(SignalGenerator const *p, fMicroSecs const t) const { return pixPntFreq(t, p->GetStimulusFrequency(t)); }
	fPixelPoint pixPntStimulusVolt(SignalGenerator const *p, fMicroSecs const t) const { return pixPntVolt(t, p->GetStimulusAmplitude(t)); }

private:
	fPixelPoint pixPntFreq(fMicroSecs const t, fHertz const f) const { return fPixelPoint(xTime(t), yFreq(f)); }
	fPixelPoint pixPntVolt(fMicroSecs const t, mV     const v) const { return fPixelPoint(xTime(t), yVolt(v)); }

	array<Color, 4> m_colTable
	{
		D2D1::ColorF::Black, // FREQ
		D2D1::ColorF::Black, // VOLT
		D2D1::ColorF::Black, // TIME
		D2D1::ColorF::Red    // HIGH
	};
};