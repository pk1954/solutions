// NNetTimeGraph.ixx
//
// NNetSignals

module;

#include <d2d1helper.h>
#include <cassert>
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

	virtual void SetModelInterface(NNetModelReaderInterface const * const);

	void SetDefaultBackgroundColor() override;
	
	enum class tColor { FREQ, VOLT, TIME, HIGH };

	void SetColor(tColor const t, Color const c) { m_colTable[static_cast<int>(t)] = c; }

protected:

	NNetModelReaderInterface const * m_pNMRI { nullptr };

	NNetParameters const * GetParams() const { return &m_pNMRI->GetParamsC(); }

	void SetVertCoordFreq(PixFpDimension<fHertz> const *pCoord) { m_pVertCoordFreq = pCoord; }
	void SetVertCoordVolt(PixFpDimension<mV>     const *pCoord) { m_pVertCoordVolt = pCoord; }

	void PaintFreqCurve(SignalGenerator const*);
	void PaintVoltCurve(SignalGenerator const*);

	Color GetColor(tColor const type) const	{ return m_colTable[static_cast<int>(type)]; }

	fPixel yFreq(fHertz const freq) const { return getY(m_pVertCoordFreq->Transform2fPixelPos(freq)); }
	fPixel yVolt(mV     const volt) const { return getY(m_pVertCoordVolt->Transform2fPixelPos(volt)); }

	fPixelPoint fPPStimFreq(SignalGenerator const *p, fMicroSecs const t) const { return pixPntFreq(t, p->GetStimulusFrequency(t)); }
	fPixelPoint fPPStimVolt(SignalGenerator const *p, fMicroSecs const t) const { return pixPntVolt(t, p->GetStimulusAmplitude(t)); }

private:
	fPixelPoint pixPntFreq(fMicroSecs const t, fHertz const f) const { return fPixelPoint(xTime(t), yFreq(f)); }
	fPixelPoint pixPntVolt(fMicroSecs const t, mV     const v) const { return fPixelPoint(xTime(t), yVolt(v)); }

	PixFpDimension<fHertz> const * m_pVertCoordFreq { nullptr };
	PixFpDimension<mV>     const * m_pVertCoordVolt { nullptr };

	array<Color, 4> m_colTable
	{
		D2D1::ColorF::Black, // FREQ
		D2D1::ColorF::Black, // VOLT
		D2D1::ColorF::Black, // TIME
		D2D1::ColorF::Red    // HIGH
	};
};